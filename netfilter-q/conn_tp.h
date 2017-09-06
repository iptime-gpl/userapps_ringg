#define CONNTRACK_ORIGINAL	0
#define CONNTRACK_REPLY  	1

struct ip_conntrack_tuple
{
        struct {
                unsigned int ip;
                unsigned short port;
        } src;

        struct {
                unsigned int ip;
                unsigned short port;
        } dst;

        unsigned short  protonum;
};

#define NIPQUAD(addr) \
        ((unsigned char *)&addr)[0], \
        ((unsigned char *)&addr)[1], \
        ((unsigned char *)&addr)[2], \
        ((unsigned char *)&addr)[3]

#define DUMP_TUPLE(tp)                                          \
printf("tuple %p: %u %u.%u.%u.%u:%hu -> %u.%u.%u.%u:%hu\n",     \
       (tp), (tp)->protonum,                            \
       NIPQUAD((tp)->src.ip), (tp)->src.port,           \
       NIPQUAD((tp)->dst.ip), (tp)->dst.port)


int conntrack_tp_all_match(
	struct ip_conntrack_tuple *t1, 
	struct ip_conntrack_tuple *t2);

int conntrack_tp_dst_match(
	struct ip_conntrack_tuple *t1, 
	struct ip_conntrack_tuple *t2);

struct ip_conntrack_tuple *ip_conntrack_find_get(
	struct ip_conntrack_tuple *tp,
	int (*cmp)(struct ip_conntrack_tuple *t1, struct ip_conntrack_tuple *t2),
	int type );


