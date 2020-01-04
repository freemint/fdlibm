/* @(#)s_ilogb.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/* ilogb(double x)
 * return the binary exponent of non-zero x
 * ilogb(0) = FP_ILOGB0
 * ilogb(NaN) = FP_ILOGBNAN (no signal is raised)
 * ilogb(+-Inf) = INT_MAX (no signal is raised)
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_ilogb

int __ieee754_ilogb(double x)
{
	int32_t hx, lx, ix;

	GET_HIGH_WORD(hx, x);
	hx &= IC(0x7fffffff);
	if (hx < IC(0x00100000))
	{
		GET_LOW_WORD(lx, x);
		if ((hx | lx) == 0)
			return FP_ILOGB0;			/* ilogb(0) = FP_ILOGB0 */
		/* subnormal x */
		if (hx == 0)
		{
			for (ix = -1043; lx > 0; lx <<= 1)
				ix -= 1;
		} else
		{
			for (ix = -1022, hx <<= 11; hx > 0; hx <<= 1)
				ix -= 1;
		}
		return (int)ix;
	}
	if (hx < IC(0x7ff00000))
		return (int)((hx >> IEEE754_DOUBLE_SHIFT) - IEEE754_DOUBLE_BIAS);
#if FP_ILOGBNAN != INT_MAX
	/* ISO C99 requires ilogb(+-Inf) == INT_MAX.  */
	GET_LOW_WORD(lx, x);
	if (((hx ^ IC(0x7ff00000)) | lx) == 0)
		return INT_MAX;
#endif
	return FP_ILOGBNAN;
}

#endif

/* wrapper ilogb */
int __ilogb(double x)
{
	int r = __ieee754_ilogb(x);

	if (r == FP_ILOGB0 ||
		r == FP_ILOGBNAN ||
		r == INT_MAX)
	{
		__kernel_standard(x, x, x, KMATHERR_ILOGB);
	}
	return r;
}

__typeof(__ilogb) ilogb __attribute__((weak, alias("__ilogb")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__ilogbl) __ilogbl __attribute__((alias("__ilogb")));
__typeof(__ilogbl) ilogbl __attribute__((weak, alias("__ilogb")));
#endif
