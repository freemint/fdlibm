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

/* float lgammaf_r(float x, int *signgamp)
 * Return the logarithm of the Gamma function of x.
 *
 * Method: call __ieee754_lgammaf_r
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

float __lgammaf_r(float x, int *signgamp)
{
	float y = __ieee754_lgammaf_r(x, signgamp);

	if (!isfinite(y) && isfinite(x) && _LIB_VERSION != _IEEE_)
		return __kernel_standard_f(x, x, y, __ieee754_floorf(x) == x && x <= 0.0F ? KMATHERRF_LGAMMA_MINUS	/* lgamma pole */
								 : KMATHERRF_LGAMMA_OVERFLOW);	/* lgamma overflow */

	return y;
}

__typeof(__lgammaf_r) lgammaf_r __attribute__((weak, alias("__lgammaf_r")));
