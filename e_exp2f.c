/* Single-precision floating point 2^x.
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
   It has been slightly modified to compute 2^x instead of e^x, and for
   single-precision.
   */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_exp2

#undef abs
#define abs(x) ((x) < 0 ? -(x) : (x))

float __ieee754_exp2f(float x)
{
	static const float himark = (float) FLT_MAX_EXP;
	static const float lomark = (float) (FLT_MIN_EXP - FLT_MANT_DIG - 1);

	static const volatile float TWOM100 = 7.88860905e-31;
	static const volatile float TWO127 = 1.7014118346e+38;

#include "t_exp2f.h"

	/* Check for usual case.  */
	if (isless(x, himark) && isgreaterequal(x, lomark))
	{
		static const float THREEp14 = 49152.0;
		int32_t tval, unsafe;
		float rx, x22, result;
		float ex2_u, scale_u;
		uint32_t exp;
		
		if (__ieee754_fabsf(x) < FLT_EPSILON / 4.0f)
			return 1.0f + x;

		{
			SAVE_AND_SET_ROUND(FE_TONEAREST);

			/* 1. Argument reduction.
			   Choose integers ex, -128 <= t < 128, and some real
			   -1/512 <= x1 <= 1/512 so that
			   x = ex + t/512 + x1.

			   First, calculate rx = ex + t/256.  */
			rx = x + THREEp14;
			rx -= THREEp14;
			x -= rx;					/* Compute x=x1. */
			/* Compute tval = (ex*256 + t)+128.
			   Now, t = (tval mod 256)-128 and ex=tval/256  [that's mod, NOT %;
			   and /-round-to-nearest not the usual c integer /].  */
			tval = (int32_t) (rx * 256.0f + 128.0f);
			
			/* 2. Adjust for accurate table entry.
			   Find e so that
			   x = ex + t/256 + e + x2
			   where -7e-4 < e < 7e-4, and
			   (float)(2^(t/256+e))
			   is accurate to one part in 2^-64.  */

			/* 'tval & 255' is the same as 'tval%256' except that it's always
			   positive.
			   Compute x = x2.  */
			x -= __exp2f_deltatable[tval & 255];

			/* 3. Compute ex2 = 2^(t/255+e+ex).  */
			ex2_u = __exp2f_atable[tval & 255];
			tval >>= 8;
			/* x2 is an integer multiple of 2^-30; avoid intermediate
			   underflow from the calculation of x22 * x.  */
			unsafe = abs(tval) >= -FLT_MIN_EXP - 32;
			GET_FLOAT_WORD(exp, ex2_u);
			exp = ((exp & UC(0x7F800000)) + ((tval >> unsafe) << IEEE754_FLOAT_SHIFT)) | (exp & ~UC(0x7F800000));
			SET_FLOAT_WORD(ex2_u, exp);
			exp = UC(0x3f800000) + ((tval - (tval >> unsafe)) << IEEE754_FLOAT_SHIFT);
			SET_FLOAT_WORD(scale_u, exp);

			/* 4. Approximate 2^x2 - 1, using a second-degree polynomial,
			   with maximum error in [-2^-9 - 2^-14, 2^-9 + 2^-14]
			   less than 1.3e-10.  */

			x22 = (.24022656679f * x + .69314736128f) * ex2_u;
			RESTORE_ROUND();
		}

		/* 5. Return (2^x2-1) * 2^(t/512+e+ex) + 2^(t/512+e+ex).  */
		result = x22 * x + ex2_u;

		if (!unsafe)
			return result;
		return result * scale_u;
	}
	/* Exceptional cases:  */
	if (isless(x, himark))
	{
		if (isinf(x))
			/* e^-inf == 0, with no error.  */
			return 0;
		/* Underflow */
		return TWOM100 * TWOM100;
	}
	/* Return x, if x is a NaN or Inf; or overflow, otherwise.  */
	return TWO127 * x;
}

#undef abs

#endif

float __exp2f(float x)
{
	float z = __ieee754_exp2f(x);

	if (!isfinite(z) && isfinite(x) && _LIB_VERSION != _IEEE_)
		/* exp2 overflow: 144, exp2 underflow: 145 */
		return __kernel_standard_f(x, x, z, signbit(x) ? KMATHERRF_EXP2_UNDERFLOW : KMATHERRF_EXP2_OVERFLOW);

	return z;
}

__typeof(__exp2f) exp2f __attribute__((weak, alias("__exp2f")));
__typeof(__exp2f) __pow2f __attribute__((alias("__exp2f")));
__typeof(__pow2f) pow2f __attribute__((weak, alias("__pow2f")));
