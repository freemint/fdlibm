/* e_coshf.c -- float version of e_cosh.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 * Optimizations by Ulrich Drepper <drepper@gmail.com>, 2011
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

#ifndef __have_fpu_cosh

float __ieee754_coshf(float x)
{
	float t, w;
	int32_t ix;

	static const float hugeval = 1.0e30;
	static const float one = 1.0;
	static const float half = 0.5;
	
	GET_FLOAT_WORD(ix, x);
	ix &= IC(0x7fffffff);

	/* x is INF or NaN */
	if (!FLT_UWORD_IS_FINITE(ix))
		return x * x;

	/* |x| in [0.5*ln2,22], return (exp(|x|)+1/exp(|x|)/2; */
	if (ix < IC(0x41b00000))
	{
		/* |x| in [0,0.5*ln2], return 1+expm1(|x|)^2/(2*exp(|x|)) */
		if (ix < IC(0x3eb17218))
		{
			t = __ieee754_expm1f(__ieee754_fabsf(x));
			w = one + t;
			if (ix < IC(0x24000000))
				return w;					/* cosh(tiny) = 1 */
			return one + (t * t) / (w + w);
		}

		/* |x| in [0,22] */
		t = __ieee754_expf(__ieee754_fabsf(x));
		return half * t + half / t;
	}

	/* |x| in [22, log(maxdouble)] return half*exp(|x|) */
	if (ix <= FLT_UWORD_LOG_MAX)
		return half * __ieee754_expf(__ieee754_fabsf(x));

	/* |x| in [log(maxdouble), overflowthresold] */
	if (ix <= FLT_UWORD_LOG_2MAX)
	{
		w = __ieee754_expf(half * __ieee754_fabsf(x));
		t = half * w;
		return t * w;
	}

	/* |x| > overflowthresold, cosh(x) overflow */
	return hugeval * hugeval;
}

#endif

float __coshf(float x)
{
	float z = __ieee754_coshf(x);

	if (!isfinite(z) && isfinite(x) && _LIB_VERSION != _IEEE_)
		return __kernel_standard_f(x, x, z, KMATHERRF_COSH);	/* cosh overflow */

	return z;
}

__typeof(__coshf) coshf __attribute__((weak, alias("__coshf")));
