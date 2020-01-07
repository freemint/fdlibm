/* @(#)w_lgamma.c 1.3 95/01/18 */
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

/* double lgamma(double x)
 * Return the logarithm of the Gamma function of x.
 *
 * Method: call lgamma_r
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

double __lgamma(double x)
{
	return lgamma_r(x, &signgam);
}             

__typeof(__lgamma) lgamma __attribute__((weak, alias("__lgamma")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__lgammal) __lgammal __attribute__((alias("__lgamma")));
__typeof(__lgammal) lgammal __attribute__((weak, alias("__lgamma")));
#endif
