/* s_nearbyintl.c -- long double version of s_nearbyint.c.
 * Conversion to long double by Ulrich Drepper,
 * Cygnus Support, drepper@cygnus.com.
 */
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

/*
 * nearbyintl(x)
 * Return x rounded to integral value according to the prevailing
 * rounding mode.
 * Method:
 *	Using floating addition.
 * Exception:
 *	none
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

long double __nearbyintl(long double x)
{
	fenv_t env;
	long double value;

	feholdexcept(&env);
	value = __ieee754_rintl(x);
	fesetenv(&env);
	return value;
}

__typeof(__nearbyintl) nearbyintl __attribute__((weak, alias("__nearbyintl")));

#endif
