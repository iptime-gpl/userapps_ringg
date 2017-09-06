# $Id: Makefile,v 1.1.1.1 2016/08/10 07:06:45 mt7623 Exp $
# made for GNU Make
CFLAGS = -Wall -g
EXECUTABLES = testgetifstats

all:	$(EXECUTABLES)

clean:
	rm -f *.o $(EXECUTABLES)

testmacrdr.o:	testmacrdr.c macrdr.h

testgetifstats:	testgetifstats.o getifstats.o

