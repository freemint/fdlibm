/* @(#)s_sin.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/* sin(x)
 * Return sine function of x.
 *
 * kernel function:
 *	__kernel_sin		... sine function on [-pi/4,pi/4]
 *	__kernel_cos		... cose function on [-pi/4,pi/4]
 *	__ieee754_rem_pio2	... argument reduction routine
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

#ifndef __have_fpu_sin

double __ieee754_sin(double x)
{
	double y[2], z = 0.0;
	int32_t n, ix;

	/* High word of x. */
	GET_HIGH_WORD(ix, x);

	/* |x| ~< pi/4 */
	ix &= IC(0x7fffffff);
	if (ix <= IC(0x3fe921fb))
		return __kernel_sin(x, z, 0);

	/* sin(Inf or NaN) is NaN */
	else if (ix >= IC(0x7ff00000))
		return x - x;

	/* argument reduction needed */
	else
	{
		n = __ieee754_rem_pio2(x, y);
		switch ((int)(n & 3))
		{
		case 0:
			return __kernel_sin(y[0], y[1], 1);
		case 1:
			return __kernel_cos(y[0], y[1]);
		case 2:
			return -__kernel_sin(y[0], y[1], 1);
		default:
			return -__kernel_cos(y[0], y[1]);
		}
	}
}

#endif

double __sin(double x)
{
	double ret;
	
	ret = __ieee754_sin(x);
	if (isnan(ret) && !isnan(x))
		ret = __kernel_standard(x, x, ret, KMATHERR_SIN_INF);
	return ret;
}

__typeof(__sin) sin __attribute__((weak, alias("__sin")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__sinl) __sinl __attribute__((alias("__sin")));
__typeof(__sinl) sinl __attribute__((weak, alias("__sin")));
#endif
