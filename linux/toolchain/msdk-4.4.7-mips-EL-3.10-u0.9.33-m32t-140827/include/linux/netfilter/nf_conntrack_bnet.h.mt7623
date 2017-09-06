#ifndef _NF_CONNTRACK_BNET_H
#define _NF_CONNTRACK_BNET_H

#define BNET_PORT 6112

extern int (*nf_nat_bnet_hook)(struct sk_buff *skb,
                                        struct nf_conn *ct, enum ip_conntrack_info ctinfo,
					unsigned int protoff, struct iphdr   *iph,	
                                        char *bnet_data);


#endif /* _NF_CONNTRACK_BNET_H */
