/* w_gammal.c -- long double version of w_gamma.c.
 * Conversion to long double by Ulrich Drepper,
 * Cygnus Support, drepper@cygnus.com.
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

/* long double gammal(double x)
 * Return the Gamma function of x.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

long double __tgammal(long double x)
{
	int local_signgam;
	long double y = __ieee754_tgammal_r(x, &local_signgam);

	if (!isfinite(y) && (isfinite(x) || isinf(x) < 0) && _LIB_VERSION != _IEEE_)
	{
		if (x == 0.0)
			return __kernel_standard_l(x, x, y, KMATHERRL_TGAMMA_ZERO);	/* tgamma pole */
		else if (__ieee754_floorl(x) == x && x < 0.0L)
			return __kernel_standard_l(x, x, y, KMATHERRL_TGAMMA_MINUS);	/* tgamma domain */
		else if (y == 0)
			return __kernel_standard_l(x, x, y, KMATHERRL_TGAMMA_UNDERFLOW);	/* tgamma underflow */
		else
			return __kernel_standard_l(x, x, y, KMATHERRL_TGAMMA_OVERFLOW);	/* tgamma overflow */
	}
	return local_signgam < 0 ? -y : y;
}

__typeof(__tgammal) tgammal __attribute__((weak, alias("__tgammal")));

#endif
