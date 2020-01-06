/* s_erff.c -- float version of s_erf.c.
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

float __erfcf(float x)
{
	int32_t hx, ix;
	float R, S, P, Q, s, y, z, r;

#include "t_erff.h"

	static const float tiny = 1e-30;
	static const float half = 5.0000000000e-01;			/* 0x3F000000 */
	static const float one = 1.0000000000e+00;			/* 0x3F800000 */
	static const float two = 2.0000000000e+00;			/* 0x40000000 */
	/* c = (subfloat)0.84506291151 */
	static const float erx = 8.4506291151e-01;			/* 0x3f58560b */

	GET_FLOAT_WORD(hx, x);
	ix = hx & IC(0x7fffffff);
	if (!FLT_UWORD_IS_FINITE(ix))
	{									/* erfc(nan)=nan */
		/* erfc(+-inf)=0,2 */
		return (float) (((uint32_t) hx >> 31) << 1) + one / x;
	}

	if (ix < IC(0x3f580000))
	{									/* |x|<0.84375 */
		if (ix < IC(0x23800000))		/* |x|<2**-56 */
			return one - x;
		z = x * x;
		r = pp0 + z * (pp1 + z * (pp2 + z * (pp3 + z * pp4)));
		s = one + z * (qq1 + z * (qq2 + z * (qq3 + z * (qq4 + z * qq5))));
		y = r / s;
		if (hx < IC(0x3e800000))
		{								/* x<1/4 */
			return one - (x + x * y);
		} else
		{
			r = x * y;
			r += (x - half);
			return half - r;
		}
	}
	if (ix < IC(0x3fa00000))
	{									/* 0.84375 <= |x| < 1.25 */
		s = __ieee754_fabsf(x) - one;
		P = pa0 + s * (pa1 + s * (pa2 + s * (pa3 + s * (pa4 + s * (pa5 + s * pa6)))));
		Q = one + s * (qa1 + s * (qa2 + s * (qa3 + s * (qa4 + s * (qa5 + s * qa6)))));
		if (hx >= 0)
		{
			z = one - erx;
			return z - P / Q;
		} else
		{
			z = erx + P / Q;
			return one + z;
		}
	}
	
	if (ix < IC(0x41e00000))
	{									/* |x|<28 */
		x = __ieee754_fabsf(x);
		s = one / (x * x);
		if (ix < IC(0x4036DB6D))
		{								/* |x| < 1/.35 ~ 2.857143 */
			R = ra0 + s * (ra1 + s * (ra2 + s * (ra3 + s * (ra4 + s * (ra5 + s * (ra6 + s * ra7))))));
			S = one + s * (sa1 + s * (sa2 + s * (sa3 + s * (sa4 + s * (sa5 + s * (sa6 + s * (sa7 + s * sa8)))))));
		} else
		{								/* |x| >= 1/.35 ~ 2.857143 */
			if (hx < 0 && ix >= IC(0x40c00000))
				return two - tiny;		/* x < -6 */
			R = rb0 + s * (rb1 + s * (rb2 + s * (rb3 + s * (rb4 + s * (rb5 + s * rb6)))));
			S = one + s * (sb1 + s * (sb2 + s * (sb3 + s * (sb4 + s * (sb5 + s * (sb6 + s * sb7))))));
		}
		GET_FLOAT_WORD(ix, x);
		SET_FLOAT_WORD(z, ix & IC(0xffffe000));
		r = __ieee754_expf(-z * z - 0.5625f) * __ieee754_expf((z - x) * (z + x) + R / S);
		if (hx > 0)
			return r / x;
		else
			return two - r / x;
	}
#ifdef _WIN32
	__set_errno(ERANGE);
#endif
	if (hx > 0)
		return tiny * tiny;
	return two - tiny;
}

__typeof(__erfcf) erfcf __attribute__((weak, alias("__erfcf")));
