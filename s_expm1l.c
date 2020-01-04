/*							expm1l.c
 *
 *	Exponential function, minus 1
 *      128-bit long double precision
 *
 *
 *
 * SYNOPSIS:
 *
 * long double x, y, expm1l();
 *
 * y = expm1l( x );
 *
 *
 *
 * DESCRIPTION:
 *
 * Returns e (2.71828...) raised to the x power, minus one.
 *
 * Range reduction is accomplished by separating the argument
 * into an integer k and fraction f such that
 *
 *     x    k  f
 *    e  = 2  e.
 *
 * An expansion x + .5 x^2 + x^3 R(x) approximates exp(f) - 1
 * in the basic range [-0.5 ln 2, 0.5 ln 2].
 *
 *
 * ACCURACY:
 *
 *                      Relative error:
 * arithmetic   domain     # trials      peak         rms
 *    IEEE    -79,+MAXLOG    100,000     1.7e-34     4.5e-35
 *
 */

/* Copyright 2001 by Stephen L. Moshier

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


#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_expm1

/* exp(x) - 1 = x + 0.5 x^2 + x^3 P(x)/Q(x)
   -.5 ln 2  <  x  <  .5 ln 2
   Theoretical peak relative error = 8.1e-36  */

long double __ieee754_expm1l(long double x)
{
	long double px, qx, xx;
	int32_t ix, sign;
	uint32_t msw, lsw;
	int32_t k;

	static const long double P0 = 2.943520915569954073888921213330863757240E8L;
	static const long double P1 = -5.722847283900608941516165725053359168840E7L;
	static const long double P2 = 8.944630806357575461578107295909719817253E6L;
	static const long double P3 = -7.212432713558031519943281748462837065308E5L;
	static const long double P4 = 4.578962475841642634225390068461943438441E4L;
	static const long double P5 = -1.716772506388927649032068540558788106762E3L;
	static const long double P6 = 4.401308817383362136048032038528753151144E1L;
	static const long double P7 = -4.888737542888633647784737721812546636240E-1L;
	static const long double Q0 = 1.766112549341972444333352727998584753865E9L;
	static const long double Q1 = -7.848989743695296475743081255027098295771E8L;
	static const long double Q2 = 1.615869009634292424463780387327037251069E8L;
	static const long double Q3 = -2.019684072836541751428967854947019415698E7L;
	static const long double Q4 = 1.682912729190313538934190635536631941751E6L;
	static const long double Q5 = -9.615511549171441430850103489315371768998E4L;
	static const long double Q6 = 3.697714952261803935521187272204485251835E3L;
	static const long double Q7 = -8.802340681794263968892934703309274564037E1L;
	/* Q8 = 1.000000000000000000000000000000000000000E0 */
	/* C1 + C2 = ln 2 */
	static const long double C1 = 6.93145751953125E-1L;
	static const long double C2 = 1.428606820309417232121458176568075500134E-6L;
	/* ln (2^16384 * (1 - 2^-113)) */
	static const long double maxlog = 1.1356523406294143949491931077970764891253E4L;
	/* ln 2^-114 */
	static const long double minarg = -7.9018778583833765273564461846232128760607E1L;
	static const long double big = 1e4932L;

	/* Detect infinity and NaN.  */
	GET_LDOUBLE_WORDS(ix, msw, lsw, x);
	sign = ix & 0x8000;
	ix &= 0x7fff;
	lsw |= msw & IC(0x7fffffff);
	if (ix >= 0x7fff)
	{
		/* Infinity. */
		if (lsw == 0)
		{
			if (sign)
				return -1.0L;
			else
				return x;
		}
		/* NaN. No invalid exception. */
		return x;
	}

	/* expm1(+- 0) = +- 0.  */
	if (ix == 0 && lsw == 0)
		return x;

	/* Overflow.  */
	if (x > maxlog)
	{
		feraiseexcept(FE_OVERFLOW);
		return HUGE_VALL;
	}

	/* Minimum value.  */
	if (x < minarg)
		return (4.0 / big - 1.0L);

	/* Express x = ln 2 (k + remainder), remainder not exceeding 1/2. */
	xx = C1 + C2;						/* ln 2. */
	px = __ieee754_floorl(0.5L + x / xx);
	k = px;
	/* remainder times ln 2 */
	x -= px * C1;
	x -= px * C2;

	/* Approximate exp(remainder ln 2).  */
	px = (((((((P7 * x + P6) * x + P5) * x + P4) * x + P3) * x + P2) * x + P1) * x + P0) * x;

	qx = (((((((x + Q7) * x + Q6) * x + Q5) * x + Q4) * x + Q3) * x + Q2) * x + Q1) * x + Q0;

	xx = x * x;
	qx = x + (0.5 * xx + xx * px / qx);

	/* exp(x) = exp(k ln 2) exp(remainder ln 2) = 2^k exp(remainder ln 2).

	   We have qx = exp(remainder ln 2) - 1, so
	   exp(x) - 1 = 2^k (qx + 1) - 1
	   = 2^k qx + 2^k - 1.  */
	if (k == 16384)
	{
		px = __ieee754_ldexpl(1.0L, (int)k - 2);
		x = (px * qx + (px - 0.25L)) * 4.0L;
	} else
	{
		px = __ieee754_ldexpl(1.0L, (int)k);
		x = px * qx + (px - 1.0L);
	}
	return x;
}

#endif

long double __expm1l(long double x)
{
	long double z = __ieee754_expm1l(x);

	if ((!isfinite(z) || z == -1.0L) && isfinite(x) && _LIB_VERSION != _IEEE_)
		return __kernel_standard_l(x, x, z, signbit(x) ? KMATHERRL_EXPM1_UNDERFLOW : KMATHERRL_EXPM1_OVERFLOW);

	return z;
}

__typeof(__expm1l) expm1l __attribute__((weak, alias("__expm1l")));

#endif
