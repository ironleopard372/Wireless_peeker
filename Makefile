CROSS=arm-openwrt-linux-
CC=$(CROSS)gcc 
CFLAGS = -g -std=gnu99 -pedantic 
CFLAGS += -I$(abspath ./src)
CFLAGS += -I$(abspath ./src/utils)
CFLAGS += -I$(abspath ./src/crypto)
LIBS_c=
OBJS= 

OBJS += main.o arp.o sniffer.o packet.o getif.o parse.o \
	MITM.o

LIBS_c += -lnet
LIBS_c += -lpcap
LIBS_c += -L ./src/l2_packet -ll2_packet
LIBS_c += -L ./src/crypto -lcrypto
LIBS_c += -L ./src/interface -liw
LIBS_c += -lnl-3 -lnl-genl-3
LIBS_c += -L ./src/utils -lutils

BINALL=MITM
ALL = $(BINALL)
all: install $(ALL)

#ifdef CONFIG_ELOOP_EPOLL
#CFLAGS += -DCONFIG_ELOOP_EPOLL -DCONFIG_CRYPTO_INTERNAL
#endif

MITM : $(OBJS)	
	$(CC) $(CFLAGS) $(OBJS) -o MITM  $(LIBS_c)

install: 
	$(MAKE) -C src

Q=@
E=echo
ifeq ($(V),1)
Q=
E=true
endif
ifeq ($(QUIET), 1)
Q=@
E=true
endif

ifdef CONFIG_CODE_CEVERAGE
%.o: %.c
	@$(E) "	CC " $<
	$(Q)cd $(dir $@); $(CC) -c -o $(notdir $@) $(CFLAGS) $(notdir $<)
else
%.o: %.c
	$(Q)$(CC) -c -o $@ $(CFLAGS) $<
	@$(E) "	CC " $<
endif

clean:
	$(MAKE) -C src clean
	rm *.o
	rm MITM
