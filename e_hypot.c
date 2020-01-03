/* @(#)e_hypot.c 1.3 95/01/18 */
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

/* __ieee754_hypot(x,y)
 *
 * Method :                  
 *	If (assume round-to-nearest) z=x*x+y*y 
 *	has error less than sqrt(2)/2 ulp, than 
 *	sqrt(z) has error less than 1 ulp (exercise).
 *
 *	So, compute sqrt(x*x+y*y) with some care as 
 *	follows to get the error below 1 ulp:
 *
 *	Assume x>y>0;
 *	(if possible, set rounding to round-to-nearest)
 *	1. if x > 2y  use
 *		x1*x1+(y*y+(x2*(x+x1))) for x*x+y*y
 *	where x1 = x with lower 32 bits cleared, x2 = x-x1; else
 *	2. if x <= 2y use
 *		t1*y1+((x-y)*(x-y)+(t1*y2+t2*y))
 *	where t1 = 2x with lower 32 bits cleared, t2 = 2x-t1, 
 *	y1= y with lower 32 bits chopped, y2 = y-y1.
 *		
 *	NOTE: scaling may be necessary if some argument is too 
 *	      large or too tiny
 *
 * Special cases:
 *	hypot(x,y) is INF if x or y is +INF or -INF; else
 *	hypot(x,y) is NAN if x or y is NAN.
 *
 * Accuracy:
 * 	hypot(x,y) returns sqrt(x^2+y^2) with error less 
 * 	than 1 ulps (units in the last place) 
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_hypot

double __ieee754_hypot(double x, double y)
{
	double a, b, t1, t2, y1, y2, w;
	int32_t j, k, ha, hb;

	GET_HIGH_WORD(ha, x);
	ha &= IC(0x7fffffff);
	GET_HIGH_WORD(hb, y);
	hb &= IC(0x7fffffff);
	if (hb > ha)
	{
		a = y;
		b = x;
		j = ha;
		ha = hb;
		hb = j;
	} else
	{
		a = x;
		b = y;
	}
	SET_HIGH_WORD(a, ha);				/* a <- |a| */
	SET_HIGH_WORD(b, hb);				/* b <- |b| */
	if ((ha - hb) > IC(0x3c00000))
	{
		return a + b;
	}									/* x/y > 2**60 */
	k = 0;
	if (ha > IC(0x5f300000))
	{									/* a>2**500 */
		if (ha >= IC(0x7ff00000))
		{								/* Inf or NaN */
			uint32_t low;

			w = a + b;					/* for sNaN */
			GET_LOW_WORD(low, a);
			if (((ha & IC(0xfffff)) | low) == 0)
				w = a;
			GET_LOW_WORD(low, b);
			if (((hb ^ IC(0x7ff00000)) | low) == 0)
				w = b;
			return w;
		}
		/* scale a and b by 2**-600 */
		ha -= IC(0x25800000);
		hb -= IC(0x25800000);
		k += 600;
		SET_HIGH_WORD(a, ha);
		SET_HIGH_WORD(b, hb);
	}
	if (hb < IC(0x20b00000))
	{									/* b < 2**-500 */
		if (hb <= IC(0x000fffff))
		{								/* subnormal b or 0 */
			uint32_t low;

			GET_LOW_WORD(low, b);
			if ((hb | low) == 0)
				return a;
			t1 = 0;
			SET_HIGH_WORD(t1, IC(0x7fd00000));	/* t1=2^1022 */
			b *= t1;
			a *= t1;
			k -= 1022;
		} else
		{								/* scale a and b by 2^600 */
			ha += IC(0x25800000);		/* a *= 2^600 */
			hb += IC(0x25800000);		/* b *= 2^600 */
			k -= 600;
			SET_HIGH_WORD(a, ha);
			SET_HIGH_WORD(b, hb);
		}
	}
	/* medium size a and b */
	w = a - b;
	if (w > b)
	{
		t1 = 0;
		SET_HIGH_WORD(t1, ha);
		t2 = a - t1;
		w = __ieee754_sqrt(t1 * t1 - (b * (-b) - t2 * (a + t1)));
	} else
	{
		a = a + a;
		y1 = 0;
		SET_HIGH_WORD(y1, hb);
		y2 = b - y1;
		t1 = 0;
		SET_HIGH_WORD(t1, ha + IC(0x00100000));
		t2 = a - t1;
		w = __ieee754_sqrt(t1 * y1 - (w * (-w) - (t1 * y2 + t2 * b)));
	}
	if (k != 0)
	{
		return __ieee754_scalbn(w, (int)k);
	}
	return w;
}
#endif

/* wrapper hypot(x,y) */
double __hypot(double x, double y)
{
	double z = __ieee754_hypot(x, y);

	if (!isfinite(z) && isfinite(x) && isfinite(y) && _LIB_VERSION != _IEEE_)
		return __kernel_standard(x, y, z, KMATHERR_HYPOT);	/* hypot overflow */

	return z;
}

__typeof(__hypot) hypot __attribute__((weak, alias("__hypot")));
#ifdef __NO_LONG_DOUBLE_MATH
long double __hypotl(long double x, long double y) __attribute__((alias("__hypot")));
__typeof(__hypotl) hypotl __attribute__((weak, alias("__hypot")));
#endif
