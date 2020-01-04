/* @(#)s_significand.c 1.3 95/01/18 */
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

/*
 * significand(x) computes just
 * 	scalb(x, (double) -ilogb(x)),
 * for exercising the fraction-part(F) IEEE 754-1985 test vector.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

double __significand(double x)
{
	uint32_t hx, ix;

	GET_HIGH_WORD(hx, x);
	ix = hx & UC(0x7ff00000);
	if (ix != 0 && ix != UC(0x7ff00000))
	{
		SET_HIGH_WORD(x, (hx & UC(0x800fffff)) | UC(0x3ff00000));
	}
	return x;
}

__typeof(__significand) significand __attribute__((weak, alias("__significand")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__significandl) __significandl __attribute__((alias("__significand")));
__typeof(__significandl) significandl __attribute__((weak, alias("__significand")));
#endif
