	.file	1 "ucontext_i.c"
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
 # 8 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@SIG_BLOCK@@@value@@@1@@@end@@@
 # 0 "" 2
 # 9 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@SIG_SETMASK@@@value@@@3@@@end@@@
 # 0 "" 2
 # 10 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@_NSIG8@@@value@@@16@@@end@@@
 # 0 "" 2
 # 12 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@RT_SIGFRAME_UCONTEXT@@@value@@@152@@@end@@@
 # 0 "" 2
 # 13 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@RT_SIGFRAME_SIZE@@@value@@@1024@@@end@@@
 # 0 "" 2
 # 17 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@UCONTEXT_FLAGS@@@value@@@0@@@end@@@
 # 0 "" 2
 # 18 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@UCONTEXT_LINK@@@value@@@4@@@end@@@
 # 0 "" 2
 # 19 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@UCONTEXT_STACK@@@value@@@8@@@end@@@
 # 0 "" 2
 # 20 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@UCONTEXT_MCONTEXT@@@value@@@24@@@end@@@
 # 0 "" 2
 # 21 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@UCONTEXT_SIGMASK@@@value@@@616@@@end@@@
 # 0 "" 2
 # 22 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@STACK_SP@@@value@@@8@@@end@@@
 # 0 "" 2
 # 23 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@STACK_SIZE@@@value@@@12@@@end@@@
 # 0 "" 2
 # 24 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@STACK_FLAGS@@@value@@@16@@@end@@@
 # 0 "" 2
 # 25 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@MCONTEXT_GREGS@@@value@@@40@@@end@@@
 # 0 "" 2
 # 26 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@MCONTEXT_FPREGS@@@value@@@296@@@end@@@
 # 0 "" 2
 # 27 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@MCONTEXT_MDHI@@@value@@@576@@@end@@@
 # 0 "" 2
 # 28 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@MCONTEXT_HI1@@@value@@@592@@@end@@@
 # 0 "" 2
 # 29 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@MCONTEXT_HI2@@@value@@@600@@@end@@@
 # 0 "" 2
 # 30 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@MCONTEXT_HI3@@@value@@@608@@@end@@@
 # 0 "" 2
 # 31 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@MCONTEXT_MDLO@@@value@@@584@@@end@@@
 # 0 "" 2
 # 32 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@MCONTEXT_LO1@@@value@@@596@@@end@@@
 # 0 "" 2
 # 33 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@MCONTEXT_LO2@@@value@@@604@@@end@@@
 # 0 "" 2
 # 34 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@MCONTEXT_LO3@@@value@@@612@@@end@@@
 # 0 "" 2
 # 35 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@MCONTEXT_PC@@@value@@@32@@@end@@@
 # 0 "" 2
 # 36 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@MCONTEXT_FPC_CSR@@@value@@@556@@@end@@@
 # 0 "" 2
 # 37 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@MCONTEXT_USED_MATH@@@value@@@564@@@end@@@
 # 0 "" 2
 # 38 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@MCONTEXT_DSP@@@value@@@568@@@end@@@
 # 0 "" 2
 # 39 "libc/sysdeps/linux/mips/ucontext_i.c" 1
	@@@name@@@UCONTEXT_SIZE@@@value@@@632@@@end@@@
 # 0 "" 2
 #NO_APP
	j	$31
	.end	dummy
	.size	dummy, .-dummy
	.ident	"GCC: (Realtek MSDK-4.4.7 Build 2001) 4.4.7"
