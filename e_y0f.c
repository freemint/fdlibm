/* e_y0f.c -- float version of e_y0.c.
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


float __ieee754_y0f(float x)
{
	float z, s, c, ss, cc, u, v;
	int32_t hx, ix;

	static const float one = 1.0;
	static const float invsqrtpi = 5.6418961287e-01;		/* 0x3f106ebb */
	static const float tpi = 6.3661974669e-01;				/* 0x3f22f983 */
	static const float zero = 0.0;
	
	static const float u00 = -7.3804296553e-02;				/* 0xbd9726b5 */
	static const float u01 = 1.7666645348e-01;				/* 0x3e34e80d */
	static const float u02 = -1.3818567619e-02;				/* 0xbc626746 */
	static const float u03 = 3.4745343146e-04;				/* 0x39b62a69 */
	static const float u04 = -3.8140706238e-06;				/* 0xb67ff53c */
	static const float u05 = 1.9559013964e-08;				/* 0x32a802ba */
	static const float u06 = -3.9820518410e-11;				/* 0xae2f21eb */
	static const float v01 = 1.2730483897e-02;				/* 0x3c509385 */
	static const float v02 = 7.6006865129e-05;				/* 0x389f65e0 */
	static const float v03 = 2.5915085189e-07;				/* 0x348b216c */
	static const float v04 = 4.4111031494e-10;				/* 0x2ff280c2 */
	
	GET_FLOAT_WORD(hx, x);
	ix = IC(0x7fffffff) & hx;
	/* Y0(NaN) is NaN, y0(-inf) is Nan, y0(inf) is 0, y0(0) is -inf.  */
	if (!FLT_UWORD_IS_FINITE(ix))
		return one / (x + x * x);
	if (FLT_UWORD_IS_ZERO(ix))
		return -HUGE_VALF + x;			/* -inf and overflow exception.  */
	if (hx < 0)
		return zero / (zero * x);
	if (ix >= IC(0x40000000))
	{									/* |x| >= 2.0 */
		/* y0(x) = sqrt(2/(pi*x))*(p0(x)*sin(x0)+q0(x)*cos(x0))
		 * where x0 = x-pi/4
		 *      Better formula:
		 *              cos(x0) = cos(x)cos(pi/4)+sin(x)sin(pi/4)
		 *                      =  1/sqrt(2) * (sin(x) + cos(x))
		 *              sin(x0) = sin(x)cos(3pi/4)-cos(x)sin(3pi/4)
		 *                      =  1/sqrt(2) * (sin(x) - cos(x))
		 * To avoid cancellation, use
		 *              sin(x) +- cos(x) = -cos(2x)/(sin(x) -+ cos(x))
		 * to compute the worse one.
		 */
		__ieee754_sincosf(x, &s, &c);
		ss = s - c;
		cc = s + c;
		/*
		 * j0(x) = 1/sqrt(pi) * (P(0,x)*cc - Q(0,x)*ss) / sqrt(x)
		 * y0(x) = 1/sqrt(pi) * (P(0,x)*ss + Q(0,x)*cc) / sqrt(x)
		 */
		if (ix <= FLT_UWORD_HALF_MAX)
		{								/* make sure x+x not overflow */
			z = -__ieee754_cosf(x + x);
			if ((s * c) < zero)
				cc = z / ss;
			else
				ss = z / cc;
		}
		if (ix > IC(0x48000000))
			z = (invsqrtpi * ss) / __ieee754_sqrtf(x);
		else
		{
			u = __j0_y0_pzerof(x);
			v = __j0_y0_qzerof(x);
			z = invsqrtpi * (u * ss + v * cc) / __ieee754_sqrtf(x);
		}
		return z;
	}
	if (ix <= IC(0x39800000))
	{									/* x < 2**-13 */
		return (u00 + tpi * __ieee754_logf(x));
	}
	z = x * x;
	u = u00 + z * (u01 + z * (u02 + z * (u03 + z * (u04 + z * (u05 + z * u06)))));
	v = one + z * (v01 + z * (v02 + z * (v03 + z * v04)));
	return (u / v + tpi * (__ieee754_j0f(x) * __ieee754_logf(x)));
}

/* wrapper y0 */
float __y0f(float x)
{
	if ((islessequal(x, 0.0F) || isgreater(x, X_TLOSS)) && _LIB_VERSION != _IEEE_)
	{
		if (x < 0.0F)
		{
			/* d = zero/(x-x) */
			feraiseexcept(FE_INVALID);
			return __kernel_standard_f(x, x, -HUGE_VALF, KMATHERRF_Y0_MINUS);
		} else if (x == 0.0F)
			/* d = -one/(x-x) */
			return __kernel_standard_f(x, x, -HUGE_VALF, KMATHERRF_Y0_ZERO);
		else if (_LIB_VERSION != _POSIX_)
			/* y0(x>X_TLOSS) */
			return __kernel_standard_f(x, x, 0.0f, KMATHERRF_Y0_TLOSS);
	}

	return __ieee754_y0f(x);
}

__typeof(__y0f) y0f __attribute__((weak, alias("__y0f")));
