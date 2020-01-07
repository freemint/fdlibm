/* e_sinhf.c -- float version of e_sinh.c.
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
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_sinh

float __ieee754_sinhf(float x)
{
	float t, w, h;
	int32_t ix, jx;

	static const float one = 1.0;
	static const float shuge = 1.0e37;
	
	GET_FLOAT_WORD(jx, x);
	ix = jx & IC(0x7fffffff);

	/* x is INF or NaN */
	if (!FLT_UWORD_IS_FINITE(ix))
		return x + x;

	h = 0.5;
	if (jx < 0)
		h = -h;
	/* |x| in [0,22], return sign(x)*0.5*(E+E/(E+1))) */
	if (ix < IC(0x41b00000))
	{									/* |x|<22 */
		if (ix < IC(0x31800000))			/* |x|<2**-28 */
			if (shuge + x > one)
				return x;				/* sinh(tiny) = tiny with inexact */
		t = __ieee754_expm1f(__ieee754_fabsf(x));
		if (ix < IC(0x3f800000))
			return h * (2.0f * t - t * t / (t + one));
		return h * (t + t / (t + one));
	}

	/* |x| in [22, log(maxdouble)] return 0.5*exp(|x|) */
	if (ix <= FLT_UWORD_LOG_MAX)
		return h * __ieee754_expf(__ieee754_fabsf(x));

	/* |x| in [log(maxdouble), overflowthresold] */
	if (ix <= FLT_UWORD_LOG_2MAX)
	{
		w = __ieee754_expf(0.5f * __ieee754_fabsf(x));
		t = h * w;
		return t * w;
	}

	/* |x| > overflowthresold, sinh(x) overflow */
	return x * shuge;
}

#endif

float __sinhf(float x)
{
	float z = __ieee754_sinhf(x);

	if (!isfinite(z) && isfinite(x) && _LIB_VERSION != _IEEE_)
		return __kernel_standard_f(x, x, z, KMATHERRF_SINH);	/* sinh overflow */

	return z;
}

__typeof(__sinhf) sinhf __attribute__((weak, alias("__sinhf")));
