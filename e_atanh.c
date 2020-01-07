/* @(#)e_atanh.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 *
 */

/* __ieee754_atanh(x)
 * Method :
 *    1.Reduced x to positive by atanh(-x) = -atanh(x)
 *    2.For x>=0.5
 *                  1              2x                          x
 *	atanh(x) = --- * log(1 + -------) = 0.5 * log1p(2 * --------)
 *                  2             1 - x                      1 - x
 *	
 * 	For x<0.5
 *	atanh(x) = 0.5*log1p(2x+2x*x/(1-x))
 *
 * Special cases:
 *	atanh(x) is NaN if |x| > 1 with signal;
 *	atanh(NaN) is that NaN with no signal;
 *	atanh(+-1) is +-INF with signal.
 *
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_atanh

double __ieee754_atanh(double x)
{
#if defined __have_fpu_log1p
	double xa = __ieee754_fabs(x);
	double t;

	static const double hugeval = 1e300;
	
	if (isless(xa, 0.5))
	{
		if (xa < 0x1.0p-28)
		{
			math_force_eval(hugeval + x);
			return x;
		}

		t = xa + xa;
		t = 0.5 * __ieee754_log1p(t + t * xa / (1.0 - xa));
	} else if (isless(xa, 1.0))
	{
		t = 0.5 * __ieee754_log1p((xa + xa) / (1.0 - xa));
	} else
	{
		if (isgreater(xa, 1.0))
			return (x - x) / (x - x);

		return x / 0.0;
	}

	return __copysign(t, x);

#elif defined __have_fpu_log

	return 0.5 * log((1 + x) / (1 - x));

#else
	double t;
	int32_t hx, ix;
	uint32_t lx;

	static const double one = 1.0;
	static const double hugeval = 1e300;
	static const double zero = 0.0;

	GET_DOUBLE_WORDS(hx, lx, x);
	ix = hx & IC(0x7fffffff);
	if ((ix | ((lx | (-lx)) >> 31)) > IC(0x3ff00000))	/* |x|>1 */
		return (x - x) / (x - x);
	if (ix == IC(0x3ff00000))
		return x / zero;
	if (ix < IC(0x3e300000) && (hugeval + x) > zero)
		return x;						/* x<2**-28 */
	SET_HIGH_WORD(x, ix);
	if (ix < IC(0x3fe00000))
	{									/* x < 0.5 */
		t = x + x;
		t = 0.5 * __ieee754_log1p(t + t * x / (one - x));
	} else
	{
		t = 0.5 * __ieee754_log1p((x + x) / (one - x));
	}
	if (hx >= 0)
		return t;
	return -t;
#endif
}

#endif

/* wrapper atanh */
double __atanh(double x)
{
	if (_LIB_VERSION != _IEEE_ && isgreaterequal(__ieee754_fabs(x), 1.0))
		return __kernel_standard(x, x, HUGE_VAL, __ieee754_fabs(x) > 1.0 ? KMATHERR_ATANH_PLUSONE	/* atanh(|x|>1) */
								 : KMATHERR_ATANH_ONE);	/* atanh(|x|==1) */

	return __ieee754_atanh(x);
}

__typeof(__atanh) atanh __attribute__((weak, alias("__atanh")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__atanhl) __atanhl __attribute__((alias("__atanh")));
__typeof(__atanhl) atanhl __attribute__((weak, alias("__atanh")));
#endif
