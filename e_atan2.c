/* @(#)e_atan2.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 *
 */

/* __ieee754_atan2(y,x)
 * Method :
 *	1. Reduce y to positive by atan2(y,x)=-atan2(-y,x).
 *	2. Reduce x to positive by (if x and y are unexceptional): 
 *		ARG (x+iy) = arctan(y/x)   	   ... if x > 0,
 *		ARG (x+iy) = pi - arctan[y/(-x)]   ... if x < 0,
 *
 * Special cases:
 *
 *	ATAN2((anything), NaN ) is NaN;
 *	ATAN2(NAN , (anything) ) is NaN;
 *	ATAN2(+-0, +(anything but NaN)) is +-0  ;
 *	ATAN2(+-0, -(anything but NaN)) is +-pi ;
 *	ATAN2(+-(anything but 0 and NaN), 0) is +-pi/2;
 *	ATAN2(+-(anything but INF and NaN), +INF) is +-0 ;
 *	ATAN2(+-(anything but INF and NaN), -INF) is +-pi;
 *	ATAN2(+-INF,+INF ) is +-pi/4 ;
 *	ATAN2(+-INF,-INF ) is +-3pi/4;
 *	ATAN2(+-INF, (anything but,0,NaN, and INF)) is +-pi/2;
 *
 * Constants:
 * The hexadecimal values are the intended ones for the following 
 * constants. The decimal values may be used, provided that the 
 * compiler will convert from decimal to binary accurately enough 
 * to produce the hexadecimal values shown.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_atan2

double __ieee754_atan2(double y, double x)
{
	double z;
	int32_t k, m, hx, hy, ix, iy;
	uint32_t lx, ly;

	static const double tiny = 1.0e-300;
	static const double zero = 0.0;
	static const double pi_o_4 = 7.8539816339744827900E-01;	/* 0x3FE921FB, 0x54442D18 */
	static const double pi_o_2 = 1.5707963267948965580E+00;	/* 0x3FF921FB, 0x54442D18 */
	static const double pi = 3.1415926535897931160E+00;		/* 0x400921FB, 0x54442D18 */
	static const double pi_lo = 1.2246467991473531772E-16;	/* 0x3CA1A626, 0x33145C07 */

	GET_DOUBLE_WORDS(hx, lx, x);
	ix = hx & IC(0x7fffffff);
	GET_DOUBLE_WORDS(hy, ly, y);
	iy = hy & IC(0x7fffffff);
	if (((ix | ((lx | -lx) >> 31)) > IC(0x7ff00000)) || ((iy | ((ly | -ly) >> 31)) > IC(0x7ff00000)))	/* x or y is NaN */
		return x + y;
	if (((hx - IC(0x3ff00000)) | lx) == 0)
		return __ieee754_atan(y);					/* x=1.0 */
	m = ((hy >> 31) & 1) | ((hx >> 30) & 2);	/* 2*sign(x)+sign(y) */

	/* when y = 0 */
	if ((iy | ly) == 0)
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
	if ((ix | lx) == 0)
		return (hy < 0) ? -pi_o_2 - tiny : pi_o_2 + tiny;

	/* when x is INF */
	if (ix == IC(0x7ff00000))
	{
		if (iy == IC(0x7ff00000))
		{
			switch ((int)m)
			{
			case 0:
				return pi_o_4 + tiny;	/* atan(+INF,+INF) */
			case 1:
				return -pi_o_4 - tiny;	/* atan(-INF,+INF) */
			case 2:
				return 3.0 * pi_o_4 + tiny;	/*atan(+INF,-INF) */
			case 3:
				return -3.0 * pi_o_4 - tiny;	/*atan(-INF,-INF) */
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
	if (iy == IC(0x7ff00000))
		return (hy < 0) ? -pi_o_2 - tiny : pi_o_2 + tiny;

	/* compute y/x */
	k = (iy - ix) >> 20;
	if (k > 60)
		z = pi_o_2 + 0.5 * pi_lo;		/* |y/x| >  2**60 */
	else if (hx < 0 && k < -60)
		z = 0.0;						/* |y|/x < -2**60 */
	else
		z = __ieee754_atan(__ieee754_fabs(y / x));			/* safe to do y/x */
	switch ((int)m)
	{
	case 0:
		return z;						/* atan(+,+) */
	case 1:
		{
			uint32_t zh;

			GET_HIGH_WORD(zh, z);
			SET_HIGH_WORD(z, zh ^ UC(0x80000000));
		}
		return z;						/* atan(-,+) */
	case 2:
		return pi - (z - pi_lo);		/* atan(+,-) */
	default:							/* case 3 */
		return (z - pi_lo) - pi;		/* atan(-,-) */
	}
}

#endif

double __atan2(double y, double x)	/* wrapper atan2 */
{
	if (_LIB_VERSION == _SVID_ && x == 0.0 && y == 0.0)
		return __kernel_standard(y, x, HUGE_VAL, KMATHERR_ATAN2);	/* atan2(+-0,+-0) */

	return __ieee754_atan2(y, x);
}

__typeof(__atan2) atan2 __attribute__((weak, alias("__atan2")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__atan2l) __atan2l __attribute__((alias("__atan2")));
__typeof(__atan2l) atan2l __attribute__((weak, alias("__atan2")));
#endif
