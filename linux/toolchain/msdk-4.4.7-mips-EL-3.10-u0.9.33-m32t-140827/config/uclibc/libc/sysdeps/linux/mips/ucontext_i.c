#include <inttypes.h>
#include <signal.h>
#include <stddef.h>
#include <sys/ucontext.h>
#include <kernel_rt_sigframe.h>
void dummy(void);
void dummy(void) {
__asm__ ("@@@name@@@SIG_BLOCK@@@value@@@%0@@@end@@@" : : "i" ((long) SIG_BLOCK));
__asm__ ("@@@name@@@SIG_SETMASK@@@value@@@%0@@@end@@@" : : "i" ((long) SIG_SETMASK));
__asm__ ("@@@name@@@_NSIG8@@@value@@@%0@@@end@@@" : : "i" ((long) (_NSIG / 8)));
#define rt_sigframe(member)	offsetof (kernel_rt_sigframe_t, member)
__asm__ ("@@@name@@@RT_SIGFRAME_UCONTEXT@@@value@@@%0@@@end@@@" : : "i" ((long) rt_sigframe (rs_uc)));
__asm__ ("@@@name@@@RT_SIGFRAME_SIZE@@@value@@@%0@@@end@@@" : : "i" ((long) sizeof (kernel_rt_sigframe_t)));
#define ucontext(member)	offsetof (ucontext_t, member)
#define stack(member)		ucontext (uc_stack.member)
#define mcontext(member)	ucontext (uc_mcontext.member)
__asm__ ("@@@name@@@UCONTEXT_FLAGS@@@value@@@%0@@@end@@@" : : "i" ((long) ucontext (uc_flags)));
__asm__ ("@@@name@@@UCONTEXT_LINK@@@value@@@%0@@@end@@@" : : "i" ((long) ucontext (uc_link)));
__asm__ ("@@@name@@@UCONTEXT_STACK@@@value@@@%0@@@end@@@" : : "i" ((long) ucontext (uc_stack)));
__asm__ ("@@@name@@@UCONTEXT_MCONTEXT@@@value@@@%0@@@end@@@" : : "i" ((long) ucontext (uc_mcontext)));
__asm__ ("@@@name@@@UCONTEXT_SIGMASK@@@value@@@%0@@@end@@@" : : "i" ((long) ucontext (uc_sigmask)));
__asm__ ("@@@name@@@STACK_SP@@@value@@@%0@@@end@@@" : : "i" ((long) stack (ss_sp)));
__asm__ ("@@@name@@@STACK_SIZE@@@value@@@%0@@@end@@@" : : "i" ((long) stack (ss_size)));
__asm__ ("@@@name@@@STACK_FLAGS@@@value@@@%0@@@end@@@" : : "i" ((long) stack (ss_flags)));
__asm__ ("@@@name@@@MCONTEXT_GREGS@@@value@@@%0@@@end@@@" : : "i" ((long) mcontext (gregs)));
__asm__ ("@@@name@@@MCONTEXT_FPREGS@@@value@@@%0@@@end@@@" : : "i" ((long) mcontext (fpregs)));
__asm__ ("@@@name@@@MCONTEXT_MDHI@@@value@@@%0@@@end@@@" : : "i" ((long) mcontext (mdhi)));
__asm__ ("@@@name@@@MCONTEXT_HI1@@@value@@@%0@@@end@@@" : : "i" ((long) mcontext (hi1)));
__asm__ ("@@@name@@@MCONTEXT_HI2@@@value@@@%0@@@end@@@" : : "i" ((long) mcontext (hi2)));
__asm__ ("@@@name@@@MCONTEXT_HI3@@@value@@@%0@@@end@@@" : : "i" ((long) mcontext (hi3)));
__asm__ ("@@@name@@@MCONTEXT_MDLO@@@value@@@%0@@@end@@@" : : "i" ((long) mcontext (mdlo)));
__asm__ ("@@@name@@@MCONTEXT_LO1@@@value@@@%0@@@end@@@" : : "i" ((long) mcontext (lo1)));
__asm__ ("@@@name@@@MCONTEXT_LO2@@@value@@@%0@@@end@@@" : : "i" ((long) mcontext (lo2)));
__asm__ ("@@@name@@@MCONTEXT_LO3@@@value@@@%0@@@end@@@" : : "i" ((long) mcontext (lo3)));
__asm__ ("@@@name@@@MCONTEXT_PC@@@value@@@%0@@@end@@@" : : "i" ((long) mcontext (pc)));
__asm__ ("@@@name@@@MCONTEXT_FPC_CSR@@@value@@@%0@@@end@@@" : : "i" ((long) mcontext (fpc_csr)));
__asm__ ("@@@name@@@MCONTEXT_USED_MATH@@@value@@@%0@@@end@@@" : : "i" ((long) mcontext (used_math)));
__asm__ ("@@@name@@@MCONTEXT_DSP@@@value@@@%0@@@end@@@" : : "i" ((long) mcontext (dsp)));
__asm__ ("@@@name@@@UCONTEXT_SIZE@@@value@@@%0@@@end@@@" : : "i" ((long) sizeof (ucontext_t)));
}
