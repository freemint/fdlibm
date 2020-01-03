/* @(#)s_fabs.c 1.3 95/01/18 */
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
 * fabs(x) returns the absolute value of x.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

double __fabs(double x)
{
#ifdef __mc68000__
	ieee_double_shape_type *sh_u = (ieee_double_shape_type *)&x;
	sh_u->parts.msw &= UC(0x7fffffff);
	return x;
#else
	uint32_t high;

	GET_HIGH_WORD(high, x);
	SET_HIGH_WORD(x, high & UC(0x7fffffff));
	return x;
#endif
}

__typeof(__fabs) fabs __attribute__((weak, alias("__fabs")));
#ifdef __NO_LONG_DOUBLE_MATH
long double __fabsl(long double x) __attribute__((alias("__fabs")));
__typeof(__fabsl) fabsl __attribute__((weak, alias("__fabs")));
#endif
