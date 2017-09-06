deps_config := \
	extra/Configs/Config.in.arch \
	extra/Configs/Config.arc \
	extra/Configs/Config.c6x \
	extra/Configs/Config.xtensa \
	extra/Configs/Config.x86_64 \
	extra/Configs/Config.vax \
	extra/Configs/Config.v850 \
	extra/Configs/Config.sparc \
	extra/Configs/Config.sh64 \
	extra/Configs/Config.sh \
	extra/Configs/Config.powerpc \
	extra/Configs/Config.mips \
	extra/Configs/Config.microblaze \
	extra/Configs/Config.nios2 \
	extra/Configs/Config.nios \
	extra/Configs/Config.metag \
	extra/Configs/Config.m68k \
	extra/Configs/Config.ia64 \
	extra/Configs/Config.i960 \
	extra/Configs/Config.i386 \
	extra/Configs/Config.hppa \
	extra/Configs/Config.h8300 \
	extra/Configs/Config.frv \
	extra/Configs/Config.e1 \
	extra/Configs/Config.cris \
	extra/Configs/Config.bfin \
	extra/Configs/Config.avr32 \
	extra/Configs/Config.arm \
	extra/Configs/Config.alpha \
	/home/hyrtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/config/uclibc/extra/Configs/Config.in

/home/hyrtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/config/uclibc/include/config/auto.conf: \
	$(deps_config)

ifneq "$(VERSION)" "0.9.33"
/home/hyrtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/config/uclibc/include/config/auto.conf: FORCE
endif
ifneq "$(ARCH)" "mips"
/home/hyrtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/config/uclibc/include/config/auto.conf: FORCE
endif

$(deps_config): ;
