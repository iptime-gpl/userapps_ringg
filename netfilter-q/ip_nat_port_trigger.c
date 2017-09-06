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
#include "linosconfig.h"

#include <lib/portforward_v2_api.h>
static void iptables_rule_management(char op, porttrigger_nf_t *pt_rule, int chain_idx)
{
	char cmd[512];
	int i;

	memset(cmd, 0x0 ,512);
	//snprintf(cmd,512,"/sbin/iptables -t nat -%c app_portforward ! -i %s ", op, IF_LOCAL);
	if(op == 'D'){
		snprintf(cmd,512,"/sbin/iptables -t nat -%c preroute_trigger", op);
	}
	else{
		snprintf(cmd,512,"/sbin/iptables -t nat -%c preroute_trigger %d", op, chain_idx);
	}
	snprintf(cmd, 512, "%s -p %s -m multiport --destination-port ", cmd, pt_rule->forward_protocol);

	i = 0;
	do {
		if (i) snprintf(cmd,512, "%s,",cmd);
		snprintf(cmd, 512, "%s%d", cmd, pt_rule->forward_ports[i]);
		i++;
	} while (pt_rule->forward_ports[i] && i < __MAX_PT_FORWARD_LENGTH);

	snprintf(cmd,512, "%s -j DNAT --to-destination %s", cmd, pt_rule->forward_ip);

	system(cmd);
}

static int get_chain_idx_tforward(portforward_ll_t *pf_ll, portforward_rule_t *rt)
{
	int chain_idx = 1;
        portforward_rule_t *rtmp;

        if(rt == NULL)  return -1;

        rtmp = pf_ll->head;

        while(rtmp){
                if(rtmp == rt){
                        return chain_idx;
                }
                if(rtmp->enable && rtmp->trigger == __TRIGGER_RULE && strlen(rtmp->trlist->forward_ip)){
                        chain_idx += 1;
                }
                rtmp = rtmp->next;
        }
        return chain_idx;
}

int port_trigger_help(struct pkt_buff *pkt,  struct iphdr  *iph)
{
	struct tcphdr *tcph = NULL;
	struct udphdr *udph = NULL;
	portforward_ll_t pf_ll;
	portforward_rule_t *rt;
	char ipaddrtmp[__MAX_PT_IP_LENGTH];
	unsigned short trigger_port;

	nfq_ip_set_transport_header(pkt, iph);

	if (iph->protocol == IPPROTO_TCP)
	{
#ifdef USE_KT_TELECOP
		{
			char ipstr[20];
			sprintf(ipstr, "%u.%u.%u.%u", NIPQUAD(iph->daddr));
			if (!strcmp(ipstr, KT_TELECOP_TRIGGER_IP))
			{
				sprintf(ipstr, "%u.%u.%u.%u", NIPQUAD(iph->saddr));
				kt_telecop_trigger_forward_triggerv2(ipstr);
				return NF_ACCEPT;
			}
		}
#endif
		tcph = nfq_tcp_get_hdr(pkt);
		trigger_port = ntohs(tcph->dest);
	}
	else if (iph->protocol == IPPROTO_UDP)
	{
		udph = nfq_udp_get_hdr(pkt);
		trigger_port = ntohs(udph->dest);
	}
	else
	{
		return NF_ACCEPT;
	}	

	portforward_read_db("user_pf", &pf_ll);
	
	rt = portforward_search_by_trigger_port(&pf_ll,(iph->protocol == IPPROTO_TCP)?"tcp":"udp", trigger_port);
	snprintf(ipaddrtmp, __MAX_PT_IP_LENGTH, "%u.%u.%u.%u", NIPQUAD(iph->saddr));
	if((rt) && (strcmp(rt->trlist->forward_ip, ipaddrtmp))){
		if(strlen(rt->trlist->forward_ip)){
			iptables_rule_management('D', rt->trlist, 0);
		}
		snprintf(rt->trlist->forward_ip, __MAX_PT_IP_LENGTH, "%u.%u.%u.%u", NIPQUAD(iph->saddr));
		iptables_rule_management('I', rt->trlist, get_chain_idx_tforward(&pf_ll, rt));
		portforward_write_db("user_pf", &pf_ll);
	}
	free_portforward_list(&pf_ll);

	return NF_ACCEPT;
}
