/* s_sinl.c -- long double version of s_sin.c.
 * Conversion to long double by Ulrich Drepper,
 * Cygnus Support, drepper@cygnus.com.
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

/* sinl(x)
 * Return sine function of x.
 *
 * kernel function:
 *	__kernel_sinl		... sine function on [-pi/4,pi/4]
 *	__kernel_cosl		... cose function on [-pi/4,pi/4]
 *	__ieee754_rem_pio2l	... argument reduction routine
 *
 * Method.
 *      Let S,C and T denote the sin, cos and tan respectively on
 *	[-PI/4, +PI/4]. Reduce the argument x to y1+y2 = x-k*pi/2
 *	in [-pi/4 , +pi/4], and let n = k mod 4.
 *	We have
 *
 *          n        sin(x)      cos(x)        tan(x)
 *     ----------------------------------------------------------
 *	    0	       S	   C		 T
 *	    1	       C	  -S		-1/T
 *	    2	      -S	  -C		 T
 *	    3	      -C	   S		-1/T
 *     ----------------------------------------------------------
 *
 * Special cases:
 *      Let trig be any of sin, cos, or tan.
 *      trig(+-INF)  is NaN, with signals;
 *      trig(NaN)    is that NaN;
 *
 * Accuracy:
 *	TRIG(x) returns trig(x) nearly rounded
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_sin

long double __ieee754_sinl(long double x)
{
	long double y[2], z = 0.0;
	int32_t n, se;
	uint32_t i0, i1;

	/* High word of x. */
	GET_LDOUBLE_WORDS(se, i0, i1, x);
	(void) i1;
	
	/* |x| ~< pi/4 */
	se &= 0x7fff;
	if (se < 0x3ffe || (se == 0x3ffe && i0 <= UC(0xc90fdaa2)))
		return __kernel_sinl(x, z, 0);

	/* sin(Inf or NaN) is NaN */
	else if (se == 0x7fff)
		return x - x;

	/* argument reduction needed */
	else
	{
		n = __ieee754_rem_pio2l(x, y);
		switch ((int)(n & 3))
		{
		case 0:
			return __kernel_sinl(y[0], y[1], 1);
		case 1:
			return __kernel_cosl(y[0], y[1]);
		case 2:
			return -__kernel_sinl(y[0], y[1], 1);
		default:
			return -__kernel_cosl(y[0], y[1]);
		}
	}
}

#endif

long double __sinl(long double x)
{
	long double ret;
	
	ret = __ieee754_sinl(x);
	if (isnan(ret) && !isnan(x))
		ret = __kernel_standard_l(x, x, ret, KMATHERRL_SIN_INF);
	return ret;
}

__typeof(__sinl) sinl __attribute__((weak, alias("__sinl")));

#endif
