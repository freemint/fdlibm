/* e_j1f.c -- float version of e_j1.c.
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

float __ieee754_j1f(float x)
{
	float z, s, c, ss, cc, r, u, v, y;
	int32_t hx, ix;

	static const float hugeval = 1e30;
	static const float one = 1.0;
	static const float invsqrtpi = 5.6418961287e-01;		/* 0x3f106ebb */
	static const float zero = 0.0;

	/* R0/S0 on [0,2] */
	static const float r00 = -6.2500000000e-02;				/* 0xbd800000 */
	static const float r01 = 1.4070566976e-03;				/* 0x3ab86cfd */
	static const float r02 = -1.5995563444e-05;				/* 0xb7862e36 */
	static const float r03 = 4.9672799207e-08;				/* 0x335557d2 */
	static const float s01 = 1.9153760746e-02;				/* 0x3c9ce859 */
	static const float s02 = 1.8594678841e-04;				/* 0x3942fab6 */
	static const float s03 = 1.1771846857e-06;				/* 0x359dffc2 */
	static const float s04 = 5.0463624390e-09;				/* 0x31ad6446 */
	static const float s05 = 1.2354227016e-11;				/* 0x2d59567e */
	
	
	GET_FLOAT_WORD(hx, x);
	ix = hx & IC(0x7fffffff);
	if (!FLT_UWORD_IS_FINITE(ix))
		return one / x;
	y = __ieee754_fabsf(x);
	if (ix >= IC(0x40000000))
	{									/* |x| >= 2.0 */
		__ieee754_sincosf(y, &s, &c);
		ss = -s - c;
		cc = s - c;
		if (ix <= FLT_UWORD_HALF_MAX)
		{								/* make sure y+y not overflow */
			z = __ieee754_cosf(y + y);
			if ((s * c) > zero)
				cc = z / ss;
			else
				ss = z / cc;
		}
		/*
		 * j1(x) = 1/sqrt(pi) * (P(1,x)*cc - Q(1,x)*ss) / sqrt(x)
		 * y1(x) = 1/sqrt(pi) * (P(1,x)*ss + Q(1,x)*cc) / sqrt(x)
		 */
		if (ix > IC(0x48000000))
			z = (invsqrtpi * cc) / __ieee754_sqrtf(y);
		else
		{
			u = __j1_y1_ponef(y);
			v = __j1_y1_qonef(y);
			z = invsqrtpi * (u * cc - v * ss) / __ieee754_sqrtf(y);
		}
		if (hx < 0)
			return -z;
		else
			return z;
	}
	if (ix < IC(0x32000000))
	{									/* |x|<2**-27 */
		if (hugeval + x > one)
			return 0.5f * x;		/* inexact if x!=0 necessary */
	}
	z = x * x;
	r = z * (r00 + z * (r01 + z * (r02 + z * r03)));
	s = one + z * (s01 + z * (s02 + z * (s03 + z * (s04 + z * s05))));
	r *= x;
	return (x * 0.5f + r / s);
}


/* wrapper j1 */
float __j1f(float x)
{
	if (isgreater(__ieee754_fabsf(x), X_TLOSS) && _LIB_VERSION != _IEEE_ && _LIB_VERSION != _POSIX_)
		/* j1(|x|>X_TLOSS) */
		return __kernel_standard_f(x, x, 0.0f, KMATHERRF_J1_TLOSS);

	return __ieee754_j1f(x);
}

__typeof(__j1f) j1f __attribute__((weak, alias("__j1f")));
