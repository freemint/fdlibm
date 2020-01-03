/* Single-precision floating point e^x.
   Copyright (C) 1997-2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Geoffrey Keating <geoffk@ozemail.com.au>

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/* How this works:

   The input value, x, is written as

   x = n * ln(2) + t/512 + delta[t] + x;

   where:
   - n is an integer, 127 >= n >= -150;
   - t is an integer, 177 >= t >= -177
   - delta is based on a table entry, delta[t] < 2^-28
   - x is whatever is left, |x| < 2^-10

   Then e^x is approximated as

   e^x = 2^n ( e^(t/512 + delta[t])
	       + ( e^(t/512 + delta[t])
		   * ( p(x + delta[t] + n * ln(2)) - delta ) ) )

   where
   - p(x) is a polynomial approximating e(x)-1;
   - e^(t/512 + delta[t]) is obtained from a table.

   The table used is the same one as for the double precision version;
   since we have the table, we might as well use it.

   It turns out to be faster to do calculations in double precision than
   to perform an 'accurate table method' expf, because of the range reduction
   overhead (compare exp2f).
   */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_exp

float __ieee754_expf(float x)
{
	static const float himark = 88.72283935546875f;
	static const float lomark = -103.972084045410f;

#include "t_expf.h"

	/* Check for usual case.  */
	if (isless(x, himark) && isgreater(x, lomark))
	{
		static const float THREEp42 = 13194139533312.0f;
		static const float THREEp22 = 12582912.0f;

		/* 1/ln(2).  */
		static const float m_1_ln2 = 1.44269502163f;

		/* ln(2) */
		static const double m_ln2 = .6931471805599452862;

		int tval;
		volatile double x22, t, result, dx;
		volatile float n, delta;
		double ex2_u;
		
		{
			int32_t exponent;
			SAVE_AND_SET_ROUND(FE_TONEAREST);
			
			/* Calculate n.  */
			n = x * m_1_ln2 + THREEp22;
			n -= THREEp22;
			dx = x - n * m_ln2;

			/* Calculate t/512.  */
			t = dx + THREEp42;
			t -= THREEp42;
			dx -= t;

			/* Compute tval = t. */
			tval = (int) (t * 512.0);

			if (t >= 0)
				delta = -__exp_deltatable[tval];
			else
				delta = __exp_deltatable[-tval];

			/* Compute ex2 = 2^n e^(t/512+delta[t]).  */
			ex2_u = __exp_atable[tval + 177];
			GET_HIGH_WORD(exponent, ex2_u);
			exponent += ((int32_t) n) << IEEE754_DOUBLE_SHIFT;
			SET_HIGH_WORD(ex2_u, exponent);
			
			/* Approximate e^(dx+delta) - 1, using a second-degree polynomial,
			   with maximum error in [-2^-10-2^-28,2^-10+2^-28]
			   less than 5e-11.  */
			x22 = (0.5000000496709180453 * dx + 1.0000001192102037084) * dx + delta;
			RESTORE_ROUND();
		}

		/* Return result.  */
		result = x22 * ex2_u + ex2_u;
		return (float) result;
	}
	/* Exceptional cases:  */
	if (isless(x, himark))
	{
		if (isinf(x))
			/* e^-inf == 0, with no error.  */
			return 0;
		/* Underflow */
		feraiseexcept(FE_UNDERFLOW);
		return 0;
	}
	/* Return x, if x is a NaN or Inf; or overflow, otherwise.  */
	if (isinf(x))
		return HUGE_VALF;
	if (isnan(x))
		return x;
	feraiseexcept(FE_OVERFLOW);
	return HUGE_VALF;
}

#endif

/* wrapper exp */
float __expf(float x)
{
	float z = __ieee754_expf(x);

	if ((!isfinite(z) || z == 0.0f) && isfinite(x) && _LIB_VERSION != _IEEE_)
		return __kernel_standard_f(x, x, z, signbit(x) ? KMATHERRF_EXP_UNDERFLOW : KMATHERRF_EXP_OVERFLOW);

	return z;
}

__typeof(__expf) expf __attribute__((weak, alias("__expf")));
