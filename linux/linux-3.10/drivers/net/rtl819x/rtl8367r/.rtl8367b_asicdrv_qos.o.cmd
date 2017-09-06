cmd_drivers/net/rtl819x/rtl865x/../rtl8367r/rtl8367b_asicdrv_qos.o := msdk-linux-gcc -Wp,-MD,drivers/net/rtl819x/rtl865x/../rtl8367r/.rtl8367b_asicdrv_qos.o.d  -nostdinc -isystem /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/toolchain/msdk-4.4.7-mips-EL-3.10-u0.9.33-m32t-140827/bin/../lib/gcc/mips-linux-uclibc/4.4.7/include -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/mips-ori/include -Iarch/mips-ori/include/generated  -Iinclude -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/mips-ori/include/uapi -Iarch/mips-ori/include/generated/uapi -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/include/uapi -Iinclude/generated/uapi -include /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/include/linux/kconfig.h -D__KERNEL__ -DVMLINUX_LOAD_ADDRESS=0x80000000 -DDATAOFFSET=0 -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -std=gnu89 -O2 -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding -EL -UMIPSEB -U_MIPSEB -U__MIPSEB -U__MIPSEB__ -UMIPSEL -U_MIPSEL -U__MIPSEL -U__MIPSEL__ -DMIPSEL -D_MIPSEL -D__MIPSEL -D__MIPSEL__ -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/mips-ori/bsp -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/mips-ori/include/asm/mach-generic -Wframe-larger-than=1024 -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -O1 -DRTL_TBLDRV -D__linux__ -mno-memcpy -DRTL865X_OVER_KERNEL -DRTL865X_OVER_LINUX -Wno-implicit -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/drivers/net/rtl819x//rtl8367r    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(rtl8367b_asicdrv_qos)"  -D"KBUILD_MODNAME=KBUILD_STR(rtl8367b_asicdrv_qos)" -c -o drivers/net/rtl819x/rtl865x/../rtl8367r/rtl8367b_asicdrv_qos.o drivers/net/rtl819x/rtl865x/../rtl8367r/rtl8367b_asicdrv_qos.c

source_drivers/net/rtl819x/rtl865x/../rtl8367r/rtl8367b_asicdrv_qos.o := drivers/net/rtl819x/rtl865x/../rtl8367r/rtl8367b_asicdrv_qos.c

deps_drivers/net/rtl819x/rtl865x/../rtl8367r/rtl8367b_asicdrv_qos.o := \
  /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/drivers/net/rtl819x//rtl8367r/rtl8367b_asicdrv_qos.h \
  /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/drivers/net/rtl819x//rtl8367r/rtl8367b_asicdrv.h \
  /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/drivers/net/rtl819x//rtl8367r/rtk_types.h \
    $(wildcard include/config/cpu/little/endian.h) \
  include/generated/uapi/linux/version.h \
  /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/drivers/net/rtl819x//rtl8367r/rtk_error.h \
  /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/drivers/net/rtl819x//rtl8367r/rtl8367b_reg.h \
    $(wildcard include/config/rst/offset.h) \
    $(wildcard include/config/rst/mask.h) \
    $(wildcard include/config/dummy/15/offset.h) \
    $(wildcard include/config/dummy/15/mask.h) \
    $(wildcard include/config/sel/offset.h) \
    $(wildcard include/config/sel/mask.h) \
  /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/drivers/net/rtl819x//rtl8367r/rtl8367b_base.h \

drivers/net/rtl819x/rtl865x/../rtl8367r/rtl8367b_asicdrv_qos.o: $(deps_drivers/net/rtl819x/rtl865x/../rtl8367r/rtl8367b_asicdrv_qos.o)

$(deps_drivers/net/rtl819x/rtl865x/../rtl8367r/rtl8367b_asicdrv_qos.o):
