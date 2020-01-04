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
/*
FUNCTION
<<round>>, <<roundf>>--round to integer, to nearest
INDEX
	round
INDEX
	roundf

ANSI_SYNOPSIS
	#include <math.h>
	double round(double <[x]>);
	float roundf(float <[x]>);

DESCRIPTION
	The <<round>> functions round their argument to the nearest integer
	value in floating-point format, rounding halfway cases away from zero,
	regardless of the current rounding direction.  (While the "inexact"
	floating-point exception behavior is unspecified by the C standard, the
	<<round>> functions are written so that "inexact" is not raised if the
	result does not equal the argument, which behavior is as recommended by
	IEEE 754 for its related functions.)

RETURNS
<[x]> rounded to an integral value.

PORTABILITY
ANSI C, POSIX

SEEALSO
<<nearbyint>>, <<rint>>

*/

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_round

double __ieee754_round(double x)
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
			i0 &= UC(0x80000000);
			if (j0 == -1)				/* Result is +1.0 or -1.0. */
				i0 |= UC(0x3ff00000);
			i1 = 0;
		} else
		{
			uint32_t i = UC(0x000fffff) >> j0;

			if (((i0 & i) | i1) == 0)
				/* X is integral.  */
				return x;

			/* Raise inexact if x != 0.  */
			math_force_eval(hugeval + x);

			i0 += UC(0x00080000) >> j0;
			i0 &= ~i;
			i1 = 0;
		}
	} else if (j0 > 51)
	{
		if (j0 == (IEEE754_DOUBLE_MAXEXP - IEEE754_DOUBLE_BIAS))
			/* Inf or NaN.  */
			return x + x;
		else
			return x;
	} else
	{
		uint32_t i = UC(0xffffffff) >> (j0 - IEEE754_DOUBLE_SHIFT);
		uint32_t j;

		if ((i1 & i) == 0)
			/* X is integral.  */
			return x;

		/* Raise inexact if x != 0.  */
		math_force_eval(hugeval + x);

		j = i1 + (UC(1) << (51 - j0));

		if (j < i1)
			i0 += 1;
		i1 = j;
		i1 &= ~i;
	}

	INSERT_WORDS(x, i0, i1);

	return x;
}

#endif

double __round(double x)
{
	return __ieee754_round(x);
}

__typeof(__round) round __attribute__((weak, alias("__round")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__roundl) __roundl __attribute__((alias("__round")));
__typeof(__roundl) roundl __attribute__((weak, alias("__round")));
#endif
