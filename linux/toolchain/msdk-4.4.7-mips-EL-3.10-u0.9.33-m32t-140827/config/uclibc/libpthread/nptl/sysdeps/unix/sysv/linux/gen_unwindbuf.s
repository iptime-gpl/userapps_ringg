	.file	1 "gen_unwindbuf.c"
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
 # 5 "libpthread/nptl/sysdeps/unix/sysv/linux/gen_unwindbuf.c" 1
	@@@name@@@UNWINDBUFSIZE@@@value@@@128@@@end@@@
 # 0 "" 2
 # 6 "libpthread/nptl/sysdeps/unix/sysv/linux/gen_unwindbuf.c" 1
	@@@name@@@UWJMPBUF@@@value@@@0@@@end@@@
 # 0 "" 2
 #NO_APP
	j	$31
	.end	dummy
	.size	dummy, .-dummy
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 2001) 4.4.7"
