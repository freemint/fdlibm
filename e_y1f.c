/* e_y1f.c -- float version of e_y1.c.
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

float __ieee754_y1f(float x)
{
	float z, s, c, ss, cc, u, v;
	int32_t hx, ix;

	static const float one = 1.0;
	static const float invsqrtpi = 5.6418961287e-01;		/* 0x3f106ebb */
	static const float tpi = 6.3661974669e-01;				/* 0x3f22f983 */
	static const float zero = 0.0;

	static const float U0[5] = {
		-1.9605709612e-01,					/* 0xbe48c331 */
		5.0443872809e-02,					/* 0x3d4e9e3c */
		-1.9125689287e-03,					/* 0xbafaaf2a */
		2.3525259166e-05,					/* 0x37c5581c */
		-9.1909917899e-08					/* 0xb3c56003 */
	};
	
	static const float V0[5] = {
		1.9916731864e-02,					/* 0x3ca3286a */
		2.0255257550e-04,					/* 0x3954644b */
		1.3560879779e-06,					/* 0x35b602d4 */
		6.2274145840e-09,					/* 0x31d5f8eb */
		1.6655924903e-11					/* 0x2d9281cf */
	};
	
	GET_FLOAT_WORD(hx, x);
	ix = IC(0x7fffffff) & hx;
	/* if Y1(NaN) is NaN, Y1(-inf) is NaN, Y1(inf) is 0 */
	if (!FLT_UWORD_IS_FINITE(ix))
		return one / (x + x * x);
	if (FLT_UWORD_IS_ZERO(ix))
		return -HUGE_VALF + x;			/* -inf and overflow exception.  */
	if (hx < 0)
		return zero / (zero * x);
	if (ix >= IC(0x40000000))
	{									/* |x| >= 2.0 */
		__ieee754_sincosf(x, &s, &c);
		ss = -s - c;
		cc = s - c;
		if (ix <= FLT_UWORD_HALF_MAX)
		{								/* make sure x+x not overflow */
			z = __ieee754_cosf(x + x);
			if ((s * c) > zero)
				cc = z / ss;
			else
				ss = z / cc;
		}
		/* y1(x) = sqrt(2/(pi*x))*(p1(x)*sin(x0)+q1(x)*cos(x0))
		 * where x0 = x-3pi/4
		 *      Better formula:
		 *              cos(x0) = cos(x)cos(3pi/4)+sin(x)sin(3pi/4)
		 *                      =  1/sqrt(2) * (sin(x) - cos(x))
		 *              sin(x0) = sin(x)cos(3pi/4)-cos(x)sin(3pi/4)
		 *                      = -1/sqrt(2) * (cos(x) + sin(x))
		 * To avoid cancellation, use
		 *              sin(x) +- cos(x) = -cos(2x)/(sin(x) -+ cos(x))
		 * to compute the worse one.
		 */
		if (ix > IC(0x48000000))
			z = (invsqrtpi * ss) / __ieee754_sqrtf(x);
		else
		{
			u = __j1_y1_ponef(x);
			v = __j1_y1_qonef(x);
			z = invsqrtpi * (u * ss + v * cc) / __ieee754_sqrtf(x);
		}
		return z;
	}
	if (ix <= IC(0x33000000))
	{									/* x < 2**-25 */
		return (-tpi / x);
	}
	z = x * x;
	u = U0[0] + z * (U0[1] + z * (U0[2] + z * (U0[3] + z * U0[4])));
	v = one + z * (V0[0] + z * (V0[1] + z * (V0[2] + z * (V0[3] + z * V0[4]))));
	return (x * (u / v) + tpi * (__ieee754_j1f(x) * __ieee754_logf(x) - one / x));
}

/* wrapper y1 */
float __y1f(float x)
{
	if ((islessequal(x, 0.0F) || isgreater(x, X_TLOSS)) && _LIB_VERSION != _IEEE_)
	{
		if (x < 0.0F)
		{
			/* d = zero/(x-x) */
			feraiseexcept(FE_INVALID);
			return __kernel_standard_f(x, x, -HUGE_VALF, KMATHERRF_Y1_MINUS);
		} else if (x == 0.0F)
			/* d = -one/(x-x) */
			return __kernel_standard_f(x, x, -HUGE_VALF, KMATHERRF_Y1_ZERO);
		else if (_LIB_VERSION != _POSIX_)
			/* y1(x>X_TLOSS) */
			return __kernel_standard_f(x, x, 0.0f, KMATHERRF_Y1_TLOSS);
	}

	return __ieee754_y1f(x);
}

__typeof(__y1f) y1f __attribute__((weak, alias("__y1f")));
