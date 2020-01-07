/* Implementation of gamma function according to ISO C.
   Copyright (C) 1997-2015 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1997.

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

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

/* Calculate X * Y exactly and store the result in *HI + *LO.  It is
   given that the values are small enough that no overflow occurs and
   large enough (or zero) that no underflow occurs.  */

static void gamma_mul_splitl(long double *hi, long double *lo, long double x, long double y)
{
#ifdef __FP_FAST_FMAL
	/* Fast built-in fused multiply-add.  */
	*hi = x * y;
	*lo = __builtin_fmal(x, y, -*hi);
#elif defined FP_FAST_FMAL
	/* Fast library fused multiply-add, compiler before GCC 4.6.  */
	*hi = x * y;
	*lo = fmal(x, y, -*hi);
#else
	/* Apply Dekker's algorithm.  */
	long double x1, y1;
	long double x2, y2;
	
	*hi = x * y;
#define C 4294967297.0L /* ((1LL << ((LDBL_MANT_DIG + 1) / 2)) + 1) */
	x1 = x * C;
	y1 = y * C;

#undef C
	x1 = (x - x1) + x1;
	y1 = (y - y1) + y1;
	x2 = x - x1;
	y2 = y - y1;

	*lo = (((x1 * y1 - *hi) + x1 * y2) + x2 * y1) + x2 * y2;
#endif
}

/* Compute the product of X + X_EPS, X + X_EPS + 1, ..., X + X_EPS + N
   - 1, in the form R * (1 + *EPS) where the return value R is an
   approximation to the product and *EPS is set to indicate the
   approximate error in the return value.  X is such that all the
   values X + 1, ..., X + N - 1 are exactly representable, and X_EPS /
   X is small enough that factors quadratic in it can be
   neglected.  */

static long double __gamma_productl(long double x, long double x_eps, int n, long double *eps)
{
	long double ret = x;
	int i;
	
	SAVE_AND_SET_ROUND(FE_TONEAREST);
	*eps = x_eps / x;
	for (i = 1; i < n; i++)
	{
		long double lo;
		*eps += x_eps / (x + i);

		gamma_mul_splitl(&ret, &lo, ret, x + i);
		*eps += lo / ret;
	}
	RESTORE_ROUND();
	return ret;
}

/* Return gamma (X), for positive X less than 1766, in the form R *
   2^(*EXP2_ADJ), where R is the return value and *EXP2_ADJ is set to
   avoid overflow or underflow in intermediate calculations.  */

static long double gammal_positive(long double x, int *exp2_adj)
{
	int local_signgam;

	/* Coefficients B_2k / 2k(2k-1) of x^-(2k-1) inside exp in Stirling's
	   approximation to gamma function.  */
	
	static const long double gamma_coeff[] = {
		 0x1.5555555555555556p-4L,
		-0xb.60b60b60b60b60bp-12L,
		 0x3.4034034034034034p-12L,
		-0x2.7027027027027028p-12L,
		 0x3.72a3c5631fe46aep-12L,
		-0x7.daac36664f1f208p-12L,
		 0x1.a41a41a41a41a41ap-8L,
		-0x7.90a1b2c3d4e5f708p-8L
	};
	
#define NCOEFF (sizeof (gamma_coeff) / sizeof (gamma_coeff[0]))

	if (x < 0.5L)
	{
		*exp2_adj = 0;
		return __ieee754_expl(__ieee754_lgammal_r(x + 1.0L, &local_signgam)) / x;
	} else if (x <= 1.5L)
	{
		*exp2_adj = 0;
		return __ieee754_expl(__ieee754_lgammal_r(x, &local_signgam));
	} else if (x < 7.5L)
	{
		/* Adjust into the range for using exp (lgamma).  */
		long double n = __ieee754_ceill(x - 1.5L);
		long double x_adj = x - n;
		long double eps;
		long double prod = __gamma_productl(x_adj, 0, n, &eps);

		*exp2_adj = 0;
		return __ieee754_expl(__ieee754_lgammal_r(x_adj, &local_signgam)) * prod * (1.0L + eps);
	} else
	{
		long double eps = 0;
		long double x_eps = 0;
		long double x_adj = x;
		long double prod = 1;
		long double exp_adj;
		long double x_adj_int;
		long double x_adj_frac;
		int x_adj_log2;
		long double x_adj_mant;
		long double ret;
		long double bsum;
		long double x_adj2;
		size_t i;
		
		if (x < 13.0L)
		{
			/* Adjust into the range for applying Stirling's
			   approximation.  */
			long double n = __ieee754_ceill(13.0L - x);

			x_adj = x + n;
			x_eps = (x - (x_adj - n));
			prod = __gamma_productl(x_adj - n, x_eps, n, &eps);
		}
		/* The result is now gamma (X_ADJ + X_EPS) / (PROD * (1 + EPS)).
		   Compute gamma (X_ADJ + X_EPS) using Stirling's approximation,
		   starting by computing pow (X_ADJ, X_ADJ) with a power of 2
		   factored out.  */
		exp_adj = -eps;
		x_adj_int = __ieee754_roundl(x_adj);
		x_adj_frac = x_adj - x_adj_int;
		x_adj_mant = __ieee754_frexpl(x_adj, &x_adj_log2);

		if (x_adj_mant < M_SQRT1_2l)
		{
			x_adj_log2--;
			x_adj_mant *= 2.0L;
		}
		*exp2_adj = x_adj_log2 * (int) x_adj_int;
		ret = __ieee754_powl(x_adj_mant, x_adj)
					  * __ieee754_exp2l(x_adj_log2 * x_adj_frac)
					  * __ieee754_expl(-x_adj) * __ieee754_sqrtl(2 * M_PIl / x_adj) / prod;
		exp_adj += x_eps * __ieee754_logl(x_adj);
		bsum = gamma_coeff[NCOEFF - 1];

		x_adj2 = x_adj * x_adj;

		for (i = 1; i <= NCOEFF - 1; i++)
			bsum = bsum / x_adj2 + gamma_coeff[NCOEFF - 1 - i];
		exp_adj += bsum / x_adj;
		return ret + ret * __ieee754_expm1l(exp_adj);
	}
#undef NCOEFF
}


long double __ieee754_tgammal_r(long double x, int *signgamp)
{
	uint32_t es, hx, lx;
	long double ret;
	
	GET_LDOUBLE_WORDS(es, hx, lx, x);

	if (((es & IEEE854_LONG_DOUBLE_MAXEXP) | hx | lx) == 0)
	{
		/* Return value for x == 0 is Inf with divide by zero exception.  */
		*signgamp = 0;
		return 1.0L / x;
	}
	if (es == UC(0xffffffff) && ((hx & UC(0x7fffffff)) | lx) == 0)
	{
		/* x == -Inf.  According to ISO this is NaN.  */
		*signgamp = 0;
		return x - x;
	}
	if ((es & IEEE854_LONG_DOUBLE_MAXEXP) == IEEE854_LONG_DOUBLE_MAXEXP)
	{
		/* Positive infinity (return positive infinity) or NaN (return
		   NaN).  */
		*signgamp = 0;
		return x + x;
	}
	if ((es & 0x8000) != 0 && __ieee754_rintl(x) == x)
	{
		/* Return value for integer x < 0 is NaN with invalid exception.  */
		*signgamp = 0;
		return (x - x) / (x - x);
	}

	if (x >= 1756.0L)
	{
		/* Overflow.  */
		*signgamp = 0;
		return LDBL_MAX * LDBL_MAX;
	} else
	{
		SAVE_AND_SET_ROUND(FE_TONEAREST);
		if (x > 0.0L)
		{
			int exp2_adj;
			
			*signgamp = 0;
			ret = gammal_positive(x, &exp2_adj);
			ret = __ieee754_scalbnl(ret, exp2_adj);
		} else if (x >= -LDBL_EPSILON / 4.0L)
		{
			*signgamp = 0;
			ret = 1.0L / x;
		} else
		{
			long double tx = __ieee754_truncl(x);
			
			*signgamp = (tx == 2.0L * __ieee754_truncl(tx / 2.0L)) ? -1 : 1;
			if (x <= -1766.0L)
				/* Underflow.  */
				ret = LDBL_MIN * LDBL_MIN;
			else
			{
				long double frac = tx - x;
				long double sinpix;
				int exp2_adj;
				
				if (frac > 0.5L)
					frac = 1.0L - frac;
				sinpix = (frac <= 0.25L ? __ieee754_sinl(M_PIl * frac) : __ieee754_cosl(M_PIl * (0.5L - frac)));
				
				ret = M_PIl / (-x * sinpix * gammal_positive(-x, &exp2_adj));
				ret = __ieee754_scalbnl(ret, -exp2_adj);
			}
		}
		RESTORE_ROUND();
	}
	if (isinf(ret) && x != 0.0L)
	{
		if (*signgamp < 0)
		{
			ret = -copysignl(LDBL_MAX, ret) * LDBL_MAX;
			ret = -ret;
		} else
		{
			ret = copysignl(LDBL_MAX, ret) * LDBL_MAX;
		}
	} else if (ret == 0.0L)
	{
		if (*signgamp < 0)
		{
			ret = -copysignl(LDBL_MIN, ret) * LDBL_MIN;
			ret = -ret;
		} else
		{
			ret = copysignl(LDBL_MIN, ret) * LDBL_MIN;
		}
	}
	return ret;
}

#endif
