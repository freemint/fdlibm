/* @(#)s_rint.c 1.3 95/01/18 */
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
 * rint(x)
 * Return x rounded to integral value according to the prevailing
 * rounding mode.
 * Method:
 *	Using floating addition.
 * Exception:
 *	Inexact flag raised if x not equal to rint(x).
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_rint

static double __ieee754_roundeven(double x)
{
	int32_t i0, j0;
	uint32_t i1;

	static const double hugeval = 1.0e300;
	
	GET_DOUBLE_WORDS(i0, i1, x);
	j0 = ((i0 >> IEEE754_DOUBLE_SHIFT) & IEEE754_DOUBLE_MAXEXP) - IEEE754_DOUBLE_BIAS;
	if (j0 < IEEE754_DOUBLE_SHIFT)
	{
		if (j0 < 0)
		{
			math_force_eval(hugeval + x);
			if (j0 == -1 && (i0 & IC(0x000fffff)) != 0)
			{
				i0 &= UC(0x80000000);
				i0 |= UC(0x3ff00000);
			} else
			{
				i0 &= UC(0x80000000);
			}
			i1 = 0;
		} else
		{
			uint32_t i = UC(0x000fffff) >> j0;

			if (((i0 & i) | i1) == 0)
				/* X is integral.  */
				return x;

			/* Raise inexact if x != 0.  */
			math_force_eval(hugeval + x);

			if ((i0 & (i >> 1)) != 0 || i1 != 0 || (i0 & (UC(0x00100000) >> j0)))
			{
				i0 += UC(0x00080000) >> j0;
			}
			i0 &= ~i;
			i1 = 0;
		}
	} else if (j0 > 51)
	{
		if (j0 == (IEEE754_DOUBLE_MAXEXP - IEEE754_DOUBLE_BIAS))
			return x + x;				/* Inf or NaN */
		else
			return x;					/* x is integral */
	} else
	{
		uint32_t i = UC(0xffffffff) >> (j0 - IEEE754_DOUBLE_SHIFT);
		uint32_t j;

		if ((i1 & i) == 0)
			return x;					/* x is integral */

		/* Raise inexact if x != 0. */
		math_force_eval(hugeval + x);

		if ((i1 & (i >> 1)) != 0 || (j0 == IEEE754_DOUBLE_SHIFT && (i0 & 1)) || (i1 & (UC(1) << (52 - j0))))
		{
			j = i1 + (UC(1) << (51 - j0));
			if (j < i1)
				i0 += 1;
			i1 = j;
		}
		i1 &= ~i;
	}

	INSERT_WORDS(x, i0, i1);
	return x;
}

double __ieee754_rint(double x)
{
	/*
	 * above code relies on a FPU doing the rounding,
	 * and using round-to-even for FE_TONEAREST
	 */
	switch (fegetround())
	{
		case FE_UPWARD: return __ieee754_ceil(x);
		case FE_DOWNWARD: return __ieee754_floor(x);
		case FE_TOWARDZERO: return __ieee754_trunc(x);
	}
	/* case FE_TONEAREST: */
	return __ieee754_roundeven(x);
}

#endif

double __rint(double x)
{
	return __ieee754_rint(x);
}


__typeof(__rint) rint __attribute__((weak, alias("__rint")));
#ifdef __NO_LONG_DOUBLE_MATH
long double __rintl(long double x) __attribute__((alias("__rint")));
__typeof(__rintl) rintl __attribute__((weak, alias("__rint")));
#endif
