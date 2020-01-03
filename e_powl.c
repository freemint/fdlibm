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

/* Expansions and modifications for 128-bit long double are
   Copyright (C) 2001 Stephen L. Moshier <moshier@na-net.ornl.gov>
   and are incorporated herein by permission of the author.  The author
   reserves the right to distribute this material elsewhere under different
   copying permissions.  These modifications are distributed here under
   the following terms:

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, see
    <http://www.gnu.org/licenses/>.  */

/* __ieee754_powl(x,y) return x**y
 *
 *		      n
 * Method:  Let x =  2   * (1+f)
 *	1. Compute and return log2(x) in two pieces:
 *		log2(x) = w1 + w2,
 *	   where w1 has 113-53 = 60 bit trailing zeros.
 *	2. Perform y*log2(x) = n+y' by simulating multi-precision
 *	   arithmetic, where |y'|<=0.5.
 *	3. Return x**y = 2**n*exp(y'*log2)
 *
 * Special cases:
 *	1.  (anything) ** 0  is 1
 *	2.  (anything) ** 1  is itself
 *	3a. (anything) ** NAN is NAN except
 *	3b. +1         ** NAN is 1
 *	4.  NAN ** (anything except 0) is NAN
 *	5.  +-(|x| > 1) **  +INF is +INF
 *	6.  +-(|x| > 1) **  -INF is +0
 *	7.  +-(|x| < 1) **  +INF is +0
 *	8.  +-(|x| < 1) **  -INF is +INF
 *	9.  +-1         ** +-INF is 1
 *	10. +0 ** (+anything except 0, NAN)               is +0
 *	11. -0 ** (+anything except 0, NAN, odd integer)  is +0
 *	12. +0 ** (-anything except 0, NAN)               is +INF
 *	13. -0 ** (-anything except 0, NAN, odd integer)  is +INF
 *	14. -0 ** (odd integer) = -( +0 ** (odd integer) )
 *	15. +INF ** (+anything except 0,NAN) is +INF
 *	16. +INF ** (-anything except 0,NAN) is +0
 *	17. -INF ** (anything)  = -0 ** (-anything)
 *	18. (-anything) ** (integer) is (-1)**(integer)*(+anything**integer)
 *	19. (-anything except 0 and inf) ** (non-integer) is NAN
 *
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_pow

long double __ieee754_powl(long double x, long double y)
{
	int x_class = fpclassify(x);
	int y_class = fpclassify(y);
	int odd_y;
	long double d, rslt;
	int32_t dexp, yexp;
	
	if (y_class == FP_ZERO || x == 1.0L)
	{
		/* unless x is signaling NaN */
		if (issignalingl(x))
			return __builtin_nanl("");
		return 1.0L;
	}

	if (x_class == FP_NAN || y_class == FP_NAN)
	{
		rslt = signbit(x) ? -__builtin_nanl("") : __builtin_nanl("");
		return rslt;
	}
	if (x_class == FP_ZERO)
	{
		if (y_class == FP_INFINITE)
			return signbit(y) ? HUGE_VALL : 0.0L;

		if (signbit(x) && __ieee754_truncl(y) != y)
		{
			return signbit(y) ? (1.0 / -x) : 0.0L;
		}
		d = __ieee754_scalbnl(y, -1);
		odd_y = __ieee754_truncl(d) != d;
		if (!signbit(y))
		{
			if (!odd_y || !signbit(x))
				return 0.0L;
			return -0.0L;
		}

		feraiseexcept(FE_DIVBYZERO);
		if (!odd_y || !signbit(x))
			return HUGE_VALL;
		return signbit(x) ? -HUGE_VALL : HUGE_VALL;
	}
	if (y_class == FP_INFINITE)
	{
		long double a_x;

		if (x_class == FP_INFINITE)
			return (signbit(y) ? 0.0L : HUGE_VALL);
		a_x = (signbit(x) ? -x : x);
		if (a_x == 1.0L)
			return 1.0L;
		if (a_x > 1.0)
			return (signbit(y) == 0 ? HUGE_VALL : 0.0L);
		return (!signbit(y) ? 0.0L : HUGE_VALL);
	}
	if (x_class == FP_INFINITE)
	{
		/* pow (x, y) signals the invalid operation exception for finite x < 0 and finite non-integer y.  */
		if (signbit(x) && __ieee754_truncl(y) != y)
		{
			return signbit(y) ? 1.0 / -x : -x;
		}
		d = __ieee754_scalbnl(y, -1);
		odd_y = __ieee754_truncl(d) != d;
		/* pow( -inf, y) = +0 for y<0 and not an odd integer,  */
		if (signbit(x) && signbit(y) && !odd_y)
			return 0.0L;
		/* pow( -inf, y) = -inf for y an odd integer > 0.  */
		if (signbit(x) && !signbit(y) && odd_y)
			return -HUGE_VALL;
		/* pow( -inf, y) = +inf for y>0 and not an odd integer.  */
		if (signbit(x) && !signbit(y) && !odd_y)
			return HUGE_VALL;
		/* pow (+/-inf, y) is +/-0 with no exception for y an odd integer < 0. */
		if (signbit(y))
		{
			/* pow (+/-inf, y) is +0 with no exception for finite y < 0 and not an odd integer.  */
			return (odd_y && signbit(x)) ? -0.0L : 0.0L;
		}
		/* pow (+/-inf, y) is +/-inf with no exception for finite y > 0 an odd integer.  */
		/* pow (+/-inf, y) is +inf with no exception for finite y > 0 and not an odd integer.  */
		return (odd_y && signbit(x) ? -HUGE_VALL : HUGE_VALL);
	}

	d = __ieee754_truncl(y);
	if (d != y)
	{
		if (signbit(x))
		{
			return -__builtin_nanl("");
		}
		if (y == 0.5L)
		{
			return __ieee754_sqrtl(x);
		}
	} else if ((d <= (long double) INT_MAX && d >= (long double) INT_MIN))
	{
		return __ieee754_powil(x, (int) y);
	}
	/* As exp already checks for minlog and maxlog no further checks are necessary.  */
	d = __ieee754_log2l(__ieee754_fabsl(x));
	dexp = __ieee754_ilogbl(d);
	yexp = __ieee754_ilogbl(y);
	if (dexp > 0 && (yexp + dexp) >= LDBL_MAX_EXP)
	{
		dexp = __ieee754_ilogbl(x);
		if ((dexp < 0 && y < 0.0L) || (dexp > 0 && y > 0.0L))
		{
			feraiseexcept(FE_OVERFLOW);
			rslt = HUGE_VALL;
		} else
		{
			feraiseexcept(FE_UNDERFLOW);
			rslt = 0;
		}
	} else if (dexp < 0 && y > 0.0L && (-yexp + dexp) < LDBL_MIN_EXP)
	{
		feraiseexcept(FE_UNDERFLOW);
		if ((-yexp + dexp) >= (LDBL_MIN_EXP - LDBL_MANT_DIG))
			rslt = 1.0L;
		else
			rslt = 0;
	} else
	{
		rslt = __ieee754_exp2l(y * d);
	}
	
	if (signbit(x))
	{
		y = __ieee754_scalbnl(y, -1);
		if (y != __ieee754_truncl(y))
			rslt = -rslt;
	}
	return rslt;
}

#endif

/* wrapper powl */
long double __powl(long double x, long double y)
{
	long double z = __ieee754_powl(x, y);

	if (!isfinite(z))
	{
		if (_LIB_VERSION != _IEEE_)
		{
			if (isnan(x))
			{
				if (y == 0.0L)
					/* pow(NaN,0.0) */
					return __kernel_standard_l(x, y, z, KMATHERRL_POW_NAN);
			} else if (isfinite(x) && isfinite(y))
			{
				if (isnan(z))
				{
					/* pow neg**non-int */
					return __kernel_standard_l(x, y, z, KMATHERRL_POW_NONINT);
				} else if (x == 0.0L && y < 0.0L)
				{
					if (signbit(x) && signbit(z))
						/* pow(-0.0,negative) */
						return __kernel_standard_l(x, y, z, KMATHERRL_POW_MINUS);
					else
						/* pow(+0.0,negative) */
						return __kernel_standard_l(x, y, z, KMATHERRL_POW_ZEROMINUS);
				} else
				{
					/* pow overflow */
					return __kernel_standard_l(x, y, z, KMATHERRL_POW_OVERFLOW);
				}
			}
		}
	} else if (z == 0.0L && isfinite(x) && isfinite(y) && _LIB_VERSION != _IEEE_)
	{
		if (x == 0.0L)
		{
			if (y == 0.0L)
				/* pow(0.0,0.0) */
				return __kernel_standard_l(x, y, z, KMATHERRL_POW_ZERO);
		} else
		{
			/* pow underflow */
			return __kernel_standard_l(x, y, z, KMATHERRL_POW_UNDERFLOW);
		}
	}

	return z;
}

__typeof(__powl) powl __attribute__((weak, alias("__powl")));

#endif
