#! /bin/sh
# $Id: iptables_display.sh,v 1.1.1.1 2016/08/10 07:06:45 mt7623 Exp $
IPTABLES=/sbin/iptables

#display all chains relative to miniupnpd
$IPTABLES -v -n -t nat -L PREROUTING
$IPTABLES -v -n -t nat -L MINIUPNPD
$IPTABLES -v -n -t filter -L FORWARD
$IPTABLES -v -n -t filter -L MINIUPNPD

