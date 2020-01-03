/* @(#)s_floor.c 1.3 95/01/18 */
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

/*
 * floor(x)
 * Return x rounded toward -inf to integral value
 * Method:
 *	Bit twiddling.
 * Exception:
 *	Inexact flag raised if x not equal to floor(x).
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_floor

double __ieee754_floor(double x)
{
	int32_t i0, j0;
	uint32_t i, j, i1;

	static const double hugeval = 1.0e300;
	
	GET_DOUBLE_WORDS(i0, i1, x);
	j0 = ((i0 >> IEEE754_DOUBLE_SHIFT) & IEEE754_DOUBLE_MAXEXP) - IEEE754_DOUBLE_BIAS;
	if (j0 < IEEE754_DOUBLE_SHIFT)
	{
		if (j0 < 0)
		{								/* raise inexact if x != 0 */
			math_force_eval(hugeval + x);	/* return 0*sign(x) if |x|<1 */
			if (i0 >= 0)
			{
				i0 = i1 = 0;
			} else if (((i0 & IC(0x7fffffff)) | i1) != 0)
			{
				i0 = IC(0xbff00000);
				i1 = 0;
			}
		} else
		{
			i = UC(0x000fffff) >> j0;
			if (((i0 & i) | i1) == 0)
				return x;				/* x is integral */
			math_force_eval(hugeval + x);	/* raise inexact flag */
			if (i0 < 0)
				i0 += UC(0x00100000) >> j0;
			i0 &= (~i);
			i1 = 0;
		}
	} else if (j0 > 51)
	{
		if (j0 == (IEEE754_DOUBLE_MAXEXP - IEEE754_DOUBLE_BIAS))
			return x + x;				/* inf or NaN */
		else
			return x;					/* x is integral */
	} else
	{
		i = UC(0xffffffff) >> (j0 - IEEE754_DOUBLE_SHIFT);
		if ((i1 & i) == 0)
			return x;					/* x is integral */
		math_force_eval(hugeval + x);		/* raise inexact flag */
		if (i0 < 0)
		{
			if (j0 == IEEE754_DOUBLE_SHIFT)
				i0 += 1;
			else
			{
				j = i1 + (UC(1) << (52 - j0));
				if (j < i1)
					i0 += 1;			/* got a carry */
				i1 = j;
			}
		}
		i1 &= (~i);
	}
	INSERT_WORDS(x, i0, i1);
	return x;
}

#endif

double __floor(double x)
{
	return __ieee754_floor(x);
}

__typeof(__floor) floor __attribute__((weak, alias("__floor")));
#ifdef __NO_LONG_DOUBLE_MATH
long double __floorl(long double x) __attribute__((alias("__floor")));
__typeof(__floorl) floorl __attribute__((weak, alias("__floor")));
#endif
