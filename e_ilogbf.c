/* s_ilogbf.c -- float version of s_ilogb.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_ilogb

int __ieee754_ilogbf(float x)
{
	int32_t hx, ix;

	GET_FLOAT_WORD(hx, x);
	hx &= IC(0x7fffffff);
	if (hx < IC(0x00800000))
	{
		if (hx == 0)
			return FP_ILOGB0;			/* ilogb(0) = FP_ILOGB0 */
		/* subnormal x */
		for (ix = -126, hx <<= 8; hx > 0; hx <<= 1)
			ix -= 1;
		return (int)ix;
	}
	if (hx < IC(0x7f800000))
		return (int)((hx >> IEEE754_FLOAT_SHIFT) - IEEE754_FLOAT_BIAS);
#if FP_ILOGBNAN != INT_MAX
	/* ISO C99 requires ilogbf(+-Inf) == INT_MAX.  */
	if (hx == IC(0x7f800000))
		return INT_MAX;
#endif
	return FP_ILOGBNAN;
}

#endif

/* wrapper ilogbf */
int __ilogbf(float x)
{
	int r = __ieee754_ilogbf(x);

	if (r == FP_ILOGB0 || r == FP_ILOGBNAN || r == INT_MAX)
	{
		__kernel_standard_f(x, x, x, KMATHERRF_ILOGB);
	}
	return r;
}

__typeof(__ilogbf) ilogbf __attribute__((weak, alias("__ilogbf")));
