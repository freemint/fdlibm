/* s_asinhf.c -- float version of s_asinh.c.
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

#ifndef __have_fpu_asinh

float __ieee754_asinhf(float x)
{
	float w;
	int32_t hx, ix;

	static const float one = 1.0000000000e+00;		/* 0x3F800000 */
	static const float ln2 = 6.9314718246e-01;		/* 0x3f317218 */
	static const float hugeval = 1.0000000000e+30;
	
	GET_FLOAT_WORD(hx, x);
	ix = hx & IC(0x7fffffff);
	if (ix < IC(0x38000000))
	{									/* |x|<2**-14 */
		if (hugeval + x > one)
			return x;					/* return x inexact except 0 */
	}
	if (ix > IC(0x47000000))
	{									/* |x| > 2**14 */
		if (!FLT_UWORD_IS_FINITE(ix))
			return x + x;				/* x is inf or NaN */
		w = __ieee754_logf(__ieee754_fabsf(x)) + ln2;
	} else
	{
		float xa = __ieee754_fabsf(x);

		if (ix > IC(0x40000000))
		{								/* 2**14 > |x| > 2.0 */
			w = __ieee754_logf(2.0f * xa + one / (__ieee754_sqrtf(xa * xa + one) + xa));
		} else
		{								/* 2.0 > |x| > 2**-14 */
			float t = xa * xa;

			w = __ieee754_log1pf(xa + t / (one + __ieee754_sqrtf(one + t)));
		}
	}
	if (hx > 0)
		return w;
	return -w;
}

#endif

float __asinhf(float x)
{
	return __ieee754_asinhf(x);
}

__typeof(__asinhf) asinhf __attribute__((weak, alias("__asinhf")));
