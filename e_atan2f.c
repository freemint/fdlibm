/* e_atan2f.c -- float version of e_atan2.c.
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

#ifndef __have_fpu_atan2

float __ieee754_atan2f(float y, float x)
{
	float z;
	int32_t k, m, hx, hy, ix, iy;

	static const float tiny = 1.0e-30;
	static const float zero = 0.0;
	static const float pi_o_4 = 7.8539818525e-01;			/* 0x3f490fdb */
	static const float pi_o_2 = 1.5707963705e+00;			/* 0x3fc90fdb */
	static const float pi = 3.1415927410e+00;				/* 0x40490fdb */
	static const float pi_lo = -8.7422776573e-08;			/* 0xb3bbbd2e */
	
	GET_FLOAT_WORD(hx, x);
	ix = hx & IC(0x7fffffff);
	GET_FLOAT_WORD(hy, y);
	iy = hy & IC(0x7fffffff);
	if (FLT_UWORD_IS_NAN(ix) || FLT_UWORD_IS_NAN(iy))	/* x or y is NaN */
		return x + y;
	if (hx == IC(0x3f800000))
		return __ieee754_atanf(y);				/* x=1.0 */
	m = ((hy >> 31) & 1) | ((hx >> 30) & 2);	/* 2*sign(x)+sign(y) */

	/* when y = 0 */
	if (FLT_UWORD_IS_ZERO(iy))
	{
		switch ((int)m)
		{
		case 0:
		case 1:
			return y;					/* atan(+-0,+anything)=+-0 */
		case 2:
			return pi + tiny;			/* atan(+0,-anything) = pi */
		case 3:
			return -pi - tiny;			/* atan(-0,-anything) =-pi */
		}
	}
	/* when x = 0 */
	if (FLT_UWORD_IS_ZERO(ix))
		return (hy < 0) ? -pi_o_2 - tiny : pi_o_2 + tiny;

	/* when x is INF */
	if (FLT_UWORD_IS_INFINITE(ix))
	{
		if (FLT_UWORD_IS_INFINITE(iy))
		{
			switch ((int)m)
			{
			case 0:
				return pi_o_4 + tiny;	/* atan(+INF,+INF) */
			case 1:
				return -pi_o_4 - tiny;	/* atan(-INF,+INF) */
			case 2:
				return (float) 3.0 *pi_o_4 + tiny;	/*atan(+INF,-INF) */

			case 3:
				return (float) -3.0 * pi_o_4 - tiny;	/*atan(-INF,-INF) */
			}
		} else
		{
			switch ((int)m)
			{
			case 0:
				return zero;			/* atan(+...,+INF) */
			case 1:
				return -zero;			/* atan(-...,+INF) */
			case 2:
				return pi + tiny;		/* atan(+...,-INF) */
			case 3:
				return -pi - tiny;		/* atan(-...,-INF) */
			}
		}
	}
	/* when y is INF */
	if (FLT_UWORD_IS_INFINITE(iy))
		return (hy < 0) ? -pi_o_2 - tiny : pi_o_2 + tiny;

	/* compute y/x */
	k = (iy - ix) >> 23;
	if (k > 60)
		z = pi_o_2 + (float) 0.5 *pi_lo;	/* |y/x| >  2**60 */

	else if (hx < 0 && k < -60)
		z = 0.0;						/* |y|/x < -2**60 */
	else
		z = __ieee754_atanf(__ieee754_fabsf(y / x));		/* safe to do y/x */
	switch ((int)m)
	{
	case 0:
		return z;						/* atan(+,+) */
	case 1:
		{
			uint32_t zh;

			GET_FLOAT_WORD(zh, z);
			SET_FLOAT_WORD(z, zh ^ UC(0x80000000));
		}
		return z;						/* atan(-,+) */
	case 2:
		return pi - (z - pi_lo);		/* atan(+,-) */
	default:							/* case 3 */
		return (z - pi_lo) - pi;		/* atan(-,-) */
	}
}

#endif

float __atan2f(float y, float x)
{
	if (x == 0.0f && y == 0.0f && _LIB_VERSION == _SVID_)
		return __kernel_standard_f(y, x, HUGE_VALF, KMATHERRF_ATAN2);	/* atan2(+-0,+-0) */

	return __ieee754_atan2f(y, x);
}

__typeof(__atan2f) atan2f __attribute__((weak, alias("__atan2f")));
