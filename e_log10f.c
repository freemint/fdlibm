/* e_log10f.c -- float version of e_log10.c.
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

#ifndef __have_fpu_log10

float __ieee754_log10f(float x)
{
	float y, z;
	int32_t i, k, hx;

	static const float two25 = 3.3554432000e+07;			/* 0x4c000000 */
	static const float ivln10 = 4.3429449201e-01;			/* 0x3ede5bd9 */
	static const float log10_2hi = 3.0102920532e-01;		/* 0x3e9a2080 */
	static const float log10_2lo = 7.9034151668e-07;		/* 0x355427db */
	
	GET_FLOAT_WORD(hx, x);

	k = 0;
	if (hx < IC(0x00800000))
	{									/* x < 2**-126  */
		if ((hx & IC(0x7fffffff)) == 0)
			return -two25 / (x - x);	/* log(+-0)=-inf */
		if (hx < 0)
			return (x - x) / (x - x);	/* log(-#) = NaN */
		k -= 25;
		x *= two25;						/* subnormal number, scale up x */
		GET_FLOAT_WORD(hx, x);
	}
	if (hx >= IC(0x7f800000))
		return x + x;
	k += (hx >> 23) - 127;
	i = ((uint32_t) k & UC(0x80000000)) >> 31;
	hx = (hx & IC(0x007fffff)) | ((0x7f - i) << 23);
	y = (float) (k + i);
	SET_FLOAT_WORD(x, hx);
	z = y * log10_2lo + ivln10 * __ieee754_logf(x);
	return z + y * log10_2hi;
}

#endif


/* wrapper log10f(x) */
float __log10f(float x)
{
	if (islessequal(x, 0.0f) && _LIB_VERSION != _IEEE_)
	{
		if (x == 0.0f)
		{
			feraiseexcept(FE_DIVBYZERO);
			return __kernel_standard_f(x, x, -HUGE_VALF, KMATHERRF_LOG10_ZERO);	/* log10(0) */
		} else
		{
			feraiseexcept(FE_INVALID);
			return __kernel_standard_f(x, x, __builtin_nanf(""), KMATHERRF_LOG10_MINUS);	/* log10(x<0) */
		}
	}

	return __ieee754_log10f(x);
}

__typeof(__log10f) log10f __attribute__((weak, alias("__log10f")));

