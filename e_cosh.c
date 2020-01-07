/* @(#)e_cosh.c 1.3 95/01/18 */
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
/* Optimized by Ulrich Drepper <drepper@gmail.com>, 2011 */

/* __ieee754_cosh(x)
 * Method :
 * mathematically cosh(x) if defined to be (exp(x)+exp(-x))/2
 *	1. Replace x by |x| (cosh(x) = cosh(-x)).
 *	2.
 *							[ exp(x) - 1 ]^2
 *	    0        <= x <= ln2/2  :  cosh(x) := 1 + -------------------
 *							   2*exp(x)
 *
 *						  exp(x) +  1/exp(x)
 *	    ln2/2    <= x <= 22     :  cosh(x) := -------------------
 *							  2
 *	    22       <= x <= lnovft :  cosh(x) := exp(x)/2
 *	    lnovft   <= x <= ln2ovft:  cosh(x) := exp(x/2)/2 * exp(x/2)
 *	    ln2ovft  <  x	    :  cosh(x) := hugeval*hugeval (overflow)
 *
 * Special cases:
 *	cosh(x) is |x| if x is +INF, -INF, or NaN.
 *	only cosh(0)=1 is exact for finite x.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_cosh

double __ieee754_cosh(double x)
{
	double t, w;
	int32_t ix;
	uint32_t lx;

	static const double one = 1.0;
	static const double half = 0.5;
	static const double hugeval = 1.0e300;
	
	/* High word of |x|. */
	GET_HIGH_WORD(ix, x);
	ix &= IC(0x7fffffff);

	/* x is INF or NaN */
	if (ix >= IC(0x7ff00000))
		return x * x;

	/* |x| in [0,22] */
	if (ix < IC(0x40360000))
	{
		/* |x| in [0,0.5*ln2], return 1+expm1(|x|)^2/(2*exp(|x|)) */
		if (ix < IC(0x3fd62e43))
		{
			t = __ieee754_expm1(__ieee754_fabs(x));
			w = one + t;
			if (ix < IC(0x3c800000))
				return w;				/* cosh(tiny) = 1 */
			return one + (t * t) / (w + w);
		}

		/* |x| in [0.5*ln2,22], return (exp(|x|)+1/exp(|x|)/2; */
		t = __ieee754_exp(__ieee754_fabs(x));
		return half * t + half / t;
	}

	/* |x| in [22, log(maxdouble)] return half*exp(|x|) */
	if (ix < IC(0x40862E42))
		return half * __ieee754_exp(__ieee754_fabs(x));

	/* |x| in [log(maxdouble), overflowthresold] */
	GET_LOW_WORD(lx, x);
	if (ix < IC(0x408633ce) || (ix == IC(0x408633ce) && lx <= UC(0x8fb9f87d)))
	{
		w = __ieee754_exp(half * __ieee754_fabs(x));
		t = half * w;
		return t * w;
	}

	/* |x| > overflowthresold, cosh(x) overflow */
	return hugeval * hugeval;
}

#endif

/* wrapper cosh */
double __cosh(double x)
{
	double z = __ieee754_cosh(x);

	if (_LIB_VERSION != _IEEE_ && !isfinite(z) && isfinite(x))
		return __kernel_standard(x, x, z, KMATHERR_COSH);	/* cosh overflow */

	return z;
}

__typeof(__cosh) cosh __attribute__((weak, alias("__cosh")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__coshl) __coshl __attribute__((alias("__cosh")));
__typeof(__coshl) coshl __attribute__((weak, alias("__cosh")));
#endif
