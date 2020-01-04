/* e_logf.c -- float version of e_log.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 * adapted for log2 by Ulrich Drepper <drepper@cygnus.com>
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

#ifndef __have_fpu_log2

float __ieee754_log2f(float x)
{
	float hfsq, f, s, z, R, w, t1, t2, dk;
	int32_t k, ix, i, j;

	static const float ln2 = 0.69314718055994530942;
	static const float two25 = 3.355443200e+07;				/* 0x4c000000 */
	static const float Lg1 = 6.6666668653e-01;				/* 3F2AAAAB */
	static const float Lg2 = 4.0000000596e-01;				/* 3ECCCCCD */
	static const float Lg3 = 2.8571429849e-01;				/* 3E924925 */
	static const float Lg4 = 2.2222198546e-01;				/* 3E638E29 */
	static const float Lg5 = 1.8183572590e-01;				/* 3E3A3325 */
	static const float Lg6 = 1.5313838422e-01;				/* 3E1CD04F */
	static const float Lg7 = 1.4798198640e-01;				/* 3E178897 */
	
	static const float zero = 0.0;

	GET_FLOAT_WORD(ix, x);

	k = 0;
	if (ix < IC(0x00800000))
	{									/* x < 2**-126  */
		if ((ix & IC(0x7fffffff)) == 0)
			return -two25 / (x - x);	/* log(+-0)=-inf */
		if (ix < 0)
			return (x - x) / (x - x);	/* log(-#) = NaN */
		k -= 25;
		x *= two25;						/* subnormal number, scale up x */
		GET_FLOAT_WORD(ix, x);
	}
	if (ix >= IC(0x7f800000))
		return x + x;
	k += (ix >> 23) - 127;
	ix &= IC(0x007fffff);
	i = (ix + (IC(0x95f64) << 3)) & IC(0x800000);
	SET_FLOAT_WORD(x, ix | (i ^ IC(0x3f800000)));	/* normalize x or x/2 */
	k += (i >> 23);
	dk = (float) k;
	f = x - (float) 1.0;
	if ((IC(0x007fffff) & (15 + ix)) < 16)
	{									/* |f| < 2**-20 */
		if (f == zero)
			return dk;
		R = f * f * ((float) 0.5 - (float) 0.33333333333333333 * f);
		return dk - (R - f) / ln2;
	}
	s = f / ((float) 2.0 + f);
	z = s * s;
	i = ix - (IC(0x6147a) << 3);
	w = z * z;
	j = (IC(0x6b851) << 3) - ix;
	t1 = w * (Lg2 + w * (Lg4 + w * Lg6));
	t2 = z * (Lg1 + w * (Lg3 + w * (Lg5 + w * Lg7)));
	i |= j;
	R = t2 + t1;
	if (i > 0)
	{
		hfsq = (float) 0.5 *f * f;

		return dk - ((hfsq - (s * (hfsq + R))) - f) / ln2;
	} else
	{
		return dk - ((s * (f - R)) - f) / ln2;
	}
}

#endif

/* wrapper log2(x) */
float __log2f(float x)
{
	if (islessequal(x, 0.0) && _LIB_VERSION != _IEEE_)
	{
		if (x == 0.0F)
		{
			feraiseexcept(FE_DIVBYZERO);
			return __kernel_standard_f(x, x, -HUGE_VALF, KMATHERRF_LOG2_ZERO);	/* log2(0) */
		} else
		{
			feraiseexcept(FE_INVALID);
			return __kernel_standard_f(x, x, __builtin_nanf(""), KMATHERRF_LOG2_MINUS);	/* log2(x<0) */
		}
	}

	return __ieee754_log2f(x);
}

__typeof(__log2f) log2f __attribute__((weak, alias("__log2f")));
