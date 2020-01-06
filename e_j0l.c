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

/* __ieee754_j0(x)
 * Bessel function of the first kind of order zero.
 * Method -- j0(x):
 *	1. For tiny x, we use j0(x) = 1 - x^2/4 + x^4/64 - ...
 *	2. Reduce x to |x| since j0(x)=j0(-x),  and
 *	   for x in (0,2)
 *		j0(x) = 1 - z/4 + z^2*R0/S0,  where z = x*x;
 *	   for x in (2,inf)
 *		j0(x) = sqrt(2/(pi*x))*(p0(x)*cos(x0)-q0(x)*sin(x0))
 *	   where x0 = x-pi/4. It is better to compute sin(x0),cos(x0)
 *	   as follow:
 *		cos(x0) = cos(x)cos(pi/4)+sin(x)sin(pi/4)
 *			= 1/sqrt(2) * (cos(x) + sin(x))
 *		sin(x0) = sin(x)cos(pi/4)-cos(x)sin(pi/4)
 *			= 1/sqrt(2) * (sin(x) - cos(x))
 *	   (To avoid cancellation, use
 *		sin(x) +- cos(x) = -cos(2x)/(sin(x) -+ cos(x))
 *	    to compute the worse one.)
 *
 *	3 Special cases
 *		j0(nan)= nan
 *		j0(0) = 1
 *		j0(inf) = 0
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

long double __ieee754_j0l(long double x)
{
	long double z, s, c, ss, cc, r, u, v;
	int32_t ix;
	uint32_t se;

	static const long double hugeval = 1e4930L;
	static const long double one = 1.0L;
	static const long double invsqrtpi = 5.6418958354775628694807945156077258584405e-1L;
	static const long double zero = 0.0;
	
	/* J0(x) = 1 - x^2 / 4 + x^4 R0(x^2) / S0(x^2)
	   0 <= x <= 2
	   peak relative error 1.41e-22 */
	static const long double R[5] = {
		4.287176872744686992880841716723478740566E7L,
		-6.652058897474241627570911531740907185772E5L,
		7.011848381719789863458364584613651091175E3L,
		-3.168040850193372408702135490809516253693E1L,
		6.030778552661102450545394348845599300939E-2L
	};
	static const long double S[4] =
	{
		2.743793198556599677955266341699130654342E9L,
		3.364330079384816249840086842058954076201E7L,
		1.924119649412510777584684927494642526573E5L,
		6.239282256012734914211715620088714856494E2L
		/*   1.000000000000000000000000000000000000000E0L, */
	};
	
	GET_LDOUBLE_EXP(se, x);
	ix = se & 0x7fff;
	if (ix >= 0x7fff)
		return one / (x * x);
	x = __ieee754_fabsl(x);
	if (ix >= 0x4000)					/* |x| >= 2.0 */
	{
		__ieee754_sincosl(x, &s, &c);
		ss = s - c;
		cc = s + c;
		if (ix < 0x7ffe)
		{								/* make sure x+x not overflow */
			z = -__ieee754_cosl(x + x);
			if ((s * c) < zero)
				cc = z / ss;
			else
				ss = z / cc;
		}
		/*
		 * j0(x) = 1/sqrt(pi) * (P(0,x)*cc - Q(0,x)*ss) / sqrt(x)
		 * y0(x) = 1/sqrt(pi) * (P(0,x)*ss + Q(0,x)*cc) / sqrt(x)
		 */
		if (ix > 0x4080)						/* 2^129 */
			z = (invsqrtpi * cc) / __ieee754_sqrtl(x);
		else
		{
			u = __j0_y0_pzerol(x);
			v = __j0_y0_qzerol(x);
			z = invsqrtpi * (u * cc - v * ss) / __ieee754_sqrtl(x);
		}
		return z;
	}
	if (ix < 0x3fef)					/* |x| < 2**-16 */
	{
		/* raise inexact if x != 0 */
		math_force_eval(hugeval + x);
		if (ix < 0x3fde)				/* |x| < 2^-33 */
			return one;
		else
			return one - 0.25 * x * x;
	}
	z = x * x;
	r = z * (R[0] + z * (R[1] + z * (R[2] + z * (R[3] + z * R[4]))));
	s = S[0] + z * (S[1] + z * (S[2] + z * (S[3] + z)));
	if (ix < 0x3fff)
	{									/* |x| < 1.00 */
		return (one - 0.25 * z + z * (r / s));
	} else
	{
		u = 0.5 * x;
		return ((one + u) * (one - u) + z * (r / s));
	}
}

/* wrapper j0 */
long double __j0l(long double x)
{
	if (isgreater(__ieee754_fabsl(x), X_TLOSS) && _LIB_VERSION != _IEEE_ && _LIB_VERSION != _POSIX_)
		/* j0(|x|>X_TLOSS) */
		return __kernel_standard_l(x, x, 0.0L, KMATHERRL_J0_TLOSS);

	return __ieee754_j0l(x);
}

__typeof(__j0l) j0l __attribute__((weak, alias("__j0l")));

#endif
