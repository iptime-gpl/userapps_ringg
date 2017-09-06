// #include <asm/byteorder.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <linux/netfilter.h>
#include <libnetfilter_queue/libnetfilter_queue.h>
#define bool int
#include <libnetfilter_queue/pktbuff.h>
#undef bool
#include <libnetfilter_queue/libnetfilter_queue_tcp.h>
#include <libnetfilter_queue/libnetfilter_queue_ipv4.h>

#include <linosconfig.h>
#define IPT_CONTINUE 0xFFFFFFFF

#define PIDFILE "/var/run/iptables-q.pid"
#define MAX_SUPPORT_MARK	10

#define BNET_PORT 	6112
#define MSM_PORT  	1863
#define MSM_FILE_PORT  	6891
#ifdef USE_SIP_WITH_STUN
#define SIP_PORT  	5060
#define STUN_PORT  	3478
#endif
#ifdef USE_PLANTYNET
#define HTTP80_PORT	80
#define HTTP8080_PORT	8080
#define DNS53_PORT	53
#endif
#define MSM_REQUEST_MARK_OFFSET 31
#define MSM_VOICE_FLAG           1
#define H323_FLAG                2
#define MSM50_FILE_FLAG          4

/* H323 sub flags */
#define H323_RRQ_FLAG   1
#define H323_ACF_FLAG   2
#define H323_Q931CONN_FLAG   4

enum 
{
	BNET_MARK = 0,
	MSM_MARK,
	MAX_MARK
};

enum
{
	BNET_MARK_VALUE = 300,
	MSM_MARK_VALUE
};


struct pkt_buff {
        unsigned char *mac_header;
        unsigned char *network_header;
        unsigned char *transport_header;

        unsigned char *head;
        unsigned char *data;
        unsigned char *tail;

        unsigned int len;
        unsigned int data_len;

        int    mangled;
};


void set_signals (void);
int process_pkt (struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
              struct nfq_data *nfa, void *data);
void q_handler (int sig);
