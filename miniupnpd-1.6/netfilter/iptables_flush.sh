#! /bin/sh
# $Id: iptables_flush.sh,v 1.1.1.1 2016/08/10 07:06:45 mt7623 Exp $
IPTABLES=/sbin/iptables

#flush all rules owned by miniupnpd
$IPTABLES -t nat -F MINIUPNPD
$IPTABLES -t filter -F MINIUPNPD

