/* @(#)s_nextafter.c 1.3 95/01/18 */
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

/* IEEE functions
 *	nextafter(x,y)
 *	return the next machine floating-point number of x in the
 *	direction toward y.
 *   Special cases:
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

double __nextafter(double x, double y)
{
	int32_t hx, hy, ix, iy;
	uint32_t lx, ly;

	GET_DOUBLE_WORDS(hx, lx, x);
	GET_DOUBLE_WORDS(hy, ly, y);
	ix = hx & IC(0x7fffffff);				/* |x| */
	iy = hy & IC(0x7fffffff);				/* |y| */

	if (((ix >= IC(0x7ff00000)) && ((ix - IC(0x7ff00000)) | lx) != 0) ||	/* x is nan */
		((iy >= IC(0x7ff00000)) && ((iy - IC(0x7ff00000)) | ly) != 0))	/* y is nan */
		return x + y;
	if (x == y)
		return x;						/* x=y, return x */
	if ((ix | lx) == 0)
	{									/* x == 0 */
		double u;

		INSERT_WORDS(x, hy & UC(0x80000000), 1);	/* return +-minsubnormal */
		u = math_opt_barrier(x);
		u = u * u;
		math_force_eval(u);				/* raise underflow flag */
		return x;
	}
	if (hx >= 0)
	{									/* x > 0 */
		if (hx > hy || ((hx == hy) && (lx > ly)))
		{								/* x > y, x -= ulp */
			if (lx == 0)
				hx -= 1;
			lx -= 1;
		} else
		{								/* x < y, x += ulp */
			lx += 1;
			if (lx == 0)
				hx += 1;
		}
	} else
	{									/* x < 0 */
		if (hy >= 0 || hx > hy || ((hx == hy) && (lx > ly)))
		{								/* x < y, x -= ulp */
			if (lx == 0)
				hx -= 1;
			lx -= 1;
		} else
		{								/* x > y, x += ulp */
			lx += 1;
			if (lx == 0)
				hx += 1;
		}
	}
	hy = hx & IC(0x7ff00000);
	if (hy >= IC(0x7ff00000))
	{
		x = x + x;						/* overflow  */
		math_force_eval(x);
		return x;						/* overflow  */
	}
	if (hy < IC(0x00100000))
	{
		double u = x * x;				/* underflow */
		math_force_eval(u);				/* raise underflow flag */
	}
	INSERT_WORDS(x, hx, lx);
	return x;
}


__typeof(__nextafter) nextafter __attribute__((weak, alias("__nextafter")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(nextafterl) __nextafterl __attribute__((alias("__nextafter")));
__typeof(__nextafterl) nextafterl __attribute__((weak, alias("__nextafter")));
__typeof(__nexttoward) __nexttoward __attribute__((alias("__nextafter")));
__typeof(nexttoward) nexttoward __attribute__((weak, alias("__nextafter")));
__typeof(__nexttowardl) __nexttowardl __attribute__((alias("__nextafterl")));
__typeof(nexttowardl) nexttowardl __attribute__((weak, alias("__nextafterl")));
#endif
