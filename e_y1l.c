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

/* Long double expansions are
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

/* __ieee754_y1(x)
 * Bessel function of the second kind of order one.
 * Method -- y1(x):
 *	1. screen out x<=0 cases: y1(0)=-inf, y1(x<0)=NaN
 *	2. For x<2.
 *	   Since
 *		y1(x) = 2/pi*(j1(x)*(ln(x/2)+Euler)-1/x-x/2+5/64*x^3-...)
 *	   therefore y1(x)-2/pi*j1(x)*ln(x)-1/x is an odd function.
 *	   We use the following function to approximate y1,
 *		y1(x) = x*U(z)/V(z) + (2/pi)*(j1(x)*ln(x)-1/x), z= x^2
 *	   Note: For tiny x, 1/x dominate y1 and hence
 *		y1(tiny) = -2/pi/tiny
 *	3. For x>=2.
 *		y1(x) = sqrt(2/(pi*x))*(p1(x)*sin(x1)+q1(x)*cos(x1))
 *	   where x1 = x-3*pi/4. It is better to compute sin(x1),cos(x1)
 *	   by method mentioned above.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

long double __ieee754_y1l(long double x)
{
	long double z, s, c, ss, cc, u, v;
	int32_t ix;
	uint32_t se, i0, i1;

	static const long double one = 1.0L;
	static const long double invsqrtpi = 5.6418958354775628694807945156077258584405e-1L;
	static const long double tpi = 6.3661977236758134307553505349005744813784e-1L;
	static const long double zero = 0.0;
	
	/* Y1(x) = 2/pi * (log(x) * j1(x) - 1/x) + x R(x^2)
	   0 <= x <= 2
	   Peak relative error 2.3e-23 */
	static const long double U0[6] = {
		-5.908077186259914699178903164682444848615E10L,
		1.546219327181478013495975514375773435962E10L,
		-6.438303331169223128870035584107053228235E8L,
		9.708540045657182600665968063824819371216E6L,
		-6.138043997084355564619377183564196265471E4L,
		1.418503228220927321096904291501161800215E2L
	};
	
	static const long double V0[5] = {
		3.013447341682896694781964795373783679861E11L,
		4.669546565705981649470005402243136124523E9L,
		3.595056091631351184676890179233695857260E7L,
		1.761554028569108722903944659933744317994E5L,
		5.668480419646516568875555062047234534863E2L
		/*  1.000000000000000000000000000000000000000E0L, */
	};
	
	GET_LDOUBLE_WORDS(se, i0, i1, x);
	ix = se & IEEE854_LONG_DOUBLE_MAXEXP;
	/* if Y1(NaN) is NaN, Y1(-inf) is NaN, Y1(inf) is 0 */
	if (se & 0x8000)
		return zero / (zero * x);
	if (ix >= IEEE854_LONG_DOUBLE_MAXEXP)
		return one / (x + x * x);
	if ((i0 | i1) == 0)
		return -HUGE_VALL + x;			/* -inf and overflow exception.  */
	if (ix >= 0x4000)
	{									/* |x| >= 2.0 */
		__ieee754_sincosl(x, &s, &c);
		ss = -s - c;
		cc = s - c;
		if (ix < 0x7ffe)
		{								/* make sure x+x not overflow */
			z = __ieee754_cosl(x + x);
			if ((s * c) > zero)
				cc = z / ss;
			else
				ss = z / cc;
		}
		/* y1(x) = sqrt(2/(pi*x))*(p1(x)*sin(x0)+q1(x)*cos(x0))
		 * where x0 = x-3pi/4
		 *      Better formula:
		 *              cos(x0) = cos(x)cos(3pi/4)+sin(x)sin(3pi/4)
		 *                      =  1/sqrt(2) * (sin(x) - cos(x))
		 *              sin(x0) = sin(x)cos(3pi/4)-cos(x)sin(3pi/4)
		 *                      = -1/sqrt(2) * (cos(x) + sin(x))
		 * To avoid cancellation, use
		 *              sin(x) +- cos(x) = -cos(2x)/(sin(x) -+ cos(x))
		 * to compute the worse one.
		 */
		if (ix > 0x4080)
			z = (invsqrtpi * ss) / __ieee754_sqrtl(x);
		else
		{
			u = __j1_y1_ponel(x);
			v = __j1_y1_qonel(x);
			z = invsqrtpi * (u * ss + v * cc) / __ieee754_sqrtl(x);
		}
		return z;
	}
	if (ix <= 0x3fbe)
	{									/* x < 2**-65 */
		return (-tpi / x);
	}
	z = x * x;
	u = U0[0] + z * (U0[1] + z * (U0[2] + z * (U0[3] + z * (U0[4] + z * U0[5]))));
	v = V0[0] + z * (V0[1] + z * (V0[2] + z * (V0[3] + z * (V0[4] + z))));
	return (x * (u / v) + tpi * (__ieee754_j1l(x) * __ieee754_logl(x) - one / x));
}

/* wrapper y1 */
long double __y1l(long double x)
{
	if ((islessequal(x, 0.0) || isgreater(x, X_TLOSS)) && _LIB_VERSION != _IEEE_)
	{
		if (x < 0.0)
		{
			/* d = zero/(x-x) */
			feraiseexcept(FE_INVALID);
			return __kernel_standard_l(x, x, -HUGE_VALL, KMATHERRL_Y1_MINUS);
		} else if (x == 0.0)
			/* d = -one/(x-x) */
			return __kernel_standard_l(x, x, -HUGE_VALL, KMATHERRL_Y1_ZERO);
		else if (_LIB_VERSION != _POSIX_)
			/* y1(x>X_TLOSS) */
			return __kernel_standard_l(x, x, 0.0l, KMATHERRL_Y1_TLOSS);
	}

	return __ieee754_y1l(x);
}

__typeof(__y1l) y1l __attribute__((weak, alias("__y1l")));

#endif
