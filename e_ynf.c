/* e_ynf.c -- float version of e_jn.c.
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

float __ieee754_ynf(int n, float x)
{
	int32_t i, hx, ix;
	uint32_t ib;
	int sign;
	float a, b, temp;

	static const float zero = 0.0000000000e+00;

	GET_FLOAT_WORD(hx, x);
	ix = IC(0x7fffffff) & hx;
	/* if Y(n,NaN) is NaN */
	if (FLT_UWORD_IS_NAN(ix))
		return x + x;
	if (FLT_UWORD_IS_ZERO(ix))
		return -HUGE_VALF + x;			/* -inf and overflow exception.  */
	if (hx < 0)
		return zero / (zero * x);
	sign = 1;
	if (n < 0)
	{
		n = -n;
		sign = 1 - ((n & 1) << 1);
	}
	if (n == 0)
		return (__ieee754_y0f(x));
	if (n == 1)
		return (sign * __ieee754_y1f(x));
	if (FLT_UWORD_IS_INFINITE(ix))
		return zero;

	a = __ieee754_y0f(x);
	b = __ieee754_y1f(x);
	/* quit if b is -inf */
	GET_FLOAT_WORD(ib, b);
	for (i = 1; i < n && ib != UC(0xff800000); i++)
	{
		temp = b;
		b = ((float) (i + i) / x) * b - a;
		GET_FLOAT_WORD(ib, b);
		a = temp;
	}
	if (sign > 0)
		return b;
	return -b;
}

/* wrapper yn */
float __ynf(int n, float x)
{
	if ((islessequal(x, 0.0F) || isgreater(x, X_TLOSS)) && _LIB_VERSION != _IEEE_)
	{
		if (x < 0.0F)
		{
			/* d = zero/(x-x) */
			feraiseexcept(FE_INVALID);
			return __kernel_standard_f(n, x, -HUGE_VALF, KMATHERRF_YN_MINUS);
		} else if (x == 0.0F)
			/* d = -one/(x-x) */
			return __kernel_standard_f(n, x, -HUGE_VALF, KMATHERRF_YN_ZERO);
		else if (_LIB_VERSION != _POSIX_)
			/* yn(n,x>X_TLOSS) */
			return __kernel_standard_f(n, x, 0.0f, KMATHERRF_YN_TLOSS);
	}

	return __ieee754_ynf(n, x);
}

__typeof(__ynf) ynf __attribute__((weak, alias("__ynf")));
