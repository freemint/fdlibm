/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 *
 */

/* long double gammal(long double x)
 * Return the logarithm of the Gamma function of x.
 *
 * Method: call gammal_r
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

long double __gammal(long double x)
{
	return gammal_r(x, &signgam);
}

__typeof(__gammal) gammal __attribute__((weak, alias("__gammal")));

#endif
