
/* @(#)w_scalb.c 1.3 95/01/18 */
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
 * wrapper scalb(double x, double fn) is provide for
 * passing various standard test suite. One 
 * should use scalbn() instead.
 */

#include "fdlibm.h"

#include <errno.h>

static double __attribute__ ((noinline)) sysv_scalb(double x, double fn)
{
	double z = __ieee754_scalb(x, fn);

	if (isinf(z))
	{
		if (isfinite(x))
			return __kernel_standard(x, fn, z, KMATHERR_SCALB_OVERFLOW);	/* scalb overflow */
		else
			__set_errno(ERANGE);
	} else if (z == 0.0 && z != x)
		return __kernel_standard(x, fn, z, KMATHERR_SCALB_UNDERFLOW);	/* scalb underflow */

	return z;
}


#ifdef _SCALB_INT
double scalb(double x, int fn)		/* wrapper scalb */
#else
double scalb(double x, double fn)	/* wrapper scalb */
#endif
{
	if (_LIB_VERSION == _SVID_)
	{
		return sysv_scalb(x, fn);
	} else
	{
		double z = __ieee754_scalb(x, fn);

		if (!isfinite(z) || z == 0.0)
		{
			if (isnan(z))
			{
				if (!isnan(x) && !isnan(fn))
					z = __kernel_standard(x, fn, z, KMATHERR_SCALB_INVALID);
			} else if (isinf(z))
			{
				if (!isinf(x) && !isinf(fn))
					z = __kernel_standard(x, fn, z, KMATHERR_SCALB_OVERFLOW);
			} else
			{
				/* z == 0.  */
				if (x != 0.0 && !isinf(fn))
					z = __kernel_standard(x, fn, z, KMATHERR_SCALB_UNDERFLOW);
			}
		}
		return z;
	}
}
