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

/* double erf(double x)
 * double erfc(double x)
 *			     x
 *		      2      |\
 *     erf(x)  =  ---------  | exp(-t*t)dt
 *		   sqrt(pi) \|
 *			     0
 *
 *     erfc(x) =  1-erf(x)
 *  Note that
 *		erf(-x) = -erf(x)
 *		erfc(-x) = 2 - erfc(x)
 *
 * Method:
 *	1. For |x| in [0, 0.84375]
 *	    erf(x)  = x + x*R(x^2)
 *          erfc(x) = 1 - erf(x)           if x in [-.84375,0.25]
 *                  = 0.5 + ((0.5-x)-x*R)  if x in [0.25,0.84375]
 *	   Remark. The formula is derived by noting
 *          erf(x) = (2/sqrt(pi))*(x - x^3/3 + x^5/10 - x^7/42 + ....)
 *	   and that
 *          2/sqrt(pi) = 1.128379167095512573896158903121545171688
 *	   is close to one. The interval is chosen because the fix
 *	   point of erf(x) is near 0.6174 (i.e., erf(x)=x when x is
 *	   near 0.6174), and by some experiment, 0.84375 is chosen to
 *	   guarantee the error is less than one ulp for erf.
 *
 *      2. For |x| in [0.84375,1.25], let s = |x| - 1, and
 *         c = 0.84506291151 rounded to single (24 bits)
 *	erf(x)  = sign(x) * (c  + P1(s)/Q1(s))
 *	erfc(x) = (1-c)  - P1(s)/Q1(s) if x > 0
 *			  1+(c+P1(s)/Q1(s))    if x < 0
 *	   Remark: here we use the taylor series expansion at x=1.
 *		erf(1+s) = erf(1) + s*Poly(s)
 *			 = 0.845.. + P1(s)/Q1(s)
 *	   Note that |P1/Q1|< 0.078 for x in [0.84375,1.25]
 *
 *      3. For x in [1.25,1/0.35(~2.857143)],
 *	erfc(x) = (1/x)*exp(-x*x-0.5625+R1(z)/S1(z))
 *              z=1/x^2
 *	erf(x)  = 1 - erfc(x)
 *
 *      4. For x in [1/0.35,107]
 *	erfc(x) = (1/x)*exp(-x*x-0.5625+R2/S2) if x > 0
 *			= 2.0 - (1/x)*exp(-x*x-0.5625+R2(z)/S2(z))
 *                             if -6.666<x<0
 *			= 2.0 - tiny		(if x <= -6.666)
 *              z=1/x^2
 *	erf(x)  = sign(x)*(1.0 - erfc(x)) if x < 6.666, else
 *	erf(x)  = sign(x)*(1.0 - tiny)
 *      Note1:
 *	   To compute exp(-x*x-0.5625+R/S), let s be a single
 *	   precision number and s := x; then
 *		-x*x = -s*s + (s-x)*(s+x)
 *	        exp(-x*x-0.5626+R/S) =
 *			exp(-s*s-0.5625)*exp((s-x)*(s+x)+R/S);
 *      Note2:
 *	   Here 4 and 5 make use of the asymptotic series
 *			  exp(-x*x)
 *		erfc(x) ~ ---------- * ( 1 + Poly(1/x^2) )
 *			  x*sqrt(pi)
 *
 *      5. For inf > x >= 107
 *	erf(x)  = sign(x) *(1 - tiny)  (raise inexact)
 *	erfc(x) = tiny*tiny (raise underflow) if x > 0
 *			= 2 - tiny if x<0
 *
 *      7. Special case:
 *	erf(0)  = 0, erf(inf)  = 1, erf(-inf) = -1,
 *	erfc(0) = 1, erfc(inf) = 0, erfc(-inf) = 2,
 *		erfc/erf(NaN) is NaN
 */


#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

long double __erfl(long double x)
{
	long double R, S, P, Q, s, y, z, r;
	int32_t ix, i;
	uint32_t se, i0, i1;

	static const long double tiny = 1e-4931L;
	static const long double one = 1.0L;
	static const long double erx = 0.845062911510467529296875L;
	/* c = (float)0.84506291151 */
	/*
	 * Coefficients for approximation to  erf on [0,0.84375]
	 */
		/* 2/sqrt(pi) - 1 */
	static const long double efx = 1.2837916709551257389615890312154517168810E-1L;
		/* 8 * (2/sqrt(pi) - 1) */
	static const long double efx8 = 1.0270333367641005911692712249723613735048E0L;

#include "t_erfl.h"

	GET_LDOUBLE_WORDS(se, i0, i1, x);
	ix = se & 0x7fff;

	if (ix >= 0x7fff)
	{									/* erf(nan)=nan */
		i = ((se & 0xffff) >> 15) << 1;
		return (long double) (1 - i) + one / x;	/* erf(+-inf)=+-1 */
	}

	ix = (ix << 16) | (i0 >> 16);
	if (ix < IC(0x3ffed800))			/* |x|<0.84375 */
	{
		if (ix < IC(0x3fde8000))		/* |x|<2**-33 */
		{
			if (ix < IC(0x00080000))
				return 0.125 * (8.0 * x + efx8 * x);	/*avoid underflow */
			return x + efx * x;
		}
		z = x * x;
		r = pp[0] + z * (pp[1] + z * (pp[2] + z * (pp[3] + z * (pp[4] + z * pp[5]))));
		s = qq[0] + z * (qq[1] + z * (qq[2] + z * (qq[3] + z * (qq[4] + z * (qq[5] + z)))));
		y = r / s;
		return x + x * y;
	}
	if (ix < IC(0x3fffa000))			/* 1.25 */
	{									/* 0.84375 <= |x| < 1.25 */
		s = __ieee754_fabsl(x) - one;
		P = pa[0] + s * (pa[1] + s * (pa[2] + s * (pa[3] + s * (pa[4] + s * (pa[5] + s * (pa[6] + s * pa[7]))))));
		Q = qa[0] + s * (qa[1] + s * (qa[2] + s * (qa[3] + s * (qa[4] + s * (qa[5] + s * (qa[6] + s))))));
		if ((se & 0x8000) == 0)
			return erx + P / Q;
		else
			return -erx - P / Q;
	}
	if (ix >= IC(0x4001d555))			/* 6.6666259765625 */
	{									/* inf>|x|>=6.666 */
		if ((se & 0x8000) == 0)
			return one - tiny;
		else
			return tiny - one;
	}
	x = __ieee754_fabsl(x);
	s = one / (x * x);
	if (ix < IC(0x4000b6db))			/* 2.85711669921875 */
	{
		R = ra[0] + s * (ra[1] + s * (ra[2] + s * (ra[3] + s * (ra[4] +
																s * (ra[5] + s * (ra[6] + s * (ra[7] + s * ra[8])))))));
		S = sa[0] + s * (sa[1] + s * (sa[2] + s * (sa[3] + s * (sa[4] +
																s * (sa[5] +
																	 s * (sa[6] + s * (sa[7] + s * (sa[8] + s))))))));
	} else
	{									/* |x| >= 1/0.35 */
		R = rb[0] + s * (rb[1] + s * (rb[2] + s * (rb[3] + s * (rb[4] + s * (rb[5] + s * (rb[6] + s * rb[7]))))));
		S = sb[0] + s * (sb[1] + s * (sb[2] + s * (sb[3] + s * (sb[4] + s * (sb[5] + s * (sb[6] + s))))));
	}
	z = x;
	GET_LDOUBLE_WORDS(i, i0, i1, z);
	i1 = 0;
	SET_LDOUBLE_WORDS(z, i, i0, i1);
	r = __ieee754_expl(-z * z - 0.5625) * __ieee754_expl((z - x) * (z + x) + R / S);
	if ((se & 0x8000) == 0)
		return one - r / x;
	else
		return r / x - one;
}

__typeof(__erfl) erfl __attribute__((weak, alias("__erfl")));

#endif
