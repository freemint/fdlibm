/* e_j0f.c -- float version of e_j0.c.
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

float __ieee754_j0f(float x)
{
	float z, s, c, ss, cc, r, u, v;
	int32_t hx, ix;

	static const float hugeval = 1e30;
	static const float one = 1.0;
	static const float invsqrtpi = 5.6418961287e-01;		/* 0x3f106ebb */
	static const float zero = 0.0;
	
	/* R0/S0 on [0, 2.00] */
	static const float R02 = 1.5625000000e-02;				/* 0x3c800000 */
	static const float R03 = -1.8997929874e-04;				/* 0xb947352e */
	static const float R04 = 1.8295404516e-06;				/* 0x35f58e88 */
	static const float R05 = -4.6183270541e-09;				/* 0xb19eaf3c */
	static const float S01 = 1.5619102865e-02;				/* 0x3c7fe744 */
	static const float S02 = 1.1692678527e-04;				/* 0x38f53697 */
	static const float S03 = 5.1354652442e-07;				/* 0x3509daa6 */
	static const float S04 = 1.1661400734e-09;				/* 0x30a045e8 */

	GET_FLOAT_WORD(hx, x);
	ix = hx & IC(0x7fffffff);
	if (!FLT_UWORD_IS_FINITE(ix))
		return one / (x * x);
	x = __ieee754_fabsf(x);
	if (ix >= IC(0x40000000))
	{									/* |x| >= 2.0 */
		__ieee754_sincosf(x, &s, &c);
		ss = s - c;
		cc = s + c;
		if (ix <= FLT_UWORD_HALF_MAX)
		{								/* make sure x+x not overflow */
			z = -__ieee754_cosf(x + x);
			if ((s * c) < zero)
				cc = z / ss;
			else
				ss = z / cc;
		}
		/*
		 * j0(x) = 1/sqrt(pi) * (P(0,x)*cc - Q(0,x)*ss) / sqrt(x)
		 * y0(x) = 1/sqrt(pi) * (P(0,x)*ss + Q(0,x)*cc) / sqrt(x)
		 */
		if (ix > IC(0x48000000))
			z = (invsqrtpi * cc) / __ieee754_sqrtf(x);
		else
		{
			u = __j0_y0_pzerof(x);
			v = __j0_y0_qzerof(x);
			z = invsqrtpi * (u * cc - v * ss) / __ieee754_sqrtf(x);
		}
		return z;
	}
	if (ix < IC(0x39000000))
	{									/* |x| < 2**-13 */
		math_force_eval(hugeval + x);	/* raise inexact if x != 0 */
		if (ix < IC(0x32000000))
			return one;					/* |x|<2**-27 */
		else
			return one - 0.25f * x * x;
	}
	z = x * x;
	r = z * (R02 + z * (R03 + z * (R04 + z * R05)));
	s = one + z * (S01 + z * (S02 + z * (S03 + z * S04)));
	if (ix < IC(0x3F800000))
	{									/* |x| < 1.00 */
		return one + z * (-0.25f + (r / s));
	} else
	{
		u = 0.5f * x;

		return ((one + u) * (one - u) + z * (r / s));
	}
}


/* wrapper j0 */
float __j0f(float x)
{
	if (isgreater(__ieee754_fabsf(x), X_TLOSS) && _LIB_VERSION != _IEEE_ && _LIB_VERSION != _POSIX_)
		/* j0(|x|>X_TLOSS) */
		return __kernel_standard_f(x, x, 0.0f, KMATHERRF_J0_TLOSS);

	return __ieee754_j0f(x);
}

__typeof(__j0f) j0f __attribute__((weak, alias("__j0f")));
