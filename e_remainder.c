/* @(#)e_remainder.c 1.3 95/01/18 */
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

/* __ieee754_remainder(x,p)
 * Return :                  
 * 	returns  x REM p  =  x - [x/p]*p as if in infinite 
 * 	precise arithmetic, where [x/p] is the (infinite bit) 
 *	integer nearest x/p (in half way case choose the even one).
 * Method : 
 *	Based on fmod() return x-[x/p]chopped*p exactlp.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_remainder

double __ieee754_remainder(double x, double p)
{
	int32_t hx, hp;
	uint32_t sx, lx, lp;
	double p_half;

	static const double zero = 0.0;

	GET_DOUBLE_WORDS(hx, lx, x);
	GET_DOUBLE_WORDS(hp, lp, p);
	sx = hx & IC(0x80000000);
	hp &= IC(0x7fffffff);
	hx &= IC(0x7fffffff);

	/* purge off exception values */
	if ((hp | lp) == 0)
		return (x * p) / (x * p);		/* p = 0 */
	if ((hx >= IC(0x7ff00000)) ||			/* x not finite */
		((hp >= IC(0x7ff00000)) &&			/* p is NaN */
		 (((hp - IC(0x7ff00000)) | lp) != 0)))
		return (x * p) / (x * p);


	if (hp <= IC(0x7fdfffff))
		x = __ieee754_fmod(x, p + p);	/* now x < 2p */
	if (((hx - hp) | (lx - lp)) == 0)
		return zero * x;
	x = __ieee754_fabs(x);
	p = __ieee754_fabs(p);
	if (hp < IC(0x00200000))
	{
		if (x + x > p)
		{
			x -= p;
			if (x + x >= p)
				x -= p;
		}
	} else
	{
		p_half = 0.5 * p;
		if (x > p_half)
		{
			x -= p;
			if (x >= p_half)
				x -= p;
		}
	}
	GET_HIGH_WORD(hx, x);
	SET_HIGH_WORD(x, hx ^ sx);
	return x;
}

#endif

/* wrapper remainder */
double __remainder(double x, double y)
{
	if (((y == 0.0 && !isnan(x))
		 || (isinf(x) && !isnan(y))) && _LIB_VERSION != _IEEE_)
		return __kernel_standard(x, y, y, KMATHERR_REMAINDER);	/* remainder domain */

	return __ieee754_remainder(x, y);
}


#undef drem
#undef dreml

__typeof(__remainder) remainder __attribute__((weak, alias("__remainder")));
__typeof(__remainder) __drem __attribute__((alias("__remainder")));
__typeof(remainder) drem __attribute__((weak, alias("__drem")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__remainderl) __remainderl __attribute__((alias("__remainder")));
__typeof(__remainderl) remainderl __attribute__((weak, alias("__remainder")));
__typeof(__remainderl) __dreml __attribute__((alias("__remainderl")));
__typeof(__remainderl) dreml __attribute__((weak, alias("__dreml")));
#endif
