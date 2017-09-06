cmd_drivers/net/rtl819x/rtl865x/../rtl8309m/mdcmdio.o := msdk-linux-gcc -Wp,-MD,drivers/net/rtl819x/rtl865x/../rtl8309m/.mdcmdio.o.d  -nostdinc -isystem /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/include -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/mips-ori/include -Iarch/mips-ori/include/generated  -Iinclude -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/mips-ori/include/uapi -Iarch/mips-ori/include/generated/uapi -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/include/uapi -Iinclude/generated/uapi -include /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/include/linux/kconfig.h -D__KERNEL__ -DVMLINUX_LOAD_ADDRESS=0x80000000 -DDATAOFFSET=0 -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -std=gnu89 -O2 -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -EL -UMIPSEB -U_MIPSEB -U__MIPSEB -U__MIPSEB__ -UMIPSEL -U_MIPSEL -U__MIPSEL -U__MIPSEL__ -DMIPSEL -D_MIPSEL -D__MIPSEL -D__MIPSEL__ -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/mips-ori/bsp -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/mips-ori/include/asm/mach-generic -Wframe-larger-than=1024 -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -O1 -DRTL_TBLDRV -D__linux__ -mno-memcpy -DRTL865X_OVER_KERNEL -DRTL865X_OVER_LINUX -Wno-implicit -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/drivers/net/rtl819x//rtl8367r    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(mdcmdio)"  -D"KBUILD_MODNAME=KBUILD_STR(mdcmdio)" -c -o drivers/net/rtl819x/rtl865x/../rtl8309m/mdcmdio.o drivers/net/rtl819x/rtl865x/../rtl8309m/mdcmdio.c

source_drivers/net/rtl819x/rtl865x/../rtl8309m/mdcmdio.o := drivers/net/rtl819x/rtl865x/../rtl8309m/mdcmdio.c

deps_drivers/net/rtl819x/rtl865x/../rtl8309m/mdcmdio.o := \
  drivers/net/rtl819x/rtl865x/../rtl8309m/rtl8309n_types.h \
  drivers/net/rtl819x/rtl865x/../rtl8309m/../common/gpio.h \
  drivers/net/rtl819x/rtl865x/../rtl8309m/mdcmdio.h \
  drivers/net/rtl819x/rtl865x/../rtl8309m/../AsicDriver/asicRegs.h \
  drivers/net/rtl819x/rtl865x/../rtl8309m/../AsicDriver/rtl865xc_asicregs.h \
    $(wildcard include/config/rtl/819x.h) \
    $(wildcard include/config/rtl8196b.h) \
    $(wildcard include/config/rtl8196c/revision/b.h) \
    $(wildcard include/config/rtl/exchange/portmask.h) \
    $(wildcard include/config/rtl/8363nb/support.h) \
    $(wildcard include/config/rtl/8364nb/support.h) \
    $(wildcard include/config/rtl/8366sc/support.h) \
    $(wildcard include/config/rtl/8365mb/support.h) \
    $(wildcard include/config/efm/patch.h) \
    $(wildcard include/config/rtl/8196d.h) \
    $(wildcard include/config/rtl/8370/support.h) \
    $(wildcard include/config/rtl/8309m/support.h) \
    $(wildcard include/config/rtl/819xd.h) \
    $(wildcard include/config/rtl/8196e.h) \
    $(wildcard include/config/rtl/8198c.h) \
    $(wildcard include/config/rtl/8197f.h) \
    $(wildcard include/config/rtk/voip/qos.h) \
    $(wildcard include/config/rtl/link/aggregation.h) \
    $(wildcard include/config/rtl/8196c.h) \
    $(wildcard include/config/rtl/8196b.h) \

drivers/net/rtl819x/rtl865x/../rtl8309m/mdcmdio.o: $(deps_drivers/net/rtl819x/rtl865x/../rtl8309m/mdcmdio.o)

$(deps_drivers/net/rtl819x/rtl865x/../rtl8309m/mdcmdio.o):
