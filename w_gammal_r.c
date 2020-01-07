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

/* double gamma(double x)
 * Return the logarithm of the Gamma function of x.
 *
 * Method: call __ieee754_gammal_r
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

long double __gammal_r(long double x, int *signgamp)
{
	long double y = __ieee754_lgammal_r(x, signgamp);

	if (!isfinite(y) && isfinite(x) && _LIB_VERSION != _IEEE_)
		return __kernel_standard_l(x, x, y, __ieee754_floorl(x) == x && x <= 0.0 ? KMATHERRL_LGAMMA_MINUS	/* lgamma pole */
								 : KMATHERRL_LGAMMA_OVERFLOW);	/* lgamma overflow */

	return y;
}

__typeof(__gammal_r) gammal_r __attribute__((weak, alias("__gammal_r")));

#endif
