/* s_tanhf.c -- float version of s_tanh.c.
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

#ifndef __have_fpu_tanh

float __ieee754_tanhf(float x)
{
	float t, z;
	int32_t jx, ix;

	static const float one = 1.0;
	static const float two = 2.0;
	static const float tiny = 1.0e-30;
	
	GET_FLOAT_WORD(jx, x);
	ix = jx & IC(0x7fffffff);

	/* x is INF or NaN */
	if (!FLT_UWORD_IS_FINITE(ix))
	{
		if (jx >= 0)
			return one / x + one;		/* tanh(+-inf)=+-1 */
		else
			return one / x - one;		/* tanh(NaN) = NaN */
	}

	/* |x| < 22 */
	if (ix < IC(0x41b00000))
	{									/* |x|<22 */
		if (ix == 0)
			return x;					/* x == +-0 */
		if (ix < IC(0x24000000))		/* |x|<2**-55 */
			return x * (one + x);		/* tanh(small) = small */
		if (ix >= IC(0x3f800000))
		{								/* |x|>=1  */
			t = expm1f(two * __ieee754_fabsf(x));
			z = one - two / (t + two);
		} else
		{
			t = expm1f(-two * __ieee754_fabsf(x));
			z = -t / (t + two);
		}
		/* |x| > 22, return +-1 */
	} else
	{
		z = one - tiny;					/* raised inexact flag */
	}
	return (jx >= 0) ? z : -z;
}

#endif

float __tanhf(float x)
{
	return __ieee754_tanhf(x);
}

__typeof(__tanhf) tanhf __attribute__((weak, alias("__tanhf")));
