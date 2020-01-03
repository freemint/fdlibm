/* @(#)s_ceil.c 1.3 95/01/18 */
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
 * ceil(x)
 * Return x rounded toward -inf to integral value
 * Method:
 *	Bit twiddling.
 * Exception:
 *	Inexact flag raised if x not equal to ceil(x).
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_ceil

double __ieee754_ceil(double x)
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
			math_force_eval(hugeval + x);
			/* return 0*sign(x) if |x|<1 */
			if (i0 < 0)
			{
				i0 = IC(0x80000000);
				i1 = 0;
			} else if ((i0 | i1) != 0)
			{
				i0 = UC(0x3ff00000);
				i1 = 0;
			}
		} else
		{
			i = UC(0x000fffff) >> j0;
			if (((i0 & i) | i1) == 0)
				return x;				/* x is integral */
			math_force_eval(hugeval + x);	/* raise inexact flag */
			if (i0 > 0)
				i0 += IC(0x00100000) >> j0;
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
		if (i0 > 0)
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

double __ceil(double x)
{
	return __ieee754_ceil(x);
}

__typeof(__ceil) ceil __attribute__((weak, alias("__ceil")));
#ifdef __NO_LONG_DOUBLE_MATH
long double __ceill(long double x) __attribute__((alias("__ceil")));
__typeof(__ceill) ceill __attribute__((weak, alias("__ceil")));
#endif
