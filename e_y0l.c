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

/* __ieee754_y0(x)
 * Bessel function of the second kind of order zero.
 * Method -- y0(x):
 *	1. For x<2.
 *	   Since
 *		y0(x) = 2/pi*(j0(x)*(ln(x/2)+Euler) + x^2/4 - ...)
 *	   therefore y0(x)-2/pi*j0(x)*ln(x) is an even function.
 *	   We use the following function to approximate y0,
 *		y0(x) = U(z)/V(z) + (2/pi)*(j0(x)*ln(x)), z= x^2
 *
 *	   Note: For tiny x, U/V = u0 and j0(x)~1, hence
 *		y0(tiny) = u0 + (2/pi)*ln(tiny), (choose tiny<2**-27)
 *	2. For x>=2.
 *		y0(x) = sqrt(2/(pi*x))*(p0(x)*cos(x0)+q0(x)*sin(x0))
 *	   where x0 = x-pi/4. It is better to compute sin(x0),cos(x0)
 *	   by the method mentioned above.
 *	3. Special cases: y0(0)=-inf, y0(x<0)=NaN, y0(inf)=0.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

long double __ieee754_y0l(long double x)
{
	long double z, s, c, ss, cc, u, v;
	int32_t ix;
	uint32_t se, i0, i1;

	static const long double one = 1.0L;
	static const long double invsqrtpi = 5.6418958354775628694807945156077258584405e-1L;
	static const long double tpi = 6.3661977236758134307553505349005744813784e-1L;
	static const long double zero = 0.0;
	
	/* y0(x) = 2/pi ln(x) J0(x) + U(x^2)/V(x^2)
	   0 < x <= 2
	   peak relative error 1.7e-21 */
	static const long double U[6] = {
		-1.054912306975785573710813351985351350861E10L,
		2.520192609749295139432773849576523636127E10L,
		-1.856426071075602001239955451329519093395E9L,
		4.079209129698891442683267466276785956784E7L,
		-3.440684087134286610316661166492641011539E5L,
		1.005524356159130626192144663414848383774E3L,
	};
	
	static const long double V[5] = {
		1.429337283720789610137291929228082613676E11L,
		2.492593075325119157558811370165695013002E9L,
		2.186077620785925464237324417623665138376E7L,
		1.238407896366385175196515057064384929222E5L,
		4.693924035211032457494368947123233101664E2L,
		/*  1.000000000000000000000000000000000000000E0L */
	};
	
	GET_LDOUBLE_WORDS(se, i0, i1, x);
	ix = se & IEEE854_LONG_DOUBLE_MAXEXP;
	/* Y0(NaN) is NaN, y0(-inf) is Nan, y0(inf) is 0  */
	if (se & 0x8000)
		return zero / (zero * x);
	if (ix >= IEEE854_LONG_DOUBLE_MAXEXP)
		return one / (x + x * x);
	if ((i0 | i1) == 0)
		return -HUGE_VALL + x;			/* -inf and overflow exception.  */
	if (ix >= 0x4000)
	{									/* |x| >= 2.0 */

		/* y0(x) = sqrt(2/(pi*x))*(p0(x)*sin(x0)+q0(x)*cos(x0))
		 * where x0 = x-pi/4
		 *      Better formula:
		 *              cos(x0) = cos(x)cos(pi/4)+sin(x)sin(pi/4)
		 *                      =  1/sqrt(2) * (sin(x) + cos(x))
		 *              sin(x0) = sin(x)cos(3pi/4)-cos(x)sin(3pi/4)
		 *                      =  1/sqrt(2) * (sin(x) - cos(x))
		 * To avoid cancellation, use
		 *              sin(x) +- cos(x) = -cos(2x)/(sin(x) -+ cos(x))
		 * to compute the worse one.
		 */
		__ieee754_sincosl(x, &s, &c);
		ss = s - c;
		cc = s + c;
		/*
		 * j0(x) = 1/sqrt(pi) * (P(0,x)*cc - Q(0,x)*ss) / sqrt(x)
		 * y0(x) = 1/sqrt(pi) * (P(0,x)*ss + Q(0,x)*cc) / sqrt(x)
		 */
		if (ix < 0x7ffe)
		{								/* make sure x+x not overflow */
			z = -__ieee754_cosl(x + x);
			if ((s * c) < zero)
				cc = z / ss;
			else
				ss = z / cc;
		}
		if (ix > 0x4080)	/* 1e39 */
			z = (invsqrtpi * ss) / __ieee754_sqrtl(x);
		else
		{
			u = __j0_y0_pzerol(x);
			v = __j0_y0_qzerol(x);
			z = invsqrtpi * (u * ss + v * cc) / __ieee754_sqrtl(x);
		}
		return z;
	}
	if (ix <= 0x3fde)	/* x < 2^-33 */
	{
		z = -7.380429510868722527629822444004602747322E-2L + tpi * __ieee754_logl(x);
		return z;
	}
	z = x * x;
	u = U[0] + z * (U[1] + z * (U[2] + z * (U[3] + z * (U[4] + z * U[5]))));
	v = V[0] + z * (V[1] + z * (V[2] + z * (V[3] + z * (V[4] + z))));
	return (u / v + tpi * (__ieee754_j0l(x) * __ieee754_logl(x)));
}

/* wrapper y0 */
long double __y0l(long double x)
{
	if ((islessequal(x, 0.0) || isgreater(x, X_TLOSS)) && _LIB_VERSION != _IEEE_)
	{
		if (x < 0.0)
		{
			/* d = zero/(x-x) */
			feraiseexcept(FE_INVALID);
			return __kernel_standard_l(x, x, -HUGE_VALL, KMATHERRL_Y0_MINUS);
		} else if (x == 0.0)
			/* d = -one/(x-x) */
			return __kernel_standard_l(x, x, -HUGE_VALL, KMATHERRL_Y0_ZERO);
		else if (_LIB_VERSION != _POSIX_)
			/* y0(x>X_TLOSS) */
			return __kernel_standard_l(x, x, 0.0L, KMATHERRL_Y0_TLOSS);
	}

	return __ieee754_y0l(x);
}

__typeof(__y0l) y0l __attribute__((weak, alias("__y0l")));

#endif
