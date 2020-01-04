/* s_ilogbl.c -- long double version of s_ilogb.c.
 * Conversion to long double by Ulrich Drepper,
 * Cygnus Support, drepper@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/* ilogbl(long double x)
 * return the binary exponent of non-zero x
 * ilogbl(0) = FP_ILOGB0
 * ilogbl(NaN) = FP_ILOGBNAN (no signal is raised)
 * ilogbl(+-Inf) = INT_MAX (no signal is raised)
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_ilogb

int __ieee754_ilogbl(long double x)
{
	int32_t es, hx, lx, ix;

	GET_LDOUBLE_EXP(es, x);
	es &= 0x7fff;
	if (es == 0)
	{
		GET_LDOUBLE_WORDS(es, hx, lx, x);
		hx &= IC(0x7fffffff);
		if ((hx | lx) == 0)
			return FP_ILOGB0;			/* ilogbl(0) = FP_ILOGB0 */
		/* subnormal x */
		if (hx == 0)
		{
			for (ix = -16414; lx > 0; lx <<= 1)
				ix -= 1;
		} else
		{
			for (ix = -16383, hx <<= 1; hx > 0; hx <<= 1)
				ix -= 1;
		}
		return (int)ix;
	}
	if (es < 0x7fff)
		return (int)es - 0x3fff;
#if FP_ILOGBNAN != INT_MAX
	GET_LDOUBLE_WORDS(es, hx, lx, x);
	if (((hx & IC(0x7fffffff)) | lx) == 0)
		/* ISO C99 requires ilogbl(+-Inf) == INT_MAX.  */
		return INT_MAX;
#endif
	return FP_ILOGBNAN;
}

#endif

/* wrapper ilogbl */
int __ilogbl(long double x)
{
	int r = __ieee754_ilogbl(x);

	if (r == FP_ILOGB0 ||
		r == FP_ILOGBNAN ||
		r == INT_MAX)
	{
		__kernel_standard_l(x, x, x, KMATHERRL_ILOGB);
	}
	return r;
}

__typeof(__ilogbl) ilogbl __attribute__((weak, alias("__ilogbl")));

#endif
