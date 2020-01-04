/* @(#)s_isnan.c 1.3 95/01/18 */
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
 * isnan(x) returns 1 is x is nan, else 0;
 * no branching!
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

int __isnan(double x)
{
	int32_t hx, lx;

	GET_DOUBLE_WORDS(hx, lx, x);
	hx &= IC(0x7fffffff);
	hx |= (uint32_t) (lx | (-lx)) >> 31;
	hx = IC(0x7ff00000) - hx;
	return (int) (((uint32_t) hx) >> 31);
}

__typeof(__isnan) isnan __attribute__((weak, alias("__isnan")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__isnanl) __isnanl __attribute__((alias("__isnan")));
__typeof(__isnanl) isnanl __attribute__((weak, alias("__isnan")));
#endif
