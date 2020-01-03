/* @(#)e_scalb.c 1.3 95/01/18 */
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

#include "fdlibm.h"

double __ieee754_scalb(double x, double fn)
{
	long ifn;
	
	if (isnan(x))
		return x * fn;
	if (!isfinite(fn))
	{
		if (isnan(fn) || fn > 0.0)
			return x * fn;
		if (x == 0.0)
			return x;
		return x / -fn;
	}
	ifn = (long) fn;
	if ((double) ifn != fn)
	{
		if (__ieee754_rint(fn) != fn)
		{
			feraiseexcept(FE_INVALID);
			return __builtin_nan("");
		}
		if (ifn > 65000l)
			return __ieee754_scalbln(x, 65000l);
		return __ieee754_scalbln(x, -65000l);
	}
	
	return __ieee754_scalbln(x, ifn);
}


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


/*
 * wrapper scalb(double x, double fn) is provide for
 * passing various standard test suite. One 
 * should use scalbn() instead.
 */

double __scalb(double x, double fn)
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

__typeof(__scalb) scalb __attribute__((weak, alias("__scalb")));
#ifdef __NO_LONG_DOUBLE_MATH
long double __scalbl(long double x, long double fn) __attribute__((alias("__scalb")));
__typeof(__scalbl) scalbl __attribute__((weak, alias("__scalb")));
#endif
