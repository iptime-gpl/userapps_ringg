# $Id: Makefile,v 1.1.1.1 2016/08/10 07:06:45 mt7623 Exp $
# made for GNU Make
CFLAGS = -Wall -g
EXECUTABLES = testgetifstats testifacewatcher

all:	$(EXECUTABLES)

clean:
	rm -f *.o $(EXECUTABLES)

testobsdrdr.o:	testobsdrdr.c obsdrdr.h

testgetifstats:	testgetifstats.o getifstats.o
	$(CC) $(CFLAGS) -o $@ $> -lkvm

testifacewatcher:	testifacewatcher.o ifacewatcher.o upnputils.o
	$(CC) $(CFLAGS) -o $@ $>

upnputils.o:	../upnputils.c

