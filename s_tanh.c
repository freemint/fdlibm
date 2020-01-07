/* @(#)s_tanh.c 1.3 95/01/18 */
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

/* Tanh(x)
 * Return the Hyperbolic Tangent of x
 *
 * Method :
 *				       x    -x
 *				      e  - e
 *	0. tanh(x) is defined to be -----------
 *				       x    -x
 *				      e  + e
 *	1. reduce x to non-negative by tanh(-x) = -tanh(x).
 *	2.  0      <= x <= 2**-55 : tanh(x) := x*(one+x)
 *					        -t
 *	    2**-55 <  x <=  1     : tanh(x) := -----; t = expm1(-2x)
 *					       t + 2
 *						     2
 *	    1      <= x <=  22.0  : tanh(x) := 1-  ----- ; t=expm1(2x)
 *						   t + 2
 *	    22.0   <  x <= INF    : tanh(x) := 1.
 *
 * Special cases:
 *	tanh(NaN) is NaN;
 *	only tanh(0)=0 is exact for finite argument.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_tanh

double __ieee754_tanh(double x)
{
	double t, z;
	int32_t jx, ix, lx;

	static const double one = 1.0;
	static const double two = 2.0;
	static const double tiny = 1.0e-300;

	/* High word of |x|. */
	GET_DOUBLE_WORDS(jx, lx, x);
	ix = jx & IC(0x7fffffff);

	/* x is INF or NaN */
	if (ix >= IC(0x7ff00000))
	{
		if (jx >= 0)
			return one / x + one;		/* tanh(+-inf)=+-1 */
		else
			return one / x - one;		/* tanh(NaN) = NaN */
	}

	/* |x| < 22 */
	if (ix < IC(0x40360000))
	{									/* |x|<22 */
		if ((ix | lx) == 0)
			return x;					/* x == +-0 */
		if (ix < IC(0x3c800000))		/* |x|<2**-55 */
			return x * (one + x);		/* tanh(small) = small */
		if (ix >= IC(0x3ff00000))
		{								/* |x|>=1  */
			t = __ieee754_expm1(two * __ieee754_fabs(x));
			z = one - two / (t + two);
		} else
		{
			t = __ieee754_expm1(-two * __ieee754_fabs(x));
			z = -t / (t + two);
		}
		/* |x| > 22, return +-1 */
	} else
	{
		z = one - tiny;					/* raised inexact flag */
	}
	return (jx >= 0) ? z : -z;
}

#endif

double __tanh(double x)
{
	return __ieee754_tanh(x);
}

__typeof(__tanh) tanh __attribute__((weak, alias("__tanh")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__tanhl) __tanhl __attribute__((alias("__tanh")));
__typeof(__tanhl) tanhl __attribute__((weak, alias("__tanh")));
#endif
