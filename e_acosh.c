/* @(#)e_acosh.c 1.3 95/01/18 */
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

/* __ieee754_acosh(x)
 * Method :
 *	Based on
 *		acosh(x) = log [ x + sqrt(x*x-1) ]
 *	we have
 *		acosh(x) := log(x)+ln2,	if x is large; else
 *		acosh(x) := log(2x-1/(sqrt(x*x-1)+x)) if x>2; else
 *		acosh(x) := log1p(t+sqrt(2.0*t+t*t)); where t=x-1.
 *
 * Special cases:
 *	acosh(x) is NaN with signal if x<1.
 *	acosh(NaN) is NaN without signal.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_acosh

double __ieee754_acosh(double x)
{
	double t;
	int32_t hx;
	uint32_t lx;

	static const double one = 1.0;
	static const double ln2 = 6.93147180559945286227e-01;	/* 0x3FE62E42, 0xFEFA39EF */

	GET_DOUBLE_WORDS(hx, lx, x);
	if (hx < IC(0x3ff00000))
	{									/* x < 1 */
		return (x - x) / (x - x);
	} else if (hx >= IC(0x41b00000))
	{									/* x > 2**28 */
		if (hx >= IC(0x7ff00000))
		{								/* x is inf of NaN */
			return x + x;
		}
		return __ieee754_log(x) + ln2;	/* acosh(huge)=log(2x) */
	} else if (((hx - IC(0x3ff00000)) | lx) == 0)
	{
		return 0.0;						/* acosh(1) = 0 */
	} else if (hx > IC(0x40000000))
	{									/* 2**28 > x > 2 */
		t = x * x;
		return __ieee754_log(2.0 * x - one / (x + __ieee754_sqrt(t - one)));
	} else
	{									/* 1<x<2 */
		t = x - one;
		return __ieee754_log1p(t + __ieee754_sqrt(2.0 * t + t * t));
	}
}

#endif

/* wrapper acosh */
double __acosh(double x)
{
	if (_LIB_VERSION != _IEEE_ && isless(x, 1.0))
		/* acosh(x<1) */
		return __kernel_standard(x, x, __builtin_nan(""), KMATHERR_ACOSH);

	return __ieee754_acosh(x);
}

__typeof(__acosh) acosh __attribute__((weak, alias("__acosh")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__acoshl) __acoshl __attribute__((alias("__acosh")));
__typeof(__acoshl) acoshl __attribute__((weak, alias("__acosh")));
#endif
