/* s_modfl.c -- long double version of s_modf.c.
 * Conversion to long double by Ulrich Drepper,
 * Cygnus Support, drepper@cygnus.com.
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

/*
 * modfl(long double x, long double *iptr)
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

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_modf

long double __ieee754_modfl(long double x, long double *iptr)
{
	int32_t i0, i1, j0;
	uint32_t i, se;

	static const long double one = 1.0;

	GET_LDOUBLE_WORDS(se, i0, i1, x);
	j0 = (se & IEEE854_LONG_DOUBLE_MAXEXP) - IEEE854_LONG_DOUBLE_BIAS;		/* exponent of x */
	if (j0 < 32)
	{									/* integer part in high x */
		if (j0 < 0)
		{								/* |x|<1 */
			SET_LDOUBLE_WORDS(*iptr, se & 0x8000, 0, 0);	/* *iptr = +-0 */
			return x;
		} else
		{
			i = IC(0x7fffffff) >> j0;
			if (((i0 & i) | i1) == 0)
			{							/* x is integral */
				*iptr = x;
				SET_LDOUBLE_WORDS(x, se & 0x8000, 0, 0);	/* return +-0 */
				return x;
			} else
			{
				SET_LDOUBLE_WORDS(*iptr, se, i0 & (~i), 0);
				return x - *iptr;
			}
		}
	} else if (j0 > 63)
	{									/* no fraction part */
		*iptr = x * one;
		/* We must handle NaNs separately.  */
		if (j0 == (IEEE854_LONG_DOUBLE_MAXEXP - IEEE854_LONG_DOUBLE_BIAS) && ((i0 & IC(0x7fffffff)) | i1) != 0)
			return x * one;
		SET_LDOUBLE_WORDS(x, se & 0x8000, 0, 0);	/* return +-0 */
		return x;
	} else
	{									/* fraction part in low x */
		i = UC(0x7fffffff) >> (j0 - 32);
		if ((i1 & i) == 0)
		{								/* x is integral */
			*iptr = x;
			SET_LDOUBLE_WORDS(x, se & 0x8000, 0, 0);	/* return +-0 */
			return x;
		} else
		{
			SET_LDOUBLE_WORDS(*iptr, se, i0, i1 & (~i));
			return x - *iptr;
		}
	}
}

#endif

long double __modfl(long double x, long double *iptr)
{
	return __ieee754_modfl(x, iptr);
}

__typeof(__modfl) modfl __attribute__((weak, alias("__modfl")));

#endif
