/* s_tanf.c -- float version of s_tan.c.
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

#ifndef __have_fpu_tan

float __ieee754_tanf(float x)
{
	float y[2], z = 0.0;
	int32_t n, ix;

	GET_FLOAT_WORD(ix, x);

	/* |x| ~< pi/4 */
	ix &= IC(0x7fffffff);
	if (ix <= IC(0x3f490fda))
		return __kernel_tanf(x, z, 1);

	/* tan(Inf or NaN) is NaN */
	else if (!FLT_UWORD_IS_FINITE(ix))
		return x - x;					/* NaN */

	/* argument reduction needed */
	else
	{
		n = __ieee754_rem_pio2f(x, y);
		return __kernel_tanf(y[0], y[1], (int)(1 - ((n & 1) << 1)));	/*   1 -- n even -1 -- n odd */
	}
}

#endif

float __tanf(float x)
{
	float ret;
	
	ret = __ieee754_tan(x);
	if (isnan(ret) && isinf(x))
		ret = __kernel_standard_f(x, x, ret, KMATHERRF_TAN_INF);
	return ret;
}

__typeof(__tanf) tanf __attribute__((weak, alias("__tanf")));
