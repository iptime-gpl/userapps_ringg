include $(USERAPPS_ROOT)/mkdefs

#KERNEL_HEADERS=-I$(ROOTDIR)/linux-2.4.x/include
#KERNEL_HEADERS=-I$(USERAPPS_ROOT)/linux/include

#CFLAGS += -Wall -g -I$(USERAPPS_ROOT)/../../mt7623/MTKSDK/5.0.2.0/MTK_APSoC_SDK/source/lib/include $(KERNEL_HEADERS)
CFLAGS += -Wall -g -I$(USERAPPS_ROOT)/../RTL3.4/rtl819x_3.4.11b/rtl819x/toolchain/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141111/include $(KERNEL_HEADERS)


INCLUDE=-I../libbridge $(KERNEL_HEADERS) 
LIBS= -L ../libbridge -lbridge 

common_SOURCES= brctl_cmd.c brctl_disp.c 
brctl_SOURCES=  brctl.c $(common_SOURCES)

common_OBJECTS= $(common_SOURCES:.c=.o)
brctl_OBJECTS= $(brctl_SOURCES:.c=.o)

OBJECTS= $(common_OBJECTS) $(brctl_OBJECTS)

PROGRAMS= brctl 


all:	$(PROGRAMS)

install: $(PROGRAMS)
	mkdir -p $(DESTDIR)$(sbindir)
	$(INSTALL) -m 755 $(PROGRAMS) $(DESTDIR)$(sbindir)

brctl:	$(brctl_OBJECTS) ../libbridge/libbridge.a
	$(CC) $(LDFLAGS) $(brctl_OBJECTS) $(LIBS) -o brctl

romfs:
	 $(ROMFSINST) -e CONFIG_USER_BRCTL_BRCTL /bin/brctl

%.o: %.c brctl.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $< 

clean:
	rm -f *.o brctl core

