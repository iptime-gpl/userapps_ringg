# $Id: Makefile,v 1.1.1.1 2016/08/10 07:06:45 mt7623 Exp $
CC=gcc
CFLAGS=-Wall -g -I.

all:	testipfrdr

clean:
	rm *.o testipfrdr

testipfrdr:	testipfrdr.o ipfrdr.o
	$(CC) -o $@ $^

ipfrdr.o:	ipfrdr.c

testipfrdr.o:	testipfrdr.c

