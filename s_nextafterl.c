/* s_nextafterl.c -- long double version of s_nextafter.c.
 * Conversion to long double by Ulrich Drepper,
 * Cygnus Support, drepper@cygnus.com.
 * Fixed for m68k by Andreas Schwab <schwab@suse.de>.
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

/* IEEE functions
 *	nextafterl(x,y)
 *	return the next machine floating-point number of x in the
 *	direction toward y.
 *   Special cases:
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

long double __nextafterl(long double x, long double y)
{
	int32_t ix, iy;
	int32_t esx, esy;
	uint32_t hx, hy, lx, ly;

	GET_LDOUBLE_WORDS(esx, hx, lx, x);
	GET_LDOUBLE_WORDS(esy, hy, ly, y);
	ix = esx & IEEE854_LONG_DOUBLE_MAXEXP;					/* |x| */
	iy = esy & IEEE854_LONG_DOUBLE_MAXEXP;					/* |y| */

	/* Intel's extended format has the normally implicit 1 explicit
	   present.  Sigh!  */
	if (((ix == IEEE854_LONG_DOUBLE_MAXEXP) && (((hx & IC(0x7fffffff)) | lx) != 0)) ||	/* x is nan */
		((iy == IEEE854_LONG_DOUBLE_MAXEXP) && (((hy & IC(0x7fffffff)) | ly) != 0)))	/* y is nan */
		return x + y;
	if (x == y)
		return y;						/* x=y, return y */
	if ((ix | hx | lx) == 0)
	{									/* x == 0 */
		long double u;

		SET_LDOUBLE_WORDS(x, esy & 0x8000, 0, 1);	/* return +-minsubnormal */
		u = math_opt_barrier(x);
		u = u * u;
		math_force_eval(u);				/* raise underflow flag */
		return x;
	}
	if (esx >= 0)
	{									/* x > 0 */
		if (esx > esy || ((esx == esy) && (hx > hy || ((hx == hy) && (lx > ly)))))
		{
			/* x > y, x -= ulp */
			if (lx == 0)
			{
				if (hx <= UC(0x80000000))
				{
					if (esx == 0)
					{
						--hx;
					} else
					{
						esx -= 1;
						hx = hx - 1;
						if (esx > 0)
							hx |= UC(0x80000000);
					}
				} else
					hx -= 1;
			}
			lx -= 1;
		} else
		{								/* x < y, x += ulp */
			lx += 1;
			if (lx == 0)
			{
				hx += 1;
				if (hx == 0 || (esx == 0 && hx == UC(0x80000000)))
				{
					esx += 1;
					hx |= UC(0x80000000);
				}
			}
		}
	} else
	{									/* x < 0 */
		if (esy >= 0 || (esx > esy || ((esx == esy) && (hx > hy || ((hx == hy) && (lx > ly))))))
		{
			/* x < y, x -= ulp */
			if (lx == 0)
			{
				if (hx <= UC(0x80000000))
				{
					esx -= 1;
					hx = hx - 1;
					if ((esx & IEEE854_LONG_DOUBLE_MAXEXP) > 0)
						hx |= UC(0x80000000);
				} else
					hx -= 1;
			}
			lx -= 1;
		} else
		{								/* x > y, x += ulp */
			lx += 1;
			if (lx == 0)
			{
				hx += 1;
				if (hx == 0 || (esx == IC(0xffff8000) && hx == UC(0x80000000)))
				{
					esx += 1;
					hx |= UC(0x80000000);
				}
			}
		}
	}
	esy = esx & IEEE854_LONG_DOUBLE_MAXEXP;
	if (esy == IEEE854_LONG_DOUBLE_MAXEXP)
		return x + x;					/* overflow  */
	if (esy == 0)
	{
		long double u = x * x;			/* underflow */
		math_force_eval(u);				/* raise underflow flag */
	}
	SET_LDOUBLE_WORDS(x, esx, hx, lx);
	return x;
}

__typeof(__nextafterl) nextafterl __attribute__((weak, alias("__nextafterl")));

#endif
