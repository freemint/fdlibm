/* @(#)w_gamma.c 5.1 93/09/24 */
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

/* double gamma(double x)
 * Return  the logarithm of the Gamma function of x or the Gamma function of x,
 * depending on the library mode.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

double __tgamma(double x)
{
	int local_signgam;
	double y = __ieee754_tgamma_r(x, &local_signgam);

	if (!isfinite(y) && (isfinite(x) || isinf(x) < 0) && _LIB_VERSION != _IEEE_)
	{
		if (x == 0.0)
			return __kernel_standard(x, x, y, KMATHERR_TGAMMA_ZERO);	/* tgamma pole */
		else if (__ieee754_floor(x) == x && x < 0.0)
			return __kernel_standard(x, x, y, KMATHERR_TGAMMA_MINUS);	/* tgamma domain */
		else if (y == 0)
			return __kernel_standard(x, x, y, KMATHERR_TGAMMA_UNDERFLOW);	/* tgamma underflow */
		else
			return __kernel_standard(x, x, y, KMATHERR_TGAMMA_OVERFLOW);	/* tgamma overflow */
	}
	return local_signgam < 0 ? -y : y;
}

__typeof(__tgamma) tgamma __attribute__((weak, alias("__tgamma")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__tgammal) __tgammal __attribute__((alias("__tgamma")));
__typeof(__tgammal) tgammal __attribute__((weak, alias("__tgamma")));
#endif
