cmd_drivers/net/rtl819x/rtl865x/../common/smi.o := mips-linux-gcc -Wp,-MD,drivers/net/rtl819x/rtl865x/../common/.smi.o.d  -nostdinc -isystem /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/toolchain/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141111/bin/../lib/gcc/mips-linux-uclibc/4.6.4/include -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/include -Iarch/rlx/include/generated  -Iinclude -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/include/uapi -Iarch/rlx/include/generated/uapi -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/include/uapi -Iinclude/generated/uapi -include /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/include/linux/kconfig.h -D__KERNEL__ -DVMLINUX_LOAD_ADDRESS=0x80000000 -DDATAOFFSET=0 -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -std=gnu89 -O2 -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -ffreestanding -EB -UMIPSEB -U_MIPSEB -U__MIPSEB -U__MIPSEB__ -UMIPSEL -U_MIPSEL -U__MIPSEL -U__MIPSEL__ -DMIPSEB -D_MIPSEB -D__MIPSEB -D__MIPSEB__ -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/ -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/soc-rtl819xd/ -Iinclude/asm-rlx -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/include/asm/mach-generic -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -fno-var-tracking-assignments -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO -O1 -DRTL_TBLDRV -D__linux__ -mno-memcpy -DRTL865X_OVER_KERNEL -DRTL865X_OVER_LINUX -Werror -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/drivers/net/rtl819x/   -ffunction-sections -fdata-sections  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(smi)"  -D"KBUILD_MODNAME=KBUILD_STR(smi)" -c -o drivers/net/rtl819x/rtl865x/../common/smi.o drivers/net/rtl819x/rtl865x/../common/smi.c

source_drivers/net/rtl819x/rtl865x/../common/smi.o := drivers/net/rtl819x/rtl865x/../common/smi.c

deps_drivers/net/rtl819x/rtl865x/../common/smi.o := \
    $(wildcard include/config/rtl/8325d/support.h) \
    $(wildcard include/config/rtl/8370/support.h) \
  drivers/net/rtl819x/rtl865x/../common/smi.h \
  include/generated/uapi/linux/version.h \
  drivers/net/rtl819x/rtl865x/../common/gpio.h \

drivers/net/rtl819x/rtl865x/../common/smi.o: $(deps_drivers/net/rtl819x/rtl865x/../common/smi.o)

$(deps_drivers/net/rtl819x/rtl865x/../common/smi.o):
