/* e_asinhl.c -- long double version of e_asinh.c.
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

/* __ieee754_sinhl(x)
 * Method :
 * mathematically sinh(x) if defined to be (exp(x)-exp(-x))/2
 *	1. Replace x by |x| (sinhl(-x) = -sinhl(x)).
 *	2.
 *						     E + E/(E+1)
 *	    0        <= x <= 25     :  sinhl(x) := --------------, E=expm1l(x)
 *							 2
 *
 *	    25       <= x <= lnovft :  sinhl(x) := expl(x)/2
 *	    lnovft   <= x <= ln2ovft:  sinhl(x) := expl(x/2)/2 * expl(x/2)
 *	    ln2ovft  <  x	    :  sinhl(x) := x*shuge (overflow)
 *
 * Special cases:
 *	sinhl(x) is |x| if x is +INF, -INF, or NaN.
 *	only sinhl(0)=0 is exact for finite x.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_sinh

long double __ieee754_sinhl(long double x)
{
	long double t, w, h;
	uint32_t jx, ix, i0, i1;

	static const long double one = 1.0;
	static const long double shuge = 1.0e4931L;
	
	/* Words of |x|. */
	GET_LDOUBLE_WORDS(jx, i0, i1, x);
	ix = jx & IEEE854_LONG_DOUBLE_MAXEXP;

	/* x is INF or NaN */
	if (ix == IEEE854_LONG_DOUBLE_MAXEXP)
		return x + x;

	h = 0.5;
	if (jx & 0x8000)
		h = -h;
	/* |x| in [0,25], return sign(x)*0.5*(E+E/(E+1))) */
	if (ix < 0x4003 || (ix == 0x4003 && i0 <= UC(0xc8000000)))
	{									/* |x|<25 */
		if (ix < 0x3fdf)				/* |x|<2**-32 */
			if (shuge + x > one)
				return x;				/* sinh(tiny) = tiny with inexact */
		t = __ieee754_expm1l(__ieee754_fabsl(x));
		if (ix < 0x3fff)
			return h * (2.0 * t - t * t / (t + one));
		return h * (t + t / (t + one));
	}

	/* |x| in [25, log(maxdouble)] return 0.5*exp(|x|) */
	if (ix < 0x400c || (ix == 0x400c && i0 < UC(0xb17217f7)))
		return h * __ieee754_expl(__ieee754_fabsl(x));

	/* |x| in [log(maxdouble), overflowthreshold] */
	if (ix < 0x400c || (ix == 0x400c && (i0 < UC(0xb174ddc0) || (i0 == UC(0xb174ddc0) && i1 <= UC(0x31aec0ea)))))
	{
		w = __ieee754_expl(0.5 * __ieee754_fabsl(x));
		t = h * w;
		return t * w;
	}

	/* |x| > overflowthreshold, sinhl(x) overflow */
	return x * shuge;
}

#endif

long double __sinhl(long double  x)
{
	long double z = __ieee754_sinhl(x);

	if (!isfinite(z) && isfinite(x) && _LIB_VERSION != _IEEE_)
		return __kernel_standard_l(x, x, z, KMATHERRL_SINH);	/* sinh overflow */

	return z;
}

__typeof(__sinhl) sinhl __attribute__((weak, alias("__sinhl")));

#endif
