#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "conn_tp.h"

static struct ip_conntrack_tuple conntp[2];

static int get_oneline( char *buffer, int max_size,  FILE *fp, int *size )
{
        int idx = 0,ch;

        if( (buffer == NULL) || ( max_size == 0 ) || ( fp == NULL ))
                return -1;

        while(1)
        {
                ch = fgetc(fp);
                if( ch == EOF )
                {
                        if( size )
                                *size = idx;
                        return -1;
                }
                if( idx == max_size )
                        break;

                buffer[idx] = (char)ch ;
                idx++;
                if((char)(ch == 0xa) || ( ch ==  0xd ))
                        break;
        }
        if( size )
                *size = idx;
        return 0;
}

static void read_one_tuple(char *protocol, char *buffer, struct ip_conntrack_tuple *tp)
{
        char srcip1[20], srcip2[20], dstip1[20], dstip2[20];
        char srcport1[10], srcport2[10], dstport1[10], dstport2[10];
        char proto_num[10], timeout[10];
        char connstatus1[32], connstatus2[32];
	char tcp_state[32];
        char *ptr;

        ptr = buffer;
        while( *ptr == 0x20 ) ptr++;

        sscanf( ptr, "%s ", proto_num );
        ptr += strlen( proto_num ) + 1;
        sscanf( ptr, "%s ", timeout );
        ptr += strlen( timeout ) + 1;

	if( !strcmp( "tcp", protocol ))
	{
		sscanf( ptr, "%s ", tcp_state );
		ptr += strlen( tcp_state ) + 1;
	}

        sscanf( ptr, "src=%s ", srcip1 );
        ptr += strlen( srcip1 ) + 1 + 4;
        sscanf( ptr, "dst=%s ", dstip1 );
        ptr += strlen( dstip1 ) + 1 + 4;
        sscanf( ptr, "sport=%s ", srcport1 );
        ptr += strlen( srcport1 ) + 1 + 6;
        sscanf( ptr, "dport=%s ", dstport1 );
        ptr += strlen( dstport1 ) + 1 + 6;
        if( strncmp( ptr, "src=", 4 ))
        {
                sscanf( ptr, "%s ", connstatus1 );
                ptr += strlen( connstatus1 ) + 1;
        }
        sscanf( ptr, "src=%s ", srcip2 );
        ptr += strlen( srcip2 ) + 1 + 4;
        sscanf( ptr, "dst=%s ", dstip2 );
        ptr += strlen( dstip2 ) + 1 + 4;
        sscanf( ptr, "sport=%s ", srcport2 );
        ptr += strlen( srcport2 ) + 1 + 6;
        sscanf( ptr, "dport=%s ", dstport2 );
        ptr += strlen( dstport2 ) + 1 + 6;
        sscanf( ptr, "%s ", connstatus2 );

        tp[0].src.ip = inet_addr(srcip1);
        tp[0].src.port = atoi(srcport1);
        tp[0].dst.ip = inet_addr(dstip1);
        tp[0].dst.port = atoi(dstport1);

        tp[1].src.ip = inet_addr(srcip2);
        tp[1].src.port = atoi(srcport2);
        tp[1].dst.ip = inet_addr(dstip2);
        tp[1].dst.port = atoi(dstport2);
}

int conntrack_tp_all_match(struct ip_conntrack_tuple *t1, struct ip_conntrack_tuple *t2)
{
	return ( t1->src.ip == t2->src.ip &&
 	         t1->src.port == t2->src.port &&
	         t1->dst.ip == t2->dst.ip &&
	         t1->dst.port == t2->dst.port );
}

int conntrack_tp_dst_match(struct ip_conntrack_tuple *t1, struct ip_conntrack_tuple *t2)
{
	return ( t1->dst.ip == t2->dst.ip &&
	         t1->dst.port == t2->dst.port );
}

struct ip_conntrack_tuple *ip_conntrack_find_get(
	struct ip_conntrack_tuple *tp,
	int (*cmp)(struct ip_conntrack_tuple *t1, struct ip_conntrack_tuple *t2),
	int type )
{
        FILE *fp;
        char *proto;
        char *rl1, *tr1;
        char protocol[32];
        int  end, size, got = 0;

        if( (fp = fopen( "/proc/net/ip_conntrack", "r" )) == NULL )
                return NULL;

        rl1 = malloc( 0x800 );
        if (tp->protonum == IPPROTO_TCP) proto = "tcp";
	else proto = "udp";

        while(1)
        {
                memset( rl1, 0x0, 0x200 );
                end = get_oneline( rl1, 0x200, fp, &size );

                tr1 = rl1;
                sscanf( tr1, "%s ", protocol );
                if( !strncmp( protocol, proto, strlen(proto)))
                {
                        tr1 += strlen( protocol ) + 1;
                        read_one_tuple( proto , tr1, conntp );

			if (cmp && cmp(tp, &conntp[type]))
			{
				got = 1;
				break;
			}

                }

                if( end == -1 ) break;
        }

        free(rl1);
        fclose(fp);


	if (got)
        	return conntp;
	else
		return NULL;
}

