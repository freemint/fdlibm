/* @(#)e_sinh.c 1.3 95/01/18 */
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

/* __ieee754_sinh(x)
 * Method :
 * mathematically sinh(x) if defined to be (exp(x)-exp(-x))/2
 *	1. Replace x by |x| (sinh(-x) = -sinh(x)).
 *	2.
 *						    E + E/(E+1)
 *	    0        <= x <= 22     :  sinh(x) := --------------, E=expm1(x)
 *							2
 *
 *	    22       <= x <= lnovft :  sinh(x) := exp(x)/2
 *	    lnovft   <= x <= ln2ovft:  sinh(x) := exp(x/2)/2 * exp(x/2)
 *	    ln2ovft  <  x	    :  sinh(x) := x*shuge (overflow)
 *
 * Special cases:
 *	sinh(x) is |x| if x is +INF, -INF, or NaN.
 *	only sinh(0)=0 is exact for finite x.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_sinh

double __ieee754_sinh(double x)
{
	double t, w, h;
	int32_t ix, jx;
	uint32_t lx;

	static const double one = 1.0;
	static const double shuge = 1.0e307;

	/* High word of |x|. */
	GET_HIGH_WORD(jx, x);
	ix = jx & IC(0x7fffffff);

	/* x is INF or NaN */
	if (ix >= IC(0x7ff00000))
		return x + x;

	h = 0.5;
	if (jx < 0)
		h = -h;
	/* |x| in [0,22], return sign(x)*0.5*(E+E/(E+1))) */
	if (ix < IC(0x40360000))
	{									/* |x|<22 */
		if (ix < IC(0x3e300000))		/* |x|<2**-28 */
			if (shuge + x > one)
				return x;				/* sinh(tiny) = tiny with inexact */
		t = __ieee754_expm1(__ieee754_fabs(x));
		if (ix < IC(0x3ff00000))
			return h * (2.0 * t - t * t / (t + one));
		return h * (t + t / (t + one));
	}

	/* |x| in [22, log(maxdouble)] return 0.5*exp(|x|) */
	if (ix < IC(0x40862e42))
		return h * __ieee754_exp(__ieee754_fabs(x));

	/* |x| in [log(maxdouble), overflowthresold] */
	GET_LOW_WORD(lx, x);
	if (ix < IC(0x408633ce) || (ix == IC(0x408633ce) && lx <= UC(0x8fb9f87d)))
	{
		w = __ieee754_exp(0.5 * __ieee754_fabs(x));
		t = h * w;
		return t * w;
	}

	/* |x| > overflowthresold, sinh(x) overflow */
	return x * shuge;
}

#endif

/* wrapper sinh */
double __sinh(double x)
{
	double z = __ieee754_sinh(x);

	if (_LIB_VERSION != _IEEE_ && !isfinite(z) && isfinite(x))
		return __kernel_standard(x, x, z, KMATHERR_SINH);	/* sinh overflow */

	return z;
}

__typeof(__sinh) sinh __attribute__((weak, alias("__sinh")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__sinhl) __sinhl __attribute__((alias("__sinh")));
__typeof(__sinhl) sinhl __attribute__((weak, alias("__sinh")));
#endif
