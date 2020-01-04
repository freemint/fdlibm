/* s_modff.c -- float version of s_modf.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_modf

float __ieee754_modff(float x, float *iptr)
{
	int32_t i0, j0;
	uint32_t i;

	static const float one = 1.0;

	GET_FLOAT_WORD(i0, x);
	j0 = ((i0 >> IEEE754_FLOAT_SHIFT) & IEEE754_FLOAT_MAXEXP) - IEEE754_FLOAT_BIAS;	/* exponent of x */
	if (j0 < IEEE754_FLOAT_SHIFT)
	{									/* integer part in x */
		if (j0 < 0)
		{								/* |x|<1 */
			SET_FLOAT_WORD(*iptr, i0 & IC(0x80000000));	/* *iptr = +-0 */
			return x;
		} else
		{
			i = IC(0x007fffff) >> j0;
			if ((i0 & i) == 0)
			{							/* x is integral */
				uint32_t ix;

				*iptr = x;
				GET_FLOAT_WORD(ix, x);
				SET_FLOAT_WORD(x, ix & IC(0x80000000));	/* return +-0 */
				return x;
			} else
			{
				SET_FLOAT_WORD(*iptr, i0 & (~i));
				return x - *iptr;
			}
		}
	} else
	{									/* no fraction part */
		*iptr = x * one;
		/* We must handle NaNs separately.  */
		if (j0 == (IEEE754_FLOAT_MAXEXP - IEEE754_FLOAT_BIAS) && (i0 & IC(0x7fffff)) != 0)
			return x * one;
		SET_FLOAT_WORD(x, i0 & IC(0x80000000));	/* return +-0 */
		return x;
	}
}

#endif

float __modff(float x, float *iptr)
{
	return __ieee754_modff(x, iptr);
}

__typeof(__modff) modff __attribute__((weak, alias("__modff")));
