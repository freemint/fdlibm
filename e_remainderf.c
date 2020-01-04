/* e_remainderf.c -- float version of e_remainder.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
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

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_remainder

float __ieee754_remainderf(float x, float p)
{
	int32_t hx, hp;
	uint32_t sx;
	float p_half;

	static const float zero = 0.0;

	GET_FLOAT_WORD(hx, x);
	GET_FLOAT_WORD(hp, p);
	sx = hx & IC(0x80000000);
	hp &= IC(0x7fffffff);
	hx &= IC(0x7fffffff);

	/* purge off exception values */
	if (FLT_UWORD_IS_ZERO(hp) || !FLT_UWORD_IS_FINITE(hx) || FLT_UWORD_IS_NAN(hp))
		return (x * p) / (x * p);

	if (hp <= FLT_UWORD_HALF_MAX)
		x = __ieee754_fmodf(x, p + p);	/* now x < 2p */
	if ((hx - hp) == 0)
		return zero * x;
	x = __ieee754_fabsf(x);
	p = __ieee754_fabsf(p);
	if (hp < IC(0x01000000))
	{
		if (x + x > p)
		{
			x -= p;
			if (x + x >= p)
				x -= p;
		}
	} else
	{
		p_half = (float) 0.5 *p;

		if (x > p_half)
		{
			x -= p;
			if (x >= p_half)
				x -= p;
		}
	}
	GET_FLOAT_WORD(hx, x);
	SET_FLOAT_WORD(x, hx ^ sx);
	return x;
}

#endif

/* wrapper remainder */
float __remainderf(float x, float y)
{
	if (((y == 0.0F && !isnan(x))
		 || (isinf(x) && !isnan(y))) && _LIB_VERSION != _IEEE_)
		return __kernel_standard_f(x, y, y, KMATHERRF_REMAINDER);	/* remainder domain */

	return __ieee754_remainderf(x, y);
}

__typeof(__remainderf) remainderf __attribute__((weak, alias("__remainderf")));
__typeof(__remainderf) __dremf __attribute__((alias("__remainderf")));
__typeof(remainderf) dremf __attribute__((weak, alias("__dremf")));
