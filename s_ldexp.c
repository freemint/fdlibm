/* @(#)s_ldexp.c 1.3 95/01/18 */
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

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_ldexp

double __ieee754_ldexp(double value, int exp)
{
	if (!isfinite(value) || value == 0.0)
		return value;
	return __ieee754_scalbn(value, exp);
}

#endif

double __ldexp(double x, int n)
{
	x = __ieee754_ldexp(x, n);
	if (!isfinite(x) || x == 0.0)
		__set_errno(ERANGE);
	return x;
}

__typeof(__ldexp) ldexp __attribute__((weak, alias("__ldexp")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__ldexpl) __ldexpl __attribute__((alias("__ldexp")));
__typeof(__ldexpl) ldexpl __attribute__((weak, alias("__ldexp")));
#endif
