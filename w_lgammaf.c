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

/* float lgammaf(double x)
 * Return the logarithm of the Gamma function of x.
 *
 * Method: call lgammaf_r
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

float __lgammaf(float x)
{
	return lgammaf_r(x, &signgam);
}

__typeof(__lgammaf) lgammaf __attribute__((weak, alias("__lgammaf")));
