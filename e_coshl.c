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

/* __ieee754_coshl(x)
 * Method :
 * mathematically coshl(x) if defined to be (exp(x)+exp(-x))/2
 *	1. Replace x by |x| (coshl(x) = coshl(-x)).
 *	2.
 *							[ exp(x) - 1 ]^2
 *	    0        <= x <= ln2/2  :  coshl(x) := 1 + -------------------
 *							   2*exp(x)
 *
 *						   exp(x) +  1/exp(x)
 *	    ln2/2    <= x <= 22     :  coshl(x) := -------------------
 *							   2
 *	    22       <= x <= lnovft :  coshl(x) := expl(x)/2
 *	    lnovft   <= x <= ln2ovft:  coshl(x) := expl(x/2)/2 * expl(x/2)
 *	    ln2ovft  <  x	    :  coshl(x) := huge*huge (overflow)
 *
 * Special cases:
 *	coshl(x) is |x| if x is +INF, -INF, or NaN.
 *	only coshl(0)=1 is exact for finite x.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_cosh

long double __ieee754_coshl(long double x)
{
	long double t, w;
	int32_t ex;
	uint32_t mx, lx;

	static const long double one = 1.0;
	static const long double half = 0.5;
	static const long double hugeval = 1.0e4900L;
	
	/* High word of |x|. */
	GET_LDOUBLE_WORDS(ex, mx, lx, x);
	ex &= 0x7fff;

	/* x is INF or NaN */
	if (ex == 0x7fff)
		return x * x;

	/* |x| in [0,22] */
	if (ex < 0x4003 || (ex == 0x4003 && mx < UC(0xb0000000)))
	{
		/* |x| in [0,0.5*ln2], return 1+expm1l(|x|)^2/(2*expl(|x|)) */
		if (ex < 0x3ffd || (ex == 0x3ffd && mx < UC(0xb17217f7)))
		{
			t = __ieee754_expm1l(__ieee754_fabsl(x));
			w = one + t;
			if (ex < 0x3fbc)
				return w;				/* cosh(tiny) = 1 */
			return one + (t * t) / (w + w);
		}

		/* |x| in [0.5*ln2,22], return (exp(|x|)+1/exp(|x|)/2; */
		t = __ieee754_expl(__ieee754_fabsl(x));
		return half * t + half / t;
	}

	/* |x| in [22, ln(maxdouble)] return half*exp(|x|) */
	if (ex < 0x400c || (ex == 0x400c && mx < UC(0xb1700000)))
		return half * __ieee754_expl(__ieee754_fabsl(x));

	/* |x| in [log(maxdouble), log(2*maxdouble)) */
	if (ex == 0x400c && (mx < UC(0xb174ddc0) || (mx == UC(0xb174ddc0) && lx < UC(0x31aec0eb))))
	{
		w = __ieee754_expl(half * __ieee754_fabsl(x));
		t = half * w;
		return t * w;
	}

	/* |x| >= log(2*maxdouble), cosh(x) overflow */
	return hugeval * hugeval;
}

#endif

long double __coshl(long double x)
{
	long double z = __ieee754_coshl(x);

	if (!isfinite(z) && isfinite(x) && _LIB_VERSION != _IEEE_)
		return __kernel_standard_l(x, x, z, KMATHERRL_COSH);	/* cosh overflow */

	return z;
}

__typeof(__coshl) coshl __attribute__((weak, alias("__coshl")));

#endif
