/* e_atanhf.c -- float version of e_atanh.c.
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
 *
 */

/* __ieee754_atanh(x)
   Method :
      1.Reduced x to positive by atanh(-x) = -atanh(x)
      2.For x>=0.5
		    1              2x                          x
	atanh(x) = --- * log(1 + -------) = 0.5 * log1p(2 * --------)
		    2             1 - x                      1 - x

	For x<0.5
	atanh(x) = 0.5*log1p(2x+2x*x/(1-x))

   Special cases:
	atanh(x) is NaN if |x| > 1 with signal;
	atanh(NaN) is that NaN with no signal;
	atanh(+-1) is +-INF with signal.

 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_atanh

float __ieee754_atanhf(float x)
{
#if defined __have_fpu_log1p
	float xa = __ieee754_fabsf(x);
	float t;

	static const float hugeval = 1e30;

	if (isless(xa, 0.5f))
	{
		if (xa < hexconstf(0x1.0p-28f, 3.7252902984619140625000e-09F, UC(0x31800000)))
		{
			math_force_eval(hugeval + x);
			return x;
		}

		t = xa + xa;
		t = 0.5f * __ieee754_log1pf(t + t * xa / (1.0f - xa));
	} else if (isless(xa, 1.0f))
	{
		t = 0.5f * __ieee754_log1pf((xa + xa) / (1.0f - xa));
	} else
	{
		if (isgreater(xa, 1.0f))
			return (x - x) / (x - x);

		return x / 0.0f;
	}

	return __copysignf(t, x);

#elif defined __have_fpu_log

	return 0.5f * logf((1.0f + x) / (1.0f - x));

#else

	float t;
	int32_t hx, ix;

	static const float one = 1.0;
	static const float hugeval = 1e30;
	static const float zero = 0.0;
	
	GET_FLOAT_WORD(hx, x);
	ix = hx & IC(0x7fffffff);
	if (ix > IC(0x3f800000))				/* |x|>1 */
		return (x - x) / (x - x);
	if (ix == IC(0x3f800000))
		return x / zero;
	if (ix < IC(0x31800000) && (hugeval + x) > zero)
		return x;						/* x<2**-28 */
	SET_FLOAT_WORD(x, ix);
	if (ix < IC(0x3f000000))
	{									/* x < 0.5 */
		t = x + x;
		t = 0.5f * __ieee754_log1pf(t + t * x / (one - x));
	} else
	{
		t = 0.5f * __ieee754_log1pf((x + x) / (one - x));
	}
	
	if (hx >= 0)
		return t;
	return -t;

#endif
}

#endif

/* wrapper atanhf */
float __atanhf(float x)
{
	if (isgreaterequal(__ieee754_fabsf(x), 1.0F) && _LIB_VERSION != _IEEE_)
		return __kernel_standard_f(x, x, HUGE_VALF, __ieee754_fabsf(x) > 1.0F ? KMATHERRF_ATANH_PLUSONE	/* atanh(|x|>1) */
								 : KMATHERRF_ATANH_ONE);	/* atanh(|x|==1) */

	return __ieee754_atanhf(x);
}

__typeof(__atanhf) atanhf __attribute__((weak, alias("__atanhf")));
