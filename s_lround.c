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
<<lround>>, <<lroundf>>, <<llround>>, <<llroundf>>--round to integer, to nearest
INDEX
	lround
INDEX
	lroundf
INDEX
	llround
INDEX
	llroundf

ANSI_SYNOPSIS
	#include <math.h>
	long int lround(double <[x]>);
	long int lroundf(float <[x]>);
	long long int llround(double <[x]>);
	long long int llroundf(float <[x]>);

DESCRIPTION
	The <<lround>> and <<llround>> functions round their argument to the
	nearest integer value, rounding halfway cases away from zero, regardless
	of the current rounding direction.  If the rounded value is outside the
	range of the return type, the numeric result is unspecified (depending
	upon the floating-point implementation, not the library).  A range
	error may occur if the magnitude of x is too large.

RETURNS
<[x]> rounded to an integral value as an integer.

SEEALSO
See the <<round>> functions for the return being the same floating-point type
as the argument.  <<lrint>>, <<llrint>>.

PORTABILITY
ANSI C, POSIX

*/

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_lround

long int __ieee754_lround(double x)
{
	int32_t j0;
	uint32_t i1, i0;
	long int result;
	int32_t sign;

	GET_DOUBLE_WORDS(i0, i1, x);
	/* Extract exponent field. */
	j0 = ((i0 >> IEEE754_DOUBLE_SHIFT) & IEEE754_DOUBLE_MAXEXP) - IEEE754_DOUBLE_BIAS;
	/* Extract sign. */
	sign = (i0 & IC(0x80000000)) != 0 ? -1 : 1;
	i0 &= UC(0x000fffff);
	i0 |= UC(0x00100000);

	/* j0 in [-1023,1024] */
	if (j0 < IEEE754_DOUBLE_SHIFT)
	{
		/* j0 in [-1023,19] */
		if (j0 < 0)
		{
			return j0 < -1 ? 0 : sign;
		} else
		{
			/* j0 in [0,19] */

			/* shift amt in [0,19] */
			i0 += UC(0x80000) >> j0;
			/* shift amt in [20,1] */
			result = i0 >> (IEEE754_DOUBLE_SHIFT - j0);
		}
	} else if (j0 < (int32_t) (8 * sizeof(long int)) - 1)
	{
		/* 32bit long: j0 in [20,30] */
		/* 64bit long: j0 in [20,62] */
		if (j0 >= 52)
		{
			/* 64bit long: j0 in [52,62] */
			/* 64bit long: shift amt in [32,42] */
			if ((j0 - IEEE754_DOUBLE_SHIFT) >= (int32_t) (8 * sizeof(long int)))
				result = 0;
			else
				result = (long int) i0 << (j0 - IEEE754_DOUBLE_SHIFT);
			if ((j0 - 52) < 32)
				result |= (unsigned long int)i1 << (j0 - 52);
		} else
		{
			/* 32bit long: j0 in [20,30] */
			/* 64bit long: j0 in [20,51] */
			uint32_t j = i1 + (UC(0x80000000) >> (j0 - IEEE754_DOUBLE_SHIFT));

			if (j < i1)
				++i0;

			if (j0 == IEEE754_DOUBLE_SHIFT)
				result = (long int) i0;
			else
				result = ((long int) i0 << (j0 - IEEE754_DOUBLE_SHIFT)) | (j >> (52 - j0));
		}
	} else
	{
		/* The number is too large.  It is left implementation defined
		   what happens.  */
		return (long int) x;
	}

	if (sign < 0)
		result = -result;
	return result;
}

#endif

long int __lround(double x)
{
	return __ieee754_lround(x);
}

__typeof(__lround) lround __attribute__((weak, alias("__lround")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__lroundl) __lroundl __attribute__((alias("__lround")));
__typeof(__lroundl) lroundl __attribute__((weak, alias("__lround")));
#endif
