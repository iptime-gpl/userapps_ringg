#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#include "app.h"
#include "conn_tp.h"

#define CACHE_NUMBER	1024
#define CACHE_URL_LEN	256
#define CACHE_TIMEOUT	3600	// second

/* NF_DROP:0,  NF_ACCEPT:1 */
#define RES_NOTICE	2

struct {
	char   url[CACHE_URL_LEN];
	time_t updated;
	char   redir_url[CACHE_URL_LEN];
	char   result;	// NF_ACCEPT, NF_DROP, RES_NOTICE
} plantynet_cache[CACHE_NUMBER] = {{"",0,"",0},};


#define NOTI_SITE_NUM 		6
char notification_site[NOTI_SITE_NUM][32] =  {
	"m.naver.com/",   	
	"www.naver.com/", 	
	"m.daum.net/",    	
	"www.daum.net/",  	
	"m.nate.com/",    	
	"www.nate.com/",  
};


int cache_count = 0 ;

int check_plantynet_cache(char *url)
{
	time_t now;
	int i;

#if 0 // test
	if (strstr(url, "joins"))
	{
		strcpy(url, "www.plantynet.com");
		return NF_DROP;
	}
#endif

	time(&now);

	for (i=0; i<CACHE_NUMBER; i++)
	{
		if ((plantynet_cache[i].result != -1) && !strcmp(url, plantynet_cache[i].url))
		{
			if ((now - plantynet_cache[i].updated) >= CACHE_TIMEOUT)
			{
				strcmp(plantynet_cache[i].url, "");
				return -1;
			}
			else
				strcpy(url, plantynet_cache[i].redir_url);
#if 0
			printf("[cache[%d]](%d,%d,%d) [%d][%s][%s]\n", i, 
				(int)now,
				(int)plantynet_cache[i].updated,
				(int)(now - plantynet_cache[i].updated),
				(int)plantynet_cache[i].result, 
				plantynet_cache[i].url,
				plantynet_cache[i].redir_url);
#endif
			return (int)plantynet_cache[i].result;
		}
	}

	return -1;
}

int add_plantynet_cache(char *url, int result)
{
	if (strlen(url) >= CACHE_URL_LEN)
		return 0;

//	printf("added cache [%d] : %s \n", cache_count, url);

	if (result == RES_NOTICE) /* Notice - just one time cache */
	{
		syslog_msg(0, "Plantynet Notice (%s)", url);
		return 0;
	}	
	else if (result == -1)
		strcpy(plantynet_cache[cache_count].url, url);
	else
		strcpy(plantynet_cache[cache_count].redir_url, url);

	plantynet_cache[cache_count].result = (char)result;
	time(&plantynet_cache[cache_count].updated);

	if ((result != -1) && (++cache_count >= CACHE_NUMBER))
		cache_count = 0;


	return 0;
}

int check_notification_site(char *url)
{
	int i;

	for (i=0; i<NOTI_SITE_NUM; i++)
	{
		if (!strcmp(notification_site[i], url))
			return 1;
	}

	return 0;
}

struct pseudohdr {

        unsigned int   saddr;
        unsigned int   daddr;
        unsigned char  useless;
        unsigned char  protocol;
        unsigned short tcplength;
};

static unsigned short csum(unsigned short *buf, unsigned int nbytes)
{
        unsigned int sum = 0;

        for (; nbytes > 1; nbytes -= 2)
                sum += *buf++;

        if (nbytes == 1)
		sum += *buf & ntohs(0xff00); 

        sum  = (sum >> 16) + (sum & 0xFFFF);
        sum += (sum >> 16);

        return ~sum;
}

const char month_tab[48] =
    "Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec ";
const char day_tab[] = "Sun,Mon,Tue,Wed,Thu,Fri,Sat,";

void rfc822_time_buf(char *buf, time_t s)
{
    struct tm *t;
    char *p;
    unsigned int a;

    t = gmtime(&s);

    p = buf + 28;
    /* p points to the last char in the buf */

    p -= 3;
    /* p points to where the ' ' will go */
    memcpy(p--, " GMT", 4);

    a = t->tm_sec;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ':';
    a = t->tm_min;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ':';
    a = t->tm_hour;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ' ';
    a = 1900 + t->tm_year;
    while (a) {
        *p-- = '0' + a % 10;
        a /= 10;
    }
    /* p points to an unused spot to where the space will go */
    p -= 3;
    /* p points to where the first char of the month will go */
    memcpy(p--, month_tab + 4 * (t->tm_mon), 4);
    *p-- = ' ';
    a = t->tm_mday;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ' ';
    p -= 3;
    memcpy(p, day_tab + t->tm_wday * 4, 4);
}

static int make_http_packet(char *sndbuf, struct iphdr *rx_iph, struct tcphdr *rx_tcph, int syn, int ack, int fin, char *host)
{
        struct ethhdr *ethh = (struct ethhdr *)(sndbuf+2);
        struct iphdr *iph = (struct iphdr *)((char *)ethh + sizeof(struct ethhdr));
        struct tcphdr *tcph = (struct tcphdr *)((char *)iph + sizeof(struct iphdr));
        struct pseudohdr *pseudo_header;
	char *data = (char *)tcph + sizeof(struct tcphdr);
	int rx_datalen, datalen;
	time_t current_time;
	char ctime_str[64];

	rx_datalen = ntohs(rx_iph->tot_len) - (rx_iph->ihl*4) - (rx_tcph->doff*4);

        tcph->source = rx_tcph->dest;
        tcph->dest = rx_tcph->source;
        tcph->seq= rx_tcph->ack_seq;
        tcph->ack_seq = htonl(ntohl(rx_tcph->seq) + rx_datalen);
        tcph->res1 = 0;
        tcph->doff = sizeof(struct tcphdr) / 4;
        tcph->fin = (fin)?1:0;
        tcph->syn = (syn)?1:0;
        tcph->rst = 0;
        tcph->psh = 0;
        tcph->ack = (ack)?1:0;
        tcph->urg = 0;
        tcph->ece = 0;
        tcph->cwr = 0;
        tcph->window = htons(512);
        tcph->urg_ptr = htons(0);


	time(&current_time);
	rfc822_time_buf(ctime_str, current_time);

	datalen = sprintf(data, 
		"HTTP/1.1 302 Found\r\n"
		"Date: %s\r\n"
		"Cache-Control: no-cache, no-store, must-revalidate\r\n"
		"Server: Httpd/1.0\r\n"
		"location: http://%s\r\n"
		"Content-Length: 3\r\n"
		"Keep-Alive: timeout=5, max=100\r\n"
		"Connection: Keep-Alive\r\n"
		"Content-Type: text/html\r\n"
		"\r\n~\n\n", ctime_str, host);

        pseudo_header = (struct pseudohdr *)((char*)tcph - sizeof(struct pseudohdr));
        pseudo_header->saddr = rx_iph->daddr;
        pseudo_header->daddr = rx_iph->saddr;
        pseudo_header->protocol = IPPROTO_TCP;
        pseudo_header->tcplength = htons( sizeof(struct tcphdr) + datalen );

        tcph->check = csum((u_short *)pseudo_header, sizeof(struct pseudohdr) + sizeof(struct tcphdr) + datalen);

        iph->ihl = 5;
        iph->version = 4;
        iph->tos = 0;
        iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct tcphdr) + datalen);
        iph->id = htons(54321);
        iph->frag_off = 0;
        iph->ttl = 128; // hops
        iph->protocol = IPPROTO_TCP;
        iph->check = 0;
        iph->saddr = rx_iph->daddr;
        iph->daddr = rx_iph->saddr;
        iph->check = csum((unsigned short *)iph, sizeof(struct iphdr));

	//syslog_msg(0, "tcpsum:%04x,  ipsum:%04x, datalen : %d", tcph->check, iph->check, datalen);

	return (sizeof(struct iphdr) + sizeof(struct tcphdr) + datalen);
}

static void send_fake_http_response(struct iphdr *iph, struct tcphdr *tcph, char *host)
{
        struct sockaddr_ll server;
        char sndbuf[1514], mac_str[32];
        struct ethhdr *ethh_reply = (struct ethhdr *)(sndbuf+2);
        int buflen=0, mac[6], i;
        int s;          /* socket */
	struct in_addr in;

        memset(sndbuf, 0, 1514);

	in.s_addr = iph->saddr;
	get_hwaddr_by_ip(inet_ntoa(in), mac_str);
	sscanf(mac_str, "%x:%x:%x:%x:%x:%x",
		&mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
	for (i=0; i<6; i++)
        	ethh_reply->h_dest[i] = (unsigned char)mac[i];  /* MAC DA */
	get_hwaddr_kernel_raw(IF_LOCAL, ethh_reply->h_source);  /* MAC SA */
        ethh_reply->h_proto = htons(ETH_P_IP);        /* protocol type (Ethernet) */

        buflen = make_http_packet(sndbuf, iph, tcph, 0, 1, 1, host);
        buflen += sizeof(struct ethhdr);

        if ((s = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP))) == -1) {
                printf("Could not open raw socket");
		return;
        }
        bzero(&server, sizeof(server));
        server.sll_family = AF_PACKET;
        server.sll_protocol = htons(ETH_P_IP);
        server.sll_ifindex = if_nametoindex(IF_LOCAL);

        if (bind(s, (struct sockaddr *) &server, sizeof(struct sockaddr_ll)) < 0)
        {
                printf("tcp cannot bind \n");
                return ;
        }

        if (send(s, &sndbuf[2], buflen, 0) < 0)
                printf("tcp sendto error \n");
        close(s);
}

static int find_pattern(unsigned char *data, int dlen,
        char *pattern, int plen,
        char term,
        int *numoff,
        int *numlen)
{
    size_t i, j, k;
    int state = 0;
    *numoff = *numlen = 0;

    if (dlen == 0)
        return 0;

    if (dlen <= plen) { /* Short packet: try for partial? */
        if (strncasecmp((char *)data, pattern, dlen) == 0)
            return -1;
        else
            return 0;
    }
    for (i = 0; i <= (dlen - plen); i++) {
        /* DFA : \r\n\r\n :: 1234 */
        if (*(data + i) == '\r') {
            if (!(state % 2)) state++;  /* forwarding move */
            else state = 0;             /* reset */
        }
        else if (*(data + i) == '\n') {
            if (state % 2) state++;
            else state = 0;
        }
        else state = 0;

        if (state >= 4)
            break;

#if 0
	{
		char buf[128], *ptr;
		memset(buf, 0, 128);
		ptr = data + i;
		if (ptr[0] == 'H')
		{
			strncpy(buf, ptr, plen);
			printf("** %s\n", buf);
		}
	}
#endif

        /* pattern compare */
        if (memcmp(data + i, pattern, plen ) != 0)
            continue;

        /* Here, it means patten match!! */
        *numoff=i + plen;
        for (j = *numoff, k = 0; data[j] != term; j++, k++)
            if (j > dlen) return -1 ;   /* no terminal char */

        *numlen = k;
        return 1;
    }
    return 0;
}

static int check_http_request(struct pkt_buff *pkt,  struct iphdr  *iph, unsigned int mark)
{
	struct tcphdr *th;
	unsigned char *data;
	int found, datalen, offset, pathlen;
	char url[1024], host[2048];
	int rc = NF_ACCEPT;
	struct in_addr in;

	nfq_ip_set_transport_header(pkt, iph);
	th = nfq_tcp_get_hdr(pkt);
	data = (unsigned char *)th + sizeof(struct tcphdr);

	datalen = pkt->data_len - (iph->ihl*4) - (th->doff*4);

	if (datalen <= (40 + strlen("GET") + strlen(" HTTP/x.x")))
		return NF_ACCEPT;

	found = find_pattern(data, datalen, "GET ", sizeof("GET ") - 1, '\r', &offset, &pathlen);
	if (!found)
		found = find_pattern(data, datalen, "POST ", sizeof("POST ") - 1, '\r', &offset, &pathlen);

	// GET, POST path
	if (!found || (pathlen -= (sizeof(" HTTP/x.x") - 1)) <= 0)
		return NF_ACCEPT;
	memset(url, 0, sizeof(url));
	strncpy(url, (char *)(data + offset), pathlen);

	// Host
	found = find_pattern(data, datalen, "Host: ", sizeof("Host: ") - 1, '\r', &offset, &pathlen);
	if (!found)
		return NF_ACCEPT;
	memset(host, 0, sizeof(host));
	strncpy(host, (char *)(data + offset), pathlen);

	// Full URL
	strcat(host, url);
	//host[CACHE_URL_LEN] = '\0';

	in.s_addr = iph->saddr;
	rc = check_plantynet_cache(host);

	if ((mark == PLANTYNET_NOTICE_MARK) && check_notification_site(host))
	{
		char tag[256];

		sprintf(tag, "plantynet_event_value_%d", PLANTYNET_SYNC_NOTICE);
		istatus_get_value_direct(tag, host);
		notice_plantynet_view_node(inet_ntoa(in));
		rc = RES_NOTICE;
	}
	else if (rc == -1)
	{	
		char mac_str[32];

		get_hwaddr_by_ip(inet_ntoa(in), mac_str);

		add_plantynet_cache(host, -1);
		rc = plantynet_service_request("UDP", 'Q', mac_str, host);
		if (rc != -1)
			add_plantynet_cache(host, rc);
	}	

	if ((rc == NF_DROP) || (rc == RES_NOTICE))
	{
		send_fake_http_response(iph, th, host);
		rc = NF_DROP;
	}
	else
		rc = NF_ACCEPT;

	return rc;
}

#ifdef USE_PLATYNET_DNS_CHECKER
struct dnshdr
{
        unsigned short  id;
        unsigned char   flag1;
        unsigned char   flag2;
        unsigned short  qc;     /* question count */
        unsigned short  ac;     /* answer count */
        unsigned short  auc;    /* authority count */
        unsigned short  arc;    /* additional record count */
};

struct zonesection
{
        char    *name;
        unsigned short  type;
        unsigned short  class;
};

struct answersection2
{
        unsigned short  np;     /* name pointer */
        unsigned short  type;
        unsigned short  class;
        unsigned short  unused;
        unsigned short  ttl;
        unsigned short  len;
        unsigned int    address;
};

static int make_dns_packet(char *sndbuf, struct iphdr *rx_iph, struct udphdr *rx_udph, struct dnshdr *rx_dnsh, char *host)
{
        struct ethhdr *ethh = (struct ethhdr *)(sndbuf+2);
        struct iphdr *iph = (struct iphdr *)((char *)ethh + sizeof(struct ethhdr));
	struct udphdr *uh = (struct udphdr *)((char *)iph + sizeof(struct iphdr));
	struct dnshdr *dnsh = (struct dnshdr *)((char*)uh + sizeof(struct udphdr));
	unsigned char *data = (unsigned char *)dnsh + sizeof(struct dnshdr);
	struct zonesection *zp;
	struct answersection2 as2;
	char *a2p, *ptr;
	int datalen, namesize;
	struct hostent *he;
	struct in_addr   sin_addr;

	ptr = strchr(host, '/');
	if (ptr) *ptr = '\0';

	if ((he=gethostbyname(host)) == NULL)
	{
		printf("plantynet : fake dns, gethostbyname error. (%s)\n", host);
		return 0;
	}	
	sin_addr = *((struct in_addr *)he->h_addr);

        iph->ihl = 5;
        iph->version = 4;
        iph->tos = 0;
        iph->tot_len = 0;
        iph->id = htons(54321);
        iph->frag_off = 0;
        iph->ttl = 128; // hops
        iph->protocol = IPPROTO_UDP;
        iph->check = 0;
        iph->saddr = rx_iph->daddr;
        iph->daddr = rx_iph->saddr;

        uh->source = rx_udph->dest;
        uh->dest = rx_udph->source;
        uh->check = 0;

	namesize = 0;
	while(data[namesize] != '\0')
	{
		namesize++;
		if (namesize > 256) return 0;
		}
	namesize += 1; // NULL string

	memcpy((char *)dnsh, (char *)rx_dnsh, sizeof(struct dnshdr));
	dnsh->flag1 = 0x81;
        dnsh->flag2 = 0x80;
        dnsh->ac = htons(0x1);
        datalen = sizeof(struct dnshdr);

	zp = (struct zonesection *)((unsigned char *)dnsh + sizeof(struct dnshdr));
	memcpy(zp, data, namesize+4);
	datalen += (namesize + 4);

	memset(&as2, 0, sizeof(struct answersection2));
	a2p = (unsigned char *)zp + namesize + 4;
	as2.np = htons(0xc00c);
	as2.type = htons(0x01);
	as2.class = htons(0x01);
	as2.unused = htons(0x0);
	as2.ttl = htons(0x0a);
	as2.len = htons(0x04);
	as2.address = (unsigned int)sin_addr.s_addr;
	memcpy(a2p, (char *)&as2, sizeof(struct answersection2));
	datalen += sizeof(struct answersection2);

	datalen += sizeof(struct iphdr) + sizeof(struct udphdr);

	iph->tot_len = htons((unsigned short)datalen);
        iph->check = csum((unsigned short *)iph, sizeof(struct iphdr));

	uh->len = htons(ntohs(iph->tot_len) - sizeof(struct iphdr));

	return datalen;
}

static void send_fake_dns_response(struct iphdr *iph, struct udphdr *udph, struct dnshdr *dnsh, char *host)
{
        struct sockaddr_ll server;
        char sndbuf[1024], mac_str[32];
        struct ethhdr *ethh_reply = (struct ethhdr *)(sndbuf+2);
        int buflen=0, mac[6], i;
        int s;          /* socket */
	struct in_addr in;

        memset(sndbuf, 0, 1024);

	in.s_addr = iph->saddr;
	get_hwaddr_by_ip(inet_ntoa(in), mac_str);
	sscanf(mac_str, "%x:%x:%x:%x:%x:%x",
		&mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);
	for (i=0; i<6; i++)
        	ethh_reply->h_dest[i] = (unsigned char)mac[i];  /* MAC DA */
	get_hwaddr_kernel_raw(IF_LOCAL, ethh_reply->h_source);  /* MAC SA */
        ethh_reply->h_proto = htons(ETH_P_IP);        /* protocol type (Ethernet) */

        buflen = make_dns_packet(sndbuf, iph, udph, dnsh, host);
	if (buflen == 0)
		return;
        buflen += sizeof(struct ethhdr);

#if 0
{
	printf("send_fake_dns_response() \n");

	for (i=0; i<buflen-2; i++)
	{
		printf("%02x ", sndbuf[i+2] & 0xff);
		if (((i+1) % 16) == 0)
			printf("\n");
	}
}
#endif

        if ((s = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP))) == -1) {
                printf("Could not open raw socket");
		return;
        }
        bzero(&server, sizeof(server));
        server.sll_family = AF_PACKET;
        server.sll_protocol = htons(ETH_P_IP);
        server.sll_ifindex = if_nametoindex(IF_LOCAL);

        if (bind(s, (struct sockaddr *) &server, sizeof(struct sockaddr_ll)) < 0)
        {
                printf("tcp cannot bind \n");
                return ;
        }

        if (send(s, &sndbuf[2], buflen, 0) < 0)
                printf("tcp sendto error \n");
        close(s);
}

static int check_dns_request(struct pkt_buff *pkt,  struct iphdr  *iph)
{
	struct udphdr *uh;
	struct dnshdr *dnsh;
	unsigned char *data;
	char host[CACHE_URL_LEN+1], *ptr, n;
	int rc = NF_ACCEPT;

	nfq_ip_set_transport_header(pkt, iph);
	uh = nfq_udp_get_hdr(pkt);
	dnsh = (struct dnshdr *)((unsigned char*)uh + sizeof(struct udphdr));
	data = (unsigned char *)dnsh + sizeof(struct dnshdr);

	memset(host, 0x0, CACHE_URL_LEN+1);
	ptr = &host[0];

	n = data[0];
	while (n != 0)
	{	
		strncat(ptr, data+1, n);
		data += (n + 1);
		if ((n = data[0]) != 0)
			strcat(ptr, ".");
	}


	rc = check_plantynet_cache(host);
	if (rc == -1)
	{
		struct in_addr in;
		char mac_str[32];

		in.s_addr = iph->saddr;
		get_hwaddr_by_ip(inet_ntoa(in), mac_str);

		add_plantynet_cache(host, -1);
		rc = plantynet_service_request("UDP", 'Q', mac_str, host);
		if (rc != -1)
			add_plantynet_cache(host, rc);
	}

	if (rc == NF_DROP)
		send_fake_dns_response(iph, uh, dnsh, host);
	else
		rc = NF_ACCEPT;

	return rc;
}
#endif

int plantynet_help(struct pkt_buff *pkt,  struct iphdr  *iph, unsigned int mark)
{
	int rc = NF_ACCEPT;

	if (iph->protocol == IPPROTO_TCP)
	{
		rc = check_http_request(pkt, iph, mark);
	}
#ifdef USE_PLATYNET_DNS_CHECKER
	else if (iph->protocol == IPPROTO_UDP)
	{
		rc = check_dns_request(pkt, iph);
	}
#endif

	return rc;
}

void plantynet_init(void)
{
	char command[256];

	/* move to NAT Helper Function */

	sprintf(command, "/sbin/iptables -D plantynet -i %s -p tcp -m multiport --destination-port 80,8080 -j NFQUEUE --queue-num 0", IF_LOCAL);
	system(command);
	sprintf(command, "/sbin/iptables -A plantynet -i %s -p tcp -m multiport --destination-port 80,8080 -j NFQUEUE --queue-num 0", IF_LOCAL);
	system(command);
#ifdef USE_PLATYNET_DNS_CHECKER
	sprintf(command, "/sbin/iptables -A plantynet -i %s -p udp --dport 53 -j NFQUEUE --queue-num 0", IF_LOCAL);
	system(command);
#endif

	system("/bin/echo 2048 > /proc/sys/net/ipv4/ip_queue_maxlen");
}
