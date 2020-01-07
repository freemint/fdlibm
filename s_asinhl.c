/* s_asinhl.c -- long double version of s_asinh.c.
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

/* asinhl(x)
 * Method :
 *	Based on
 *		asinhl(x) = signl(x) * logl [ |x| + sqrtl(x*x+1) ]
 *	we have
 *	asinhl(x) := x  if  1+x*x=1,
 *		  := signl(x)*(logl(x)+ln2)) for large |x|, else
 *		  := signl(x)*logl(2|x|+1/(|x|+sqrtl(x*x+1))) if|x|>2, else
 *		  := signl(x)*log1pl(|x| + x^2/(1 + sqrtl(1+x^2)))
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_asinh

long double __ieee754_asinhl(long double x)
{
	long double t, w;
	int32_t hx, ix;

	static const long double one = 1.000000000000000000000e+00L;	/* 0x3FFF, 0x00000000, 0x00000000 */
	static const long double ln2 = 6.931471805599453094287e-01L;	/* 0x3FFE, 0xB17217F7, 0xD1CF79AC */
	static const long double hugeval = 1.000000000000000000e+4900L;

	GET_LDOUBLE_EXP(hx, x);
	ix = hx & IEEE854_LONG_DOUBLE_MAXEXP;
	if (ix < 0x3fde)
	{									/* |x|<2**-34 */
		if (hugeval + x > one)
			return x;					/* return x inexact except 0 */
	}
	if (ix > 0x4020)
	{									/* |x| > 2**34 */
		if (ix == IEEE854_LONG_DOUBLE_MAXEXP)
			return x + x;				/* x is inf or NaN */
		w = __ieee754_logl(__ieee754_fabsl(x)) + ln2;
	} else
	{
		long double xa = __ieee754_fabsl(x);

		if (ix > 0x4000)
		{								/* 2**34 > |x| > 2.0 */
			w = __ieee754_logl(2.0L * xa + one / (__ieee754_sqrtl(xa * xa + one) + xa));
		} else
		{								/* 2.0 > |x| > 2**-28 */
			t = xa * xa;
			w = __ieee754_log1pl(xa + t / (one + __ieee754_sqrtl(one + t)));
		}
	}
	if (hx > 0)
		return w;
	return -w;
}

#endif

long double __asinhl(long double x)
{
	return __ieee754_asinhl(x);
}

__typeof(__asinhl) asinhl __attribute__((weak, alias("__asinhl")));

#endif
