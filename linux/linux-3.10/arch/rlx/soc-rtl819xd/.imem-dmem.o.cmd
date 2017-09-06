cmd_arch/rlx/soc-rtl819xd/imem-dmem.o := mips-linux-gcc -Wp,-MD,arch/rlx/soc-rtl819xd/.imem-dmem.o.d  -nostdinc -isystem /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/toolchain/rsdk-4.6.4-5281-EB-3.10-0.9.33-m32ub-20141111/bin/../lib/gcc/mips-linux-uclibc/4.6.4/include -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/include -Iarch/rlx/include/generated  -Iinclude -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/include/uapi -Iarch/rlx/include/generated/uapi -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/include/uapi -Iinclude/generated/uapi -include /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/include/linux/kconfig.h -D__KERNEL__ -DVMLINUX_LOAD_ADDRESS=0x80000000 -DDATAOFFSET=0  -D__ASSEMBLY__  -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -ffreestanding -EB -UMIPSEB -U_MIPSEB -U__MIPSEB -U__MIPSEB__ -UMIPSEL -U_MIPSEL -U__MIPSEL -U__MIPSEL__ -DMIPSEB -D_MIPSEB -D__MIPSEB -D__MIPSEB__ -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/ -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/soc-rtl819xd/ -Iinclude/asm-rlx -I/home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/include/asm/mach-generic          -c -o arch/rlx/soc-rtl819xd/imem-dmem.o arch/rlx/soc-rtl819xd/imem-dmem.S

source_arch/rlx/soc-rtl819xd/imem-dmem.o := arch/rlx/soc-rtl819xd/imem-dmem.S

deps_arch/rlx/soc-rtl819xd/imem-dmem.o := \
    $(wildcard include/config/rtk/voip.h) \
    $(wildcard include/config/rtl/819xd.h) \
    $(wildcard include/config/arch/cpu/rlx5281.h) \
    $(wildcard include/config/rtl8198/revision/b.h) \
    $(wildcard include/config/rtl/89xxd.h) \
    $(wildcard include/config/rtl/89xxc.h) \
  /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/include/asm/asmmacro.h \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/mips/mt/smtc.h) \
    $(wildcard include/config/cpu/mipsr2.h) \
  /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/include/asm/hazards.h \
  include/linux/stringify.h \
  /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/include/asm/asmmacro-32.h \
    $(wildcard include/config/cpu/has/radiax.h) \
  /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/include/asm/asm-offsets.h \
  include/generated/asm-offsets.h \
  /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/include/asm/regdef.h \
  /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/include/uapi/asm/sgidefs.h \
  /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/include/asm/fpregdef.h \
  /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/include/asm/rlxregs.h \
    $(wildcard include/config/cpu/rlx5281.h) \
    $(wildcard include/config/cpu/rlx4281.h) \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/export.h \
    $(wildcard include/config/have/underscore/symbol/prefix.h) \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
  /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/include/asm/linkage.h \
  /home/rtlac/RTL3.4/rtl819x_3.4.11b/rtl819x/linux-3.10/arch/rlx/include/asm/asm.h \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
    $(wildcard include/config/sgi/ip28.h) \

arch/rlx/soc-rtl819xd/imem-dmem.o: $(deps_arch/rlx/soc-rtl819xd/imem-dmem.o)

$(deps_arch/rlx/soc-rtl819xd/imem-dmem.o):
