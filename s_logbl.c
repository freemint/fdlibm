/* s_logbl.c -- long double version of s_logb.c.
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
 * long double logbl(x)
 * IEEE 754 logb. Included to pass IEEE test suite. Not recommend.
 * Use ilogb instead.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

long double __logbl(long double x)
{
	int32_t es, lx, ix;

	GET_LDOUBLE_WORDS(es, ix, lx, x);
	es &= IEEE854_LONG_DOUBLE_MAXEXP;						/* exponent */
	if ((es | ix | lx) == 0)
		return -1.0 / __ieee754_fabsl(x);
	if (es == IEEE854_LONG_DOUBLE_MAXEXP)
		return x * x;
	if (es == 0)						/* IEEE 754 logb */
	{
		/* POSIX specifies that denormal number is treated as
		   though it were normalized.  */
		int32_t ma;

		if (ix == 0)
			ma = count_leading_zeros(lx) + 32;
		else
			ma = count_leading_zeros(ix);
		es -= ma - 1;
	}
	return (long double) (es - IEEE854_LONG_DOUBLE_BIAS);
}

__typeof(__logbl) logbl __attribute__((weak, alias("__logbl")));

#endif
