/* s_logbf.c -- float version of s_logb.c.
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

float __logbf(float x)
{
	int32_t ix, rix;

	GET_FLOAT_WORD(ix, x);
	ix &= IC(0x7fffffff);					/* high |x| */
	if (ix == 0)
		return (float) -1.0 / __ieee754_fabsf(x);
	if (ix >= IC(0x7f800000))
		return x * x;
	if ((rix = ix >> IEEE754_FLOAT_SHIFT) == 0)
	{
		/* POSIX specifies that denormal number is treated as
		   though it were normalized.  */
		rix -= count_leading_zeros(ix) - 9;
	}
	return (float) (rix - IEEE754_FLOAT_BIAS);
}

__typeof(__logbf) logbf __attribute__((weak, alias("__logbf")));
