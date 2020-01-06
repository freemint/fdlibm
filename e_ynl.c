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

/* Modifications for long double are
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

/*
 * __ieee754_yn(n, x)
 * floating point Bessel's function of the 2nd kind
 * of order n
 *
 * Special cases:
 *	y0(0)=y1(0)=yn(n,0) = -inf with overflow signal;
 *	y0(-ve)=y1(-ve)=yn(n,-ve) are NaN with invalid signal.
 * Note 2. About jn(n,x), yn(n,x)
 *	For n=0, j0(x) is called,
 *	for n=1, j1(x) is called,
 *	for n<x, forward recursion us used starting
 *	from values of j0(x) and j1(x).
 *	for n>x, a continued fraction approximation to
 *	j(n,x)/j(n-1,x) is evaluated and then backward
 *	recursion is used starting from a supposed value
 *	for j(n,x). The resulting value of j(0,x) is
 *	compared with the actual value to correct the
 *	supposed value of j(n,x).
 *
 *	yn(n,x) is similar in all respects, except
 *	that forward recursion is used for all
 *	values of n>1.
 *
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

long double __ieee754_ynl(int n, long double x)
{
	uint32_t se, i0, i1;
	int32_t i, ix;
	int sign;
	long double a, b, temp;

	static const long double invsqrtpi = 5.64189583547756286948079e-1L;
	static const long double zero = 0.0L;
	
	GET_LDOUBLE_WORDS(se, i0, i1, x);
	ix = se & IEEE854_LONG_DOUBLE_MAXEXP;
	/* if Y(n,NaN) is NaN */
	if ((ix == IEEE854_LONG_DOUBLE_MAXEXP) && ((i0 & IC(0x7fffffff)) != 0))
		return x + x;
	if ((ix | i0 | i1) == 0)
		return -HUGE_VALL + x;			/* -inf and overflow exception.  */
	if (se & 0x8000)
		return zero / (zero * x);
	sign = 1;
	if (n < 0)
	{
		n = -n;
		sign = 1 - ((n & 1) << 1);
	}
	if (n == 0)
		return (__ieee754_y0l(x));
	if (n == 1)
		return (sign * __ieee754_y1l(x));
	if (ix == IEEE854_LONG_DOUBLE_MAXEXP)
		return zero;
	if (ix >= 0x412D)
	{									/* x > 2**302 */

		/* ??? See comment above on the possible futility of this.  */

		/* (x >> n**2)
		 *      Jn(x) = cos(x-(2n+1)*pi/4)*sqrt(2/x*pi)
		 *      Yn(x) = sin(x-(2n+1)*pi/4)*sqrt(2/x*pi)
		 *      Let s=sin(x), c=cos(x),
		 *          xn=x-(2n+1)*pi/4, sqt2 = sqrt(2),then
		 *
		 *             n    sin(xn)*sqt2    cos(xn)*sqt2
		 *          ----------------------------------
		 *             0     s-c             c+s
		 *             1    -s-c            -c+s
		 *             2    -s+c            -c-s
		 *             3     s+c             c-s
		 */
		long double s;
		long double c;

		__ieee754_sincosl(x, &s, &c);
		switch (n & 3)
		{
		case 0:
			temp = s - c;
			break;
		case 1:
			temp = -s - c;
			break;
		case 2:
			temp = -s + c;
			break;
		case 3:
			temp = s + c;
			break;
		}
		b = invsqrtpi * temp / __ieee754_sqrtl(x);
	} else
	{
		a = __ieee754_y0l(x);
		b = __ieee754_y1l(x);
		/* quit if b is -inf */
		GET_LDOUBLE_WORDS(se, i0, i1, b);
		/* Use 0xffffffff since GET_LDOUBLE_WORDS sign-extends SE.  */
		for (i = 1; i < n && se != UC(0xffffffff); i++)
		{
			temp = b;
			b = ((long double) (i + i) / x) * b - a;
			GET_LDOUBLE_WORDS(se, i0, i1, b);
			a = temp;
		}
	}
	if (sign > 0)
		return b;
	return -b;
}

/* wrapper yn */
long double __ynl(int n, long double x)
{
	if ((islessequal(x, 0.0) || isgreater(x, X_TLOSS)) && _LIB_VERSION != _IEEE_)
	{
		if (x < 0.0)
		{
			/* d = zero/(x-x) */
			feraiseexcept(FE_INVALID);
			return __kernel_standard_l(n, x, -HUGE_VALL, KMATHERRL_YN_MINUS);
		} else if (x == 0.0)
			/* d = -one/(x-x) */
			return __kernel_standard_l(n, x, -HUGE_VALL, KMATHERRL_YN_ZERO);
		else if (_LIB_VERSION != _POSIX_)
			/* yn(n,x>X_TLOSS) */
			return __kernel_standard_l(n, x, 0.0L, KMATHERRL_YN_TLOSS);
	}

	return __ieee754_yn(n, x);
}

__typeof(__ynl) ynl __attribute__((weak, alias("__ynl")));

#endif
