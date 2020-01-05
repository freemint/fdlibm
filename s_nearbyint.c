/* Adapted for use as nearbyint by Ulrich Drepper <drepper@cygnus.com>.  */
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

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

/*
 * nearbyint(x)
 * Return x rounded to integral value according to the prevailing
 * rounding mode.
 * Method:
 *	Using floating addition.
 * Exception:
 *	none
 */

double __nearbyint(double x)
{
	fenv_t env;
	double value;

	feholdexcept(&env);
	value = __ieee754_rint(x);
	fesetenv(&env);
	return value;
}

__typeof(__nearbyint) nearbyint __attribute__((weak, alias("__nearbyint")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(nearbyintl) __nearbyintl __attribute__((alias("__nearbyint")));
__typeof(__nearbyintl) nearbyintl __attribute__((weak, alias("__nearbyint")));
#endif
