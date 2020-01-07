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

/* float gammaf(double x)
 * Return the logarithm of the Gamma function of x.
 *
 * Method: call gammaf_r
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

float __gammaf(float x)
{
	return gammaf_r(x, &signgam);
}

__typeof(__gammaf) gammaf __attribute__((weak, alias("__gammaf")));
