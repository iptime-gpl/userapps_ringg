#include "app.h"

extern void bnet_init(void);
#ifdef USE_PLANTYNET
extern void plantynet_init(void);
#endif

extern int process_pkt (struct nfq_q_handle *qh, struct nfgenmsg *nfmsg,
              struct nfq_data *nfa, void *data);


struct nfq_handle *h;
struct nfq_q_handle *qh;
struct sigaction sa;

void set_signals (void)
{
        sa.sa_handler = q_handler;
        sigemptyset (&sa.sa_mask);
        sigaddset (&sa.sa_mask, SIGTERM);
        sa.sa_flags = 0;
        sigaction (SIGTERM, &sa, 0);
}

void q_handler (int sig)
{
        if (sig == SIGTERM)
        {
		nfq_destroy_queue(qh);
		nfq_unbind_pf(h, AF_INET);
		nfq_close(h);
                unlink (PIDFILE);
                syslog (LOG_INFO, "deamon killed");
                exit (0);
        }
}

static void initialize_apps(void)
{
	bnet_init();
#ifdef USE_PLANTYNET
	plantynet_init();
#endif
}

#include "linosconfig.h"
#ifdef USE_MERGE_TO_INIT
int iptablesq_main (void)
#else
int main (int argc, char **argv)
#endif
{
	int fd, rv;
	char buf[2048];

	create_pid("iptablesq");

	initialize_apps();
	set_signals ();

	h = nfq_open();
	if (!h) {
		fprintf(stderr, "error during nfq_open()\n");
		exit(1);
	}

	printf("unbinding existing nf_queue handler for AF_INET (if any)\n");
	if (nfq_unbind_pf(h, AF_INET) < 0) {
		fprintf(stderr, "error during nfq_unbind_pf()\n");
		exit(1);
	}

	printf("binding nfnetlink_queue as nf_queue handler for AF_INET\n");
	if (nfq_bind_pf(h, AF_INET) < 0) {
		fprintf(stderr, "error during nfq_bind_pf()\n");
		exit(1);
	}

	printf("binding this socket to queue '0'\n");
	qh = nfq_create_queue(h,  0, &process_pkt, NULL);
	if (!qh) {
		fprintf(stderr, "error during nfq_create_queue()\n");
		exit(1);
	}

	printf("setting copy_packet mode\n");
	if (nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff) < 0) {
		fprintf(stderr, "can't set packet_copy mode\n");
		exit(1);
	}

	fd = nfq_fd(h);

#ifdef USE_PLANTYNET
     {
	int optval;
	int optlen = sizeof(optval);
	getsockopt(fd , SOL_SOCKET, SO_RCVBUF, (char*)&optval, &optlen);
	optval = optval * 2;
	setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char*)&optval, sizeof(optval));

	nfq_set_queue_maxlen(qh, 2048);
     }
#endif

	for (;;) 
	{
		if ((rv = recv(fd, buf, sizeof(buf), 0)) >= 0) {
			//printf("pkt received (%d)\n", rv);
			nfq_handle_packet(h, buf, rv);
			continue;
		}
        
		if (rv < 0 && errno == ENOBUFS) {
			printf("losing packets!\n");
			continue;
		}
        
		perror("recv failed");
		break;	
	}

	printf("unbinding from queue 0\n");
	nfq_destroy_queue(qh);

	printf("unbinding from AF_INET\n");
	nfq_unbind_pf(h, AF_INET);

	printf("closing library handle\n");
	nfq_close(h);

	return 0;
}

