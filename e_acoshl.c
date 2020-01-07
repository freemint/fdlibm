/* e_acoshl.c -- long double version of e_acosh.c.
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

/* __ieee754_acoshl(x)
 * Method :
 *	Based on
 *		acoshl(x) = logl [ x + sqrtl(x*x-1) ]
 *	we have
 *		acoshl(x) := logl(x)+ln2,	if x is large; else
 *		acoshl(x) := logl(2x-1/(sqrtl(x*x-1)+x)) if x>2; else
 *		acoshl(x) := log1pl(t+sqrtl(2.0*t+t*t)); where t=x-1.
 *
 * Special cases:
 *	acoshl(x) is NaN with signal if x<1.
 *	acoshl(NaN) is NaN without signal.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_acosh

long double __ieee754_acoshl(long double x)
{
	long double t;
	uint32_t se, i0, i1;

	static const long double one = 1.0;
	static const long double ln2 = 6.931471805599453094287e-01L;	/* 0x3FFE, 0xB17217F7, 0xD1CF79AC */
	
	GET_LDOUBLE_WORDS(se, i0, i1, x);
	if (se < 0x3fff || (se & 0x8000))
	{									/* x < 1 */
		return (x - x) / (x - x);
	} else if (se >= 0x401d)
	{									/* x > 2**30 */
		if (se >= 0x7fff)
		{								/* x is inf of NaN */
			return x + x;
		} else
			return __ieee754_logl(x) + ln2;	/* acoshl(huge)=logl(2x) */
	} else if (((se - 0x3fff) | i0 | i1) == 0)
	{
		return 0.0;						/* acosh(1) = 0 */
	} else if (se > 0x4000)
	{									/* 2**28 > x > 2 */
		t = x * x;
		return __ieee754_logl(2.0 * x - one / (x + __ieee754_sqrtl(t - one)));
	} else
	{									/* 1<x<2 */
		t = x - one;
		return __ieee754_log1pl(t + __ieee754_sqrtl(2.0 * t + t * t));
	}
}

#endif

/* wrapper acosl */
long double __acoshl(long double x)
{
	if (isless(x, 1.0L) && _LIB_VERSION != _IEEE_)
		/* acosh(x<1) */
		return __kernel_standard_l(x, x, __builtin_nanl(""), KMATHERRL_ACOSH);

	return __ieee754_acoshl(x);
}

__typeof(__acoshl) acoshl __attribute__((weak, alias("__acoshl")));

#endif
