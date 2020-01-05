/* s_finitel.c -- long double version of s_finite.c.
 * Conversion to IEEE quad long double by Jakub Jelinek, jj@ultra.linux.cz.
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
 * finitel(x) returns 1 is x is finite, else 0;
 * no branching!
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

int __finitel(long double x)
{
	uint32_t exp;

	GET_LDOUBLE_EXP(exp, x);
	return (int) (((exp & 0x7fff) - 0x7fff) >> 31);
}

__typeof(__finitel) finitel __attribute__((weak, alias("__finitel")));

#endif
