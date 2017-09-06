	.file	1 "gen_lowlevelcond.c"
	.section .mdebug.abi32
	.previous
	.gnu_attribute 4, 3
	.abicalls
	.option	pic0
	.text
	.align	2
	.globl	dummy
	.set	nomips16
	.ent	dummy
	.type	dummy, @function
dummy:
	.frame	$sp,0,$31		# vars= 0, regs= 0/0, args= 0, gp= 0
	.mask	0x00000000,0
	.fmask	0x00000000,0
 #APP
 # 7 "libpthread/nptl/sysdeps/unix/sysv/linux/gen_lowlevelcond.c" 1
	@@@name@@@cond_lock@@@value@@@0@@@end@@@
 # 0 "" 2
 # 8 "libpthread/nptl/sysdeps/unix/sysv/linux/gen_lowlevelcond.c" 1
	@@@name@@@cond_futex@@@value@@@4@@@end@@@
 # 0 "" 2
 # 9 "libpthread/nptl/sysdeps/unix/sysv/linux/gen_lowlevelcond.c" 1
	@@@name@@@cond_nwaiters@@@value@@@36@@@end@@@
 # 0 "" 2
 # 10 "libpthread/nptl/sysdeps/unix/sysv/linux/gen_lowlevelcond.c" 1
	@@@name@@@total_seq@@@value@@@8@@@end@@@
 # 0 "" 2
 # 11 "libpthread/nptl/sysdeps/unix/sysv/linux/gen_lowlevelcond.c" 1
	@@@name@@@wakeup_seq@@@value@@@16@@@end@@@
 # 0 "" 2
 # 12 "libpthread/nptl/sysdeps/unix/sysv/linux/gen_lowlevelcond.c" 1
	@@@name@@@woken_seq@@@value@@@24@@@end@@@
 # 0 "" 2
 # 13 "libpthread/nptl/sysdeps/unix/sysv/linux/gen_lowlevelcond.c" 1
	@@@name@@@dep_mutex@@@value@@@32@@@end@@@
 # 0 "" 2
 # 14 "libpthread/nptl/sysdeps/unix/sysv/linux/gen_lowlevelcond.c" 1
	@@@name@@@broadcast_seq@@@value@@@40@@@end@@@
 # 0 "" 2
 # 15 "libpthread/nptl/sysdeps/unix/sysv/linux/gen_lowlevelcond.c" 1
	@@@name@@@nwaiters_shift@@@value@@@1@@@end@@@
 # 0 "" 2
 #NO_APP
	j	$31
	.end	dummy
	.size	dummy, .-dummy
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 2001) 4.4.7"
