include $(USERAPPS_ROOT)/mkdefs

LIBMNL_DIR:=libmnl-1.0.4
LIBNFNETLINK_DIR:=libnfnetlink-1.0.1
LIBNETFILTER_QUEUE_DIR:=libnetfilter_queue-1.0.2

ifeq ($(RTL_SDK_VERSION), R3.4.11b)
ifeq ($(USE_BIG_ENDIAN), n)
CONFIGURE_FILE:=rtlac_le_config.sh
else
CONFIGURE_FILE:=rtlac_be_config.sh
endif
else

ifeq ($(USE_MT7623),y)
CONFIGURE_FILE:=mt7623_config.sh
else
CONFIGURE_FILE:=mt7621_config.sh
endif

endif

ifeq ($(USE_QCA),y)
CONFIGURE_FILE:=qca_config.sh
endif

all: libmnl libnfnetlink libnetfilter_queue
	rm -rf *.o

libmnl:
	make clean -C $(LIBMNL_DIR);\
	cd $(LIBMNL_DIR);\
	./$(CONFIGURE_FILE);\
	cp -ra src/libmnl.map.backup src/libmnl.map;\
	make;\

libnfnetlink:
	make clean -C $(LIBNFNETLINK_DIR);\
	cd $(LIBNFNETLINK_DIR);\
	cp -ra configure.efm configure;\
	./$(CONFIGURE_FILE);\
	make;\

libnetfilter_queue:	
	make clean -C $(LIBNETFILTER_QUEUE_DIR);\
	cd $(LIBNETFILTER_QUEUE_DIR);\
	./$(CONFIGURE_FILE);\
	make;\

clean:
	find -name '*.o' | xargs rm -f
