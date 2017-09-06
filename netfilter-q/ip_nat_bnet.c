/* BNET extension for TCP NAT alteration. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/types.h>
#include "app.h"
#include "conn_tp.h"

#define TIMEOUT   30

struct bnet_host_t
{ 
	struct ip_conntrack_tuple conn;
	time_t register_time;
};
struct bnet_host_t  bnet_host[255];

struct udp_patch
{
	unsigned int	flag;
	unsigned int	serverip;
	unsigned int	clientip;
};

struct udp_patch udp_patch;

void bnet_init(void)
{
	memset(bnet_host, 0, sizeof(struct bnet_host_t) * 255);
	memset(&udp_patch, 0, sizeof(struct udp_patch));
}

static void iptables_rule_management(int add, struct ip_conntrack_tuple *tp)
{
	char command[256];
	char chain_name[256];

	if (!add && (tp->src.ip == 0 || tp->src.port == 0 || 
	             tp->dst.ip == 0 || tp->dst.port == 0))
		return;	

	strcpy(chain_name, "hairpin_postroute");
	sprintf(command, "iptables -t nat -%c %s -p udp -s %u.%u.%u.%u --sport %d -j SNAT --to-source %u.%u.%u.%u:%hu",
		(add) ? 'I' : 'D', chain_name,
		NIPQUAD(tp->src.ip), BNET_PORT,
		NIPQUAD(tp->dst.ip), tp->dst.port);
	system(command);
	
	strcpy(chain_name, "hairpin_preroute");
	sprintf(command, "iptables -t nat -%c %s -p udp -d %u.%u.%u.%u --dport %hu -j DNAT --to-destination %u.%u.%u.%u:%d", 
		(add) ? 'I' : 'D', chain_name,
		NIPQUAD(tp->dst.ip), tp->dst.port,
		NIPQUAD(tp->src.ip), BNET_PORT);
	system(command);

	sprintf(command, "iptables -t nat -%c app_portforward -i %s -p udp -d %u.%u.%u.%u --dport %hu -j DNAT --to-destination %u.%u.%u.%u:%d", 
		(add) ? 'I' : 'D',
		IF_LOCAL,
		NIPQUAD(tp->dst.ip), BNET_PORT,
		NIPQUAD(tp->src.ip), BNET_PORT);
	system(command);

	if (!add)
	{
		sprintf(command, "iptables -t nat -D app_portforward ! -i %s -p udp --dport %d -j DNAT --to-destination %u.%u.%u.%u",
			IF_LOCAL, BNET_PORT, NIPQUAD(tp->src.ip));	
		system(command);
	}

}

static void udp_patch_iptables(struct ip_conntrack_tuple *tp, unsigned int serverip)
{
	char command[256];

	if (udp_patch.clientip == tp->src.ip)
		return;

	sprintf(command, "iptables -t nat -A app_portforward ! -i %s -p udp --dport %d -j DNAT --to-destination %u.%u.%u.%u",
		IF_LOCAL, BNET_PORT, NIPQUAD(tp->src.ip));	
	system(command);

	if (udp_patch.flag)
	{
		sprintf(command, "iptables -t nat -D app_portforward ! -i %s -p udp --dport %d -j DNAT --to-destination %u.%u.%u.%u",
			IF_LOCAL, BNET_PORT, NIPQUAD(udp_patch.clientip));	
		system(command);
	}
	
	udp_patch.serverip = serverip;
	udp_patch.clientip = tp->src.ip;
	udp_patch.flag = 1;
}

static int get_new_client_info(
	struct ip_conntrack_tuple *tuple, 
	unsigned long extip, 
	unsigned long extport,
	unsigned long serverip)
{
	int rc, idx, i;

#if 1
	DUMP_TUPLE(tuple);
#endif

	idx = __constant_ntohl(tuple->src.ip) & 0xff;

	if (bnet_host[idx].register_time)
	{
		time_t now;

		time(&now);
		if ((now - bnet_host[idx].register_time) > TIMEOUT)
		{
			/* clear iptables rules */
			iptables_rule_management(0, &bnet_host[idx].conn);
			memset(&bnet_host[idx], 0, sizeof(struct bnet_host_t));
		}
		else
		{
			return NF_DROP;
		}
	}

	for (i=0; i<255;i++)
	{
		if((bnet_host[i].conn.dst.ip == extip ) &&
	    	   (bnet_host[i].conn.dst.port == (unsigned short)extport))
		{
			/* clear iptables rules */
			iptables_rule_management(0, &bnet_host[i].conn);
			memset(&bnet_host[i].conn, 
				0, sizeof(struct ip_conntrack_tuple));
		}
	}
	bnet_host[idx].conn.src.ip = tuple->src.ip;
	bnet_host[idx].conn.src.port = tuple->src.port;
	bnet_host[idx].conn.dst.ip = extip;
	bnet_host[idx].conn.dst.port = (unsigned short)extport;
	time(&bnet_host[idx].register_time);
	/* add iptables rules */
	iptables_rule_management(1, &bnet_host[idx].conn);

#if 1
	DUMP_TUPLE(&bnet_host[idx].conn);
#endif

	udp_patch_iptables(&bnet_host[idx].conn, serverip);
	sleep(4);

	rc = NF_ACCEPT;	

	return rc;
}

static unsigned int clear_bnet_host(struct ip_conntrack_tuple *tuple)
{
	int idx;

	//DUMP_TUPLE(tuple);
	idx = __constant_ntohl( tuple->src.ip ) & 0xff;

	if (tuple->src.port == bnet_host[idx].conn.src.port)
        {
		iptables_rule_management(0, &bnet_host[idx].conn);
		memset(&bnet_host[idx], 0, sizeof(struct bnet_host_t));
	}

	return NF_ACCEPT;
}

#define BNET_NEW_PORT_BASE        63000
int bnet_help (struct pkt_buff *pkt,  struct iphdr  *iph, unsigned int mark)
{
	struct tcphdr *th;
	struct ip_conntrack_tuple tuple;

	nfq_ip_set_transport_header(pkt, iph);
	th = nfq_tcp_get_hdr(pkt);

	tuple.src.ip = mark;
	tuple.src.port = __constant_ntohs(th->source);
	tuple.dst.ip = iph->daddr;
	tuple.dst.port = __constant_ntohs(th->dest);
	tuple.protonum = IPPROTO_TCP;

	if (iph->protocol == IPPROTO_TCP)
	{
		if (th->rst || (th->fin  && th->ack ))
			return clear_bnet_host(&tuple);
		else	
		{
			unsigned long extport = (htonl(mark)&0xff) + BNET_NEW_PORT_BASE;
			return get_new_client_info(&tuple, iph->saddr, extport, iph->daddr );
		}
	}

	return 1;
}

