/*
 * ip_nat_proto_esp.c - Version 0.1
 *
 * NAT protocol helper module for ESP.
 */

#include <linux/config.h>
#include <linux/module.h>
#include <linux/ip.h>

#include <net/netfilter/nf_nat.h>
//#include <net/netfilter/nf_nat_rule.h>
//#include <net/netfilter/nf_nat_protocol.h>
#include <net/netfilter/nf_nat_l4proto.h>
#include <linux/netfilter/nf_conntrack_proto_esp.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Harald Welte <laforge@gnumonks.org>");
MODULE_DESCRIPTION("Netfilter NAT protocol helper module for ESP");

#if 0
//#define DEBUGP(format, args...) printk(KERN_DEBUG __FILE__ ":" __FUNCTION__": " format, ## args)
#define DEBUGP printk
#define DUMP_TUPLE_ESP(x) printk("%u.%u.%u.%u:0x%x -> %u.%u.%u.%u:0x%x\n", \
			NIPQUAD((x)->src.ip), ntohl((x)->src.u.esp.spi), \
			NIPQUAD((x)->dst.ip), ntohl((x)->dst.u.esp.spi))
#else
#define DUMP_TUPLE_ESP(x)
#define DEBUGP(x, args...)
#endif

/* is key in given range between min and max */
static bool
esp_in_range(const struct nf_conntrack_tuple *tuple,
             enum nf_nat_manip_type maniptype,
             const union nf_conntrack_man_proto *min,
             const union nf_conntrack_man_proto *max)
{
	unsigned int spi;
	DEBUGP("esp_in_range entry.\n");
	return 0;	
	if (maniptype == NF_NAT_MANIP_SRC)
		spi = tuple->src.u.esp.spi;
	else
		spi = tuple->dst.u.esp.spi;
	
	DUMP_TUPLE_ESP(tuple);

	return (ntohl(spi) >= ntohl(min->esp.spi) &&
		 ntohl(spi) <= ntohl(max->esp.spi));

}

/* generate unique tuple ... */
static void 
esp_unique_tuple(const struct nf_nat_l3proto *l3proto,
                 struct nf_conntrack_tuple *tuple,
		 const struct nf_nat_range *range,
		 enum nf_nat_manip_type maniptype,
		 const struct nf_conn *conntrack)
{
	unsigned int min, i, range_size;
	unsigned int key = 0, *spi = NULL;
	DEBUGP("esp_unique_tuple entry.\n");
	return;

	DUMP_TUPLE_ESP(tuple);

	if (maniptype == NF_NAT_MANIP_SRC)
		spi = &tuple->src.u.esp.spi;
	else
		spi = &tuple->dst.u.esp.spi;
	
	/*range is to do fast search*/
	if (! (range->flags & NF_NAT_RANGE_PROTO_SPECIFIED))
	{
			DEBUGP("NATing ESP (ct = %p)\n",conntrack);
			min = 1;
			range_size = 0xffffffff;
	} 
	else
	{
		min = ntohl(range->min_proto.esp.spi);
		range_size = ntohl(range->max_proto.esp.spi) - min + 1;
	}

	DEBUGP("min = %u, range_size = %u\n", min, range_size); 

	for (i = 0; i < range_size; i++, key++) {
		*spi = htonl(min + key % range_size);
		if (!nf_nat_used_tuple(tuple, conntrack))
			return;
	}

	DEBUGP("%p: no NAT mapping\n", conntrack);

	return;
}

/* manipulate a ESP packet according to maniptype */
static bool 
esp_manip_pkt(struct sk_buff *skb, 
              const struct nf_nat_l3proto *l3proto,
              unsigned int iphdroff, unsigned int hdroff,
              const struct nf_conntrack_tuple *tuple,
              enum nf_nat_manip_type maniptype)
{
	struct iphdr *iph = (struct iphdr *)(skb->data + iphdroff);
	struct esp_hdr *esph = (struct esp_hdr *)((unsigned int *)iph+iph->ihl);
	unsigned int *old_ip;
	unsigned int *old_spi =NULL;
	unsigned int sip = 0, dip =0,ret = 0;
        DEBUGP("\n@@@@ %s @@@@\n", __FUNCTION__);	

	
	DEBUGP("dir =[%d] manip [%u.%u.%u.%u] spi[%x].\n "
			"ip_header sip[%u.%u.%u.%u]dip[%u.%u.%u.%u].\n"
			"spi[0x%x] seq[%u].\n",
			maniptype,NIPQUAD(manip->ip),ntohl(manip->u.esp.spi),
			NIPQUAD(iph->saddr),NIPQUAD(iph->daddr),ntohl(esph->spi),ntohl(esph->seq));

	/*keep data to tuple struct*/
	if (maniptype == NF_NAT_MANIP_SRC)
	{
		/* Get rid of dst  ip / spi */
		 old_ip = &iph->daddr; //here orignal ip header
		 old_spi =&esph->spi;
	}
	else 
	{
		ret = esp_packet_in(iph,&sip,&dip);
		if (ret == NF_ACCEPT)
		{
			DEBUGP("found !! sip[%u.%u.%u.%u]dip[%u.%u.%u.%u]\n",
				NIPQUAD(sip),NIPQUAD(dip));
		 	old_ip = &dip; //here orignal ip header
		 	old_spi = &esph->spi;
		}
		else
		{
			DEBUGP("Not found !! sip[%u.%u.%u.%u]dip[%u.%u.%u.%u]\n",
				NIPQUAD(sip),NIPQUAD(dip));
			/* Get rid of dst  ip / spi */
		 	old_ip = &iph->daddr; //here orignal ip header
		 	old_spi = &esph->spi;
		}
	}
	return 1;	
}

#if 0
/* print out a nat tuple */
static unsigned int 
esp_print(char *buffer, 
	  const struct ip_conntrack_tuple *match,
	  const struct ip_conntrack_tuple *mask)
{
	unsigned int len = 0;
	DEBUGP("esp_print entry.\n");
	DEBUGP("match.....\n");
	DUMP_TUPLE_ESP(match);
	DEBUGP("mask.....\n");
	DUMP_TUPLE_ESP(mask);
	DEBUGP("end.....\n\n");
	return 0;
	if (mask->dst.u.esp.spi)
		len += sprintf(buffer + len, "dst spi=[0x%x]",
				ntohl(match->dst.u.esp.spi));

	if (mask->src.u.esp.spi)
		len += sprintf(buffer + len, "src spi=0x%x ",
				ntohl(match->src.u.esp.spi));

	return len;
}

/* print a range of keys */
static unsigned int 
esp_print_range(char *buffer, const struct ip_nat_range *range)
{
	DEBUGP("esp_print_range entry.\n");
	return 0;
	if (range->min.esp.spi != 0 
	    || range->max.esp.spi != 0xFFFFFFFF) 
	{
		if (range->min.esp.spi == range->max.esp.spi)
			return sprintf(buffer, "ESP SPI 0x%x ",
					ntohl(range->min.esp.spi));
		else
			return sprintf(buffer, "ESP SPI 0x%u-0x%u ",
					ntohl(range->min.esp.spi),
					ntohl(range->max.esp.spi));
	} else
		return 0;
}
#endif

/* nat helper struct */
static const struct nf_nat_l4proto esp = 
{ 
        .l4proto               = IPPROTO_ESP,
        .manip_pkt              = esp_manip_pkt,
        .in_range               = esp_in_range,
        .unique_tuple           = esp_unique_tuple,
#if defined(CONFIG_NF_CT_NETLINK) || defined(CONFIG_NF_CT_NETLINK_MODULE)
	.nlattr_to_range        = nf_nat_l4proto_nlattr_to_range,
#endif
};
				  
static int __init init(void)
{

        if (nf_nat_l4proto_register(NFPROTO_IPV4, &esp))
                return -EIO;
		
	DEBUGP("init IP_nat_proto_esp register.\n");
        return 0;
}

static void __exit fini(void)
{
	DEBUGP("uninit IP_nat_proto_esp register.\n");
        nf_nat_l4proto_unregister(NFPROTO_IPV4, &esp);
}

module_init(init);
module_exit(fini);

