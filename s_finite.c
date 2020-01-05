/* @(#)s_finite.c 1.3 95/01/18 */
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
 * finite(x) returns 1 is x is finite, else 0;
 * no branching!
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

int __finite(double x)
{
	uint32_t hx;

	GET_HIGH_WORD(hx, x);
	return (int) (((hx & UC(0x7fffffff)) - UC(0x7ff00000)) >> 31);
}

__typeof(__finite) finite __attribute__((weak, alias("__finite")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(finitel) __finitel __attribute__((alias("__finite")));
__typeof(__finitel) finitel __attribute__((weak, alias("__finite")));
#endif
