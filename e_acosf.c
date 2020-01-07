/* e_acosf.c -- float version of e_acos.c.
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

#ifndef __have_fpu_acos

float __ieee754_acosf(float x)
{
	float z, p, q, r, w, s, c, df;
	int32_t hx, ix;

	static const float one = 1.0000000000e+00;			/* 0x3F800000 */
	static const float pi = 3.1415925026e+00;			/* 0x40490fda */
	static const float pio2_hi = 1.5707962513e+00;		/* 0x3fc90fda */
	static const float pio2_lo = 7.5497894159e-08;		/* 0x33a22168 */
	static const float pS0 = 1.6666667163e-01;			/* 0x3e2aaaab */
	static const float pS1 = -3.2556581497e-01;			/* 0xbea6b090 */
	static const float pS2 = 2.0121252537e-01;			/* 0x3e4e0aa8 */
	static const float pS3 = -4.0055535734e-02;			/* 0xbd241146 */
	static const float pS4 = 7.9153501429e-04;			/* 0x3a4f7f04 */
	static const float pS5 = 3.4793309169e-05;			/* 0x3811ef08 */
	static const float qS1 = -2.4033949375e+00;			/* 0xc019d139 */
	static const float qS2 = 2.0209457874e+00;			/* 0x4001572d */
	static const float qS3 = -6.8828397989e-01;			/* 0xbf303361 */
	static const float qS4 = 7.7038154006e-02;			/* 0x3d9dc62e */
	
	GET_FLOAT_WORD(hx, x);
	ix = hx & IC(0x7fffffff);
	if (ix == IC(0x3f800000))
	{									/* |x|==1 */
		if (hx > 0)
			return 0.0;					/* acos(1) = 0  */
		else
			return pi + 2.0f * pio2_lo;	/* acos(-1)= pi */
	} else if (ix > IC(0x3f800000))
	{									/* |x| >= 1 */
		return (x - x) / (x - x);		/* acos(|x|>1) is NaN */
	}
	if (ix < IC(0x3f000000))
	{									/* |x| < 0.5 */
		if (ix <= IC(0x32800000))
			return pio2_hi + pio2_lo;	/*if|x|<=2**-26 */
		z = x * x;
		p = z * (pS0 + z * (pS1 + z * (pS2 + z * (pS3 + z * (pS4 + z * pS5)))));
		q = one + z * (qS1 + z * (qS2 + z * (qS3 + z * qS4)));
		r = p / q;
		return pio2_hi - (x - (pio2_lo - x * r));
	} else if (hx < 0)
	{									/* x < -0.5 */
		z = (one + x) * 0.5f;
		p = z * (pS0 + z * (pS1 + z * (pS2 + z * (pS3 + z * (pS4 + z * pS5)))));
		q = one + z * (qS1 + z * (qS2 + z * (qS3 + z * qS4)));
		s = __ieee754_sqrtf(z);
		r = p / q;
		w = r * s - pio2_lo;
		return pi - 2.0f * (s + w);
	} else
	{									/* x > 0.5 */
		int32_t idf;

		z = (one - x) * 0.5f;
		s = __ieee754_sqrtf(z);
		df = s;
		GET_FLOAT_WORD(idf, df);
		SET_FLOAT_WORD(df, idf & IC(0xfffff000));
		c = (z - df * df) / (s + df);
		p = z * (pS0 + z * (pS1 + z * (pS2 + z * (pS3 + z * (pS4 + z * pS5)))));
		q = one + z * (qS1 + z * (qS2 + z * (qS3 + z * qS4)));
		r = p / q;
		w = r * s + c;
		return 2.0f * (df + w);
	}
}

#endif

/* wrapper acosf */
float __acosf(float x)
{
	if (isgreater(__ieee754_fabsf(x), 1.0f) && _LIB_VERSION != _IEEE_)
	{
		/* acos(|x|>1) */
		feraiseexcept(FE_INVALID);
		return __kernel_standard_f(x, x, __builtin_nanf(""), KMATHERRF_ACOS);
	}

	return __ieee754_acosf(x);
}

__typeof(__acosf) acosf __attribute__((weak, alias("__acosf")));
