# $Id: Makefile,v 1.1.1.1 2016/08/10 07:06:45 mt7623 Exp $
# made for GNU Make (and BSD make)
CFLAGS = -Wall -g -DTEST
EXECUTABLES = testobsdrdr

all:	$(EXECUTABLES)

clean:
	rm -f *.o $(EXECUTABLES)

testobsdrdr:	testobsdrdr.o obsdrdr.o
	$(CC) $(CFLAGS) -o $@ $>

obsdrdr.o:	obsdrdr.c obsdrdr.h

testobsdrdr.o:	testobsdrdr.c obsdrdr.h

