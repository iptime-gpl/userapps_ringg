/*
 * Realtek Semiconductor Corp.
 *
 * arch/mips/kernel/gcmp.c
 *     GCMP initialization code
 *
 * Copyright (C) 2006-2012 Tony Wu (tonywu@realtek.com)
 */

#include <linux/version.h>
#include <linux/smp.h>
#include <linux/interrupt.h>

#include <asm/gic.h>
#include <asm/gcmpregs.h>

#ifdef CONFIG_MIPS_CMP

unsigned long gcmp_base;
int gcmp_present = -1;

/* Return the number of IOCU's present */
int __init gcmp_niocu(void)
{
	return gcmp_present ?
	    (GCMPGCB(GC) & GCMP_GCB_GC_NUMIOCU_MSK) >> GCMP_GCB_GC_NUMIOCU_SHF : 0;
}

/* Set GCMP region attributes */
void __init gcmp_setregion(int region, unsigned long base,
			   unsigned long mask, int type)
{
	if (gcmp_present <= 0) {
		pr_debug("GCMP not present\n");
		return;
	}

	/*
	 * workaround 1074K errata E25:
	 * set CCMC syncctl bit to 1 for memory sync
	 */
	pr_debug("Set SYNCCTL to memory sync\n");
	GCMPGCB(CCMC) |= 1 << GCMP_GCB_CCMC_SYNCCTL_SHF;
	GCMPGCBn(CMxBASE, region) = base;
	GCMPGCBn(CMxMASK, region) = mask | type;
}

/*
 * GCMP needs to be detected before any SMP initialisation
 */
int __init gcmp_probe(unsigned long base, unsigned long size)
{
	if (gcmp_present >= 0)
		return gcmp_present;

	gcmp_base = (unsigned long) ioremap_nocache(base, size);
	gcmp_present = (GCMPGCB(GCMPB) & GCMP_GCB_GCMPB_GCMPBASE_MSK) == base;
	if (gcmp_present)
		pr_debug("GCMP present\n");

	return gcmp_present;
}
#endif
