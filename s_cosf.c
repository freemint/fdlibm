/* sf_cos.c -- float version of s_cos.c.
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

#ifndef __have_fpu_cos

float __ieee754_cosf(float x)
{
	float y[2], z = 0.0;
	int32_t n, ix;

	GET_FLOAT_WORD(ix, x);

	/* |x| ~< pi/4 */
	ix &= IC(0x7fffffff);
	if (ix <= IC(0x3f490fd8))
		return __kernel_cosf(x, z);

	/* cos(Inf or NaN) is NaN */
	else if (!FLT_UWORD_IS_FINITE(ix))
		return x - x;

	/* argument reduction needed */
	else
	{
		n = __ieee754_rem_pio2f(x, y);
		switch ((int)(n & 3))
		{
		case 0:
			return __kernel_cosf(y[0], y[1]);
		case 1:
			return -__kernel_sinf(y[0], y[1], 1);
		case 2:
			return -__kernel_cosf(y[0], y[1]);
		default:
			return __kernel_sinf(y[0], y[1], 1);
		}
	}
}

#endif

float __cosf(float x)
{
	float ret;
	
	ret = __ieee754_cosf(x);
	if (isnan(ret) && !isnan(x))
		ret = __kernel_standard_f(x, x, ret, KMATHERRF_COS_INF);
	return ret;
}

__typeof(__cosf) cosf __attribute__((weak, alias("__cosf")));
