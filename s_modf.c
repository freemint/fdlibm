/* @(#)s_modf.c 1.3 95/01/18 */
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
 * modf(double x, double *iptr) 
 * return fraction part of x, and return x's integral part in *iptr.
 * Method:
 *	Bit twiddling.
 *
 * Exception:
 *	No exception.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_modf

double __ieee754_modf(double x, double *iptr)
{
	int32_t i0, i1, j0;
	uint32_t i;

	static const double one = 1.0;

	GET_DOUBLE_WORDS(i0, i1, x);
	j0 = ((i0 >> IEEE754_DOUBLE_SHIFT) & IEEE754_DOUBLE_MAXEXP) - IEEE754_DOUBLE_BIAS;	/* exponent of x */
	if (j0 < IEEE754_DOUBLE_SHIFT)
	{									/* integer part in high x */
		if (j0 < 0)
		{								/* |x|<1 */
			INSERT_WORDS(*iptr, i0 & UC(0x80000000), 0);	/* *iptr = +-0 */
			return x;
		} else
		{
			i = UC(0x000fffff) >> j0;
			if (((i0 & i) | i1) == 0)
			{							/* x is integral */
				*iptr = x;
				INSERT_WORDS(x, i0 & UC(0x80000000), 0);	/* return +-0 */
				return x;
			} else
			{
				INSERT_WORDS(*iptr, i0 & (~i), 0);
				return x - *iptr;
			}
		}
	} else if (j0 > 51)
	{									/* no fraction part */
		*iptr = x * one;
		/* We must handle NaNs separately.  */
		if (j0 == (IEEE754_DOUBLE_MAXEXP - IEEE754_DOUBLE_BIAS) && ((i0 & UC(0xfffff)) | i1) != 0)
			return x * one;
		INSERT_WORDS(x, i0 & UC(0x80000000), 0);	/* return +-0 */
		return x;
	} else
	{									/* fraction part in low x */
		i = UC(0xffffffff) >> (j0 - IEEE754_DOUBLE_SHIFT);
		if ((i1 & i) == 0)
		{								/* x is integral */
			*iptr = x;
			INSERT_WORDS(x, i0 & UC(0x80000000), 0);	/* return +-0 */
			return x;
		} else
		{
			INSERT_WORDS(*iptr, i0, i1 & (~i));
			return x - *iptr;
		}
	}
}

#endif

double __modf(double x, double *iptr)
{
	return __ieee754_modf(x, iptr);
}


__typeof(__modf) modf __attribute__((weak, alias("__modf")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__modfl) __modfl __attribute__((alias("__modf")));
__typeof(__modfl) modfl __attribute__((weak, alias("__modf")));
#endif
