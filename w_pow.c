/* @(#)w_pow.c 1.3 95/01/18 */
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
 * wrapper pow(x,y) return x**y
 */

#include "fdlibm.h"


/* wrapper pow */
double pow(double x, double y)	/* wrapper pow */
{
	double z = __ieee754_pow(x, y);

	if (!isfinite(z))
	{
		if (_LIB_VERSION != _IEEE_)
		{
			if (isnan(x))
			{
				if (y == 0.0)
					/* pow(NaN,0.0) */
					return __kernel_standard(x, y, z, KMATHERR_POW_NAN);
			} else if (isfinite(x) && isfinite(y))
			{
				if (isnan(z))
				{
					/* pow neg**non-int */
					return __kernel_standard(x, y, z, KMATHERR_POW_NONINT);
				} else if (x == 0.0 && y < 0.0)
				{
					if (signbit(x) && signbit(z))
						/* pow(-0.0,negative) */
						return __kernel_standard(x, y, z, KMATHERR_POW_MINUS);
					else
						/* pow(+0.0,negative) */
						return __kernel_standard(x, y, z, KMATHERR_POW_ZEROMINUS);
				} else
				{
					/* pow overflow */
					return __kernel_standard(x, y, z, KMATHERR_POW_OVERFLOW);
				}
			}
		}
	} else if (z == 0.0 && isfinite(x) && isfinite(y) && _LIB_VERSION != _IEEE_)
	{
		if (x == 0.0)
		{
			if (y == 0.0)
				/* pow(0.0,0.0) */
				return __kernel_standard(x, y, z, KMATHERR_POW_ZERO);
		} else
		{
			/* pow underflow */
			return __kernel_standard(x, y, z, KMATHERR_POW_UNDERFLOW);
		}
	}

	return z;
}
