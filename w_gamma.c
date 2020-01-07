/* @(#)w_gamma.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 *
 */

/* double gamma(double x)
 * Return the logarithm of the Gamma function of x.
 *
 * Method: call gamma_r
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

double __gamma(double x)
{
	return gamma_r(x, &signgam);
}

__typeof(__gamma) gamma __attribute__((weak, alias("__gamma")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__gammal) __gammal __attribute__((alias("__gamma")));
__typeof(__gammal) gammal __attribute__((weak, alias("__gamma")));
#endif
