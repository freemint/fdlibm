/* Double-precision floating point 2^x.
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

/* The basic design here is from
   Shmuel Gal and Boris Bachelis, "An Accurate Elementary Mathematical
   Library for the IEEE Floating Point Standard", ACM Trans. Math. Soft.,
   17 (1), March 1991, pp. 26-45.
   It has been slightly modified to compute 2^x instead of e^x.
   */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_exp2

#undef abs
#define abs(x) ((x) < 0 ? -(x) : (x))

double __ieee754_exp2(double x)
{
	static const double himark = (double) DBL_MAX_EXP;
	static const double lomark = (double) (DBL_MIN_EXP - DBL_MANT_DIG - 1);

#include "t_exp2.h"

	/* Check for usual case.  */
	if (isless(x, himark))
	{
		static const double THREEp42 = 13194139533312.0;

		int32_t tval, unsafe;
		double rx, x22, result;
		double ex2_u, scale_u;
		uint32_t exp;

		/* Exceptional cases:  */
		if (!isgreaterequal(x, lomark))
		{
			if (isinf(x))
				/* e^-inf == 0, with no error.  */
				return 0;
			/* Underflow */
			feraiseexcept(FE_UNDERFLOW);
			return 0;
		}

		if (__ieee754_fabs(x) < DBL_EPSILON / 4.0)
			return 1.0 + x;

		{
			SAVE_AND_SET_ROUND(FE_TONEAREST);

			/* 1. Argument reduction.
			   Choose integers ex, -256 <= t < 256, and some real
			   -1/1024 <= x1 <= 1024 so that
			   x = ex + t/512 + x1.

			   First, calculate rx = ex + t/512.  */
			rx = x + THREEp42;
			rx -= THREEp42;
			x -= rx;					/* Compute x=x1. */
			/* Compute tval = (ex*512 + t)+256.
			   Now, t = (tval mod 512)-256 and ex=tval/512  [that's mod, NOT %;
			   and /-round-to-nearest not the usual c integer /].  */
			tval = (int32_t) (rx * 512.0 + 256.0);

			/* 2. Adjust for accurate table entry.
			   Find e so that
			   x = ex + t/512 + e + x2
			   where -1e6 < e < 1e6, and
			   (double)(2^(t/512+e))
			   is accurate to one part in 2^-64.  */

			/* 'tval & 511' is the same as 'tval%512' except that it's always
			   positive.
			   Compute x = x2.  */
			x -= exp2_deltatable[tval & 511];

			/* 3. Compute ex2 = 2^(t/512+e+ex).  */
			ex2_u = exp2_accuratetable[tval & 511];
			tval >>= 9;
			/* x2 is an integer multiple of 2^-54; avoid intermediate
			   underflow from the calculation of x22 * x.  */
			unsafe = abs(tval) >= -DBL_MIN_EXP - 56;
			GET_HIGH_WORD(exp, ex2_u);
			exp = ((exp & UC(0x7ff00000)) + ((tval >> unsafe) << IEEE754_DOUBLE_SHIFT)) | (exp & ~UC(0x7ff00000));
			SET_HIGH_WORD(ex2_u, exp);
			exp = UC(0x3ff00000) + ((tval - (tval >> unsafe)) << IEEE754_DOUBLE_SHIFT);
			INSERT_WORDS(scale_u, exp, 0);

			/* 4. Approximate 2^x2 - 1, using a fourth-degree polynomial,
			   with maximum error in [-2^-10-2^-30,2^-10+2^-30]
			   less than 10^-19.  */

			x22 = (((.0096181293647031180 * x + .055504110254308625) * x + .240226506959100583) * x + .69314718055994495) * ex2_u;
			(void)math_opt_barrier(x22);
			RESTORE_ROUND();
		}

		/* 5. Return (2^x2-1) * 2^(t/512+e+ex) + 2^(t/512+e+ex).  */
		result = x22 * x + ex2_u;

		if (!unsafe)
			return result;
		return result * scale_u;
	}
	/* Return x, if x is a NaN or Inf; or overflow, otherwise.  */
	if (isinf(x))
		return HUGE_VAL;
	if (isnan(x))
		return x;
	feraiseexcept(FE_OVERFLOW);
	return HUGE_VAL;
}

#undef abs

#endif

double __exp2(double x)
{
	double z = __ieee754_exp2(x);

	if (!isfinite(z) && isfinite(x) && _LIB_VERSION != _IEEE_)
		/* exp2 overflow: 44, exp2 underflow: 45 */
		return __kernel_standard(x, x, z, signbit(x) ? KMATHERR_EXP2_UNDERFLOW : KMATHERR_EXP2_OVERFLOW);

	return z;
}


__typeof(__exp2) exp2 __attribute__((weak, alias("__exp2")));
__typeof(__exp2) __pow2 __attribute__((alias("__exp2")));
__typeof(__pow2) pow2 __attribute__((weak, alias("__pow2")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__exp2l) __exp2l __attribute__((alias("__exp2")));
__typeof(__exp2l) exp2l __attribute__((weak, alias("__exp2l")));
__typeof(__pow2l) __pow2l __attribute__((alias("__pow2")));
__typeof(__pow2l) pow2l __attribute__((weak, alias("__pow2l")));
#endif
