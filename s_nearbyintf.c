/* s_nearbyintf.c -- float version of s_nearbyint.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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


#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

float __nearbyintf(float x)
{
	fenv_t env;
	float value;

	feholdexcept(&env);
	value = __ieee754_rintf(x);
	fesetenv(&env);
	return value;
}

__typeof(__nearbyintf) nearbyintf __attribute__((weak, alias("__nearbyintf")));
