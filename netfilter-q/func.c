#include <netinet/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include "app.h"

extern int bnet_help(struct pkt_buff *pkt,  struct iphdr  *iph, unsigned int mark);
#ifdef USE_PORT_TRIGGER
extern int port_trigger_help(struct pkt_buff *pkt,  struct iphdr  *iph);
#endif

static int tcp_process_pkt (struct pkt_buff *pkt,  struct iphdr  *iph, unsigned int mark)
{
	struct tcphdr *th;
	int verdict = NF_ACCEPT;

	nfq_ip_set_transport_header(pkt, iph);
	th = nfq_tcp_get_hdr(pkt);

	switch (__constant_ntohs(th->source))
	{
	case BNET_PORT :
		verdict = bnet_help(pkt, iph, mark);
		break;
	default :
		switch (__constant_ntohs(th->dest))
		{
		case BNET_PORT :
			verdict = bnet_help(pkt, iph, mark);
			break;
#ifdef USE_PLANTYNET
		case HTTP80_PORT :
		case HTTP8080_PORT :
			verdict = plantynet_help(pkt, iph, mark);
			break;
#endif
		default :
			break;
		}
	}
	
	return verdict;
}

static int udp_process_pkt (struct pkt_buff *pkt,  struct iphdr  *iph, unsigned int mark)
{
	struct udphdr *uh;
	unsigned int verdict = NF_ACCEPT;

	nfq_ip_set_transport_header(pkt, iph);
	uh = nfq_udp_get_hdr(pkt);

#if defined(USE_PLANTYNET) && defined(USE_PLATYNET_DNS_CHECKER)
	if (__constant_ntohs(uh->dest) == DNS53_PORT)
		verdict = plantynet_help(pkt, iph, mark);
	else
#endif
		verdict = NF_ACCEPT;

	return verdict;
}

int process_pkt (struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
              struct nfq_data *nfa, void *data)
{
	//unsigned short queue_num = ntohs(nfmsg->res_id);
	struct nfqnl_msg_packet_hdr *packet_hdr;
	unsigned char *payload;
	struct pkt_buff *pkt;
	unsigned int mark;
	struct iphdr  *iph;
	int id = 0, verdict = NF_ACCEPT, len;

	packet_hdr = nfq_get_msg_packet_hdr(nfa);
	if (packet_hdr) 
		id = ntohl(packet_hdr->packet_id);          

	len = nfq_get_payload(nfa, &payload);

	if (len)
	{
		pkt = pktb_alloc(AF_INET, payload, len, 0);	
		iph = nfq_ip_get_hdr(pkt);
		mark = nfq_get_nfmark(nfa);

#ifdef USE_PORT_TRIGGER
		if (mark == PORT_TRIGGER_FLAG )
			verdict = port_trigger_help(pkt, iph);
		else
#endif
		if (iph->protocol == IPPROTO_TCP)
			verdict = tcp_process_pkt(pkt, iph, mark);
		else if (iph->protocol == IPPROTO_UDP)
			verdict = udp_process_pkt(pkt, iph, mark);

		pktb_free(pkt);
	}
	return nfq_set_verdict (qh, id, verdict, 0, NULL);
}
