/* e_acoshf.c -- float version of e_acosh.c.
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

#ifndef __have_fpu_acosh

float __ieee754_acoshf(float x)
{
	float t;
	int32_t hx;

	static const float one = 1.0;
	static const float ln2 = 6.9314718246e-01;				/* 0x3f317218 */

	GET_FLOAT_WORD(hx, x);
	if (hx < IC(0x3f800000))
	{									/* x < 1 */
		return (x - x) / (x - x);
	} else if (hx >= IC(0x4d800000))
	{									/* x > 2**28 */
		if (!FLT_UWORD_IS_FINITE(hx))
		{								/* x is inf of NaN */
			return x + x;
		}
		return __ieee754_logf(x) + ln2;	/* acosh(huge)=log(2x) */
	} else if (hx == IC(0x3f800000))
	{
		return 0.0f;					/* acosh(1) = 0 */
	} else if (hx > IC(0x40000000))
	{									/* 2**28 > x > 2 */
		t = x * x;
		return __ieee754_logf(2.0F * x - one / (x + __ieee754_sqrtf(t - one)));
	} else
	{									/* 1<x<2 */
		t = x - one;
		return __ieee754_log1pf(t + __ieee754_sqrtf(2.0F * t + t * t));
	}
}

#endif

/* wrapper acoshf */
float __acoshf(float x)
{
	if (isless(x, 1.0f) && _LIB_VERSION != _IEEE_)
		/* acosh(x<1) */
		return __kernel_standard_f(x, x, __builtin_nanf(""), KMATHERRF_ACOSH);

	return __ieee754_acoshf(x);
}

__typeof(__acoshf) acoshf __attribute__((weak, alias("__acoshf")));
