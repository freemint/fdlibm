/* s_atanhl.c -- long double version of s_atan.c.
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

/* __ieee754_atanhl(x)
 * Method :
 *    1.Reduced x to positive by atanh(-x) = -atanh(x)
 *    2.For x>=0.5
 *                   1              2x                          x
 *	atanhl(x) = --- * log(1 + -------) = 0.5 * log1p(2 * --------)
 *                   2             1 - x                      1 - x
 *
 *	For x<0.5
 *	atanhl(x) = 0.5*log1pl(2x+2x*x/(1-x))
 *
 * Special cases:
 *	atanhl(x) is NaN if |x| > 1 with signal;
 *	atanhl(NaN) is that NaN with no signal;
 *	atanhl(+-1) is +-INF with signal.
 *
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_atanh

long double __ieee754_atanhl(long double x)
{
	long double t;
	int32_t ix;
	uint32_t i0, i1;
	int32_t se;
	
	static const long double one = 1.0;
	static const long double hugeval = 1e4900L;
	static const long double zero = 0.0;

	GET_LDOUBLE_WORDS(se, i0, i1, x);
	ix = se & 0x7fff;
	if ((ix + ((((i0 & IC(0x7fffffff)) | i1) | (-((i0 & IC(0x7fffffff)) | i1))) >> 31)) > 0x3fff)
		/* |x|>1 */
		return (x - x) / (x - x);
	if (ix == 0x3fff)
		return x / zero;
	if (ix < 0x3fe3)
	{
		math_force_eval(hugeval + x);
		return x;						/* x<2**-28 */
	}
	SET_LDOUBLE_EXP(x, ix);
	if (ix < 0x3ffe)
	{									/* x < 0.5 */
		t = x + x;
		t = 0.5 * __ieee754_log1pl(t + t * x / (one - x));
	} else
	{
		t = 0.5 * __ieee754_log1pl((x + x) / (one - x));
	}
	if (se < 0)
		t = -t;
	return t;
}

#endif

/* wrapper atanh */
long double __atanhl(long double x)
{
	if (isgreaterequal(__ieee754_fabsl(x), 1.0) && _LIB_VERSION != _IEEE_)
		return __kernel_standard_l(x, x, HUGE_VALL, __ieee754_fabsl(x) > 1.0 ? KMATHERRL_ATANH_PLUSONE	/* atanh(|x|>1) */
								 : KMATHERRL_ATANH_ONE);	/* atanh(|x|==1) */

	return __ieee754_atanhl(x);
}

__typeof(__atanhl) atanhl __attribute__((weak, alias("__atanhl")));

#endif
