# $Id: Makefile,v 1.1.1.1 2016/08/10 07:06:45 mt7623 Exp $
CC=gcc
CFLAGS=-Wall -g -I.
RM=rm -f

all:	testipfwrdr

clean:
	$(RM) *.o testipfwrdr

testipfwrdr:	testipfwrdr.o ipfwrdr.o
	$(CC) -o $@ $^

ipfwrdr.o:	ipfwrdr.c

testipfwrdr.o:	testipfwrdr.c

