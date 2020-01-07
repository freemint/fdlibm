/* Implementation of gamma function according to ISO C.
   Copyright (C) 1997-2013 Free Software Foundation, Inc.
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
#include "fpu_ctrl.h"

/* Compute the product of X + X_EPS, X + X_EPS + 1, ..., X + X_EPS + N
   - 1, in the form R * (1 + *EPS) where the return value R is an
   approximation to the product and *EPS is set to indicate the
   approximate error in the return value.  X is such that all the
   values X + 1, ..., X + N - 1 are exactly representable, and X_EPS /
   X is small enough that factors quadratic in it can be
   neglected.  */

static float __gamma_productf(float x, float x_eps, int n, float *eps)
{
	double x_full = (double) x + (double) x_eps;
	double ret = x_full;
	int i;
	volatile float fret;
	
	for (i = 1; i < n; i++)
		ret *= x_full + i;

	fret = (float)ret;

	*eps = (ret - (double)fret) / (double)fret;

	return fret;
}


/* Return gamma (X), for positive X less than 42, in the form R *
   2^(*EXP2_ADJ), where R is the return value and *EXP2_ADJ is set to
   avoid overflow or underflow in intermediate calculations.  */

static float gammaf_positive(float x, int *exp2_adj)
{
	int local_signgam;

	/* Coefficients B_2k / 2k(2k-1) of x^-(2k-1) inside exp in Stirling's
	   approximation to gamma function.  */
	
	static const float gamma_coeff[] = {
		 0x1.555556p-4f,
		-0xb.60b61p-12f,
		 0x3.403404p-12f
	};
	
#define NCOEFF (sizeof (gamma_coeff) / sizeof (gamma_coeff[0]))
	
	if (x < 0.5f)
	{
		*exp2_adj = 0;
		return __ieee754_expf(__ieee754_lgammaf_r(x + 1.0f, &local_signgam)) / x;
	} else if (x <= 1.5f)
	{
		*exp2_adj = 0;
		return __ieee754_expf(__ieee754_lgammaf_r(x, &local_signgam));
	} else if (x < 2.5f)
	{
		float x_adj = x - 1.0f;
		*exp2_adj = 0;

		return __ieee754_expf(__ieee754_lgammaf_r(x_adj, &local_signgam)) * x_adj;
	} else
	{
		float eps = 0;
		float x_eps = 0;
		float x_adj = x;
		float prod = 1;
		float exp_adj;
		float x_adj_int;
		float x_adj_frac;
		int x_adj_log2;
		float x_adj_mant;
		float ret;
		float bsum;
		float x_adj2;
		size_t i;
		
		if (x < 4.0f)
		{
			/* Adjust into the range for applying Stirling's
			   approximation.  */
			float n = __ieee754_ceilf(4.0f - x);

			volatile float x_tmp = x + n;

			x_adj = x_tmp;
			x_eps = (x - (x_adj - n));
			prod = __gamma_productf(x_adj - n, x_eps, n, &eps);
		}
		/* The result is now gamma (X_ADJ + X_EPS) / (PROD * (1 + EPS)).
		   Compute gamma (X_ADJ + X_EPS) using Stirling's approximation,
		   starting by computing pow (X_ADJ, X_ADJ) with a power of 2
		   factored out.  */
		exp_adj = -eps;
		x_adj_int = __ieee754_roundf(x_adj);
		x_adj_frac = x_adj - x_adj_int;
		x_adj_mant = __ieee754_frexpf(x_adj, &x_adj_log2);

		if (x_adj_mant < (float) M_SQRT1_2)
		{
			x_adj_log2--;
			x_adj_mant *= 2.0f;
		}
		*exp2_adj = x_adj_log2 * (int) x_adj_int;
		ret = __ieee754_powf(x_adj_mant, x_adj)
					 * __ieee754_exp2f(x_adj_log2 * x_adj_frac)
					 * __ieee754_expf(-x_adj) * __ieee754_sqrtf(2.0f * (float) M_PI / x_adj) / prod;
		exp_adj += x_eps * __ieee754_logf(x_adj);
		bsum = gamma_coeff[NCOEFF - 1];

		x_adj2 = x_adj * x_adj;

		for (i = 1; i <= NCOEFF - 1; i++)
			bsum = bsum / x_adj2 + gamma_coeff[NCOEFF - 1 - i];
		exp_adj += bsum / x_adj;
		ret += ret * __ieee754_expm1f(exp_adj);
		return ret;
	}
#undef NCOEFF
}


float __ieee754_tgammaf_r(float x, int *signgamp)
{
	int32_t hx;
	volatile float ret;

	GET_FLOAT_WORD(hx, x);

	if ((hx & IC(0x7fffffff)) == 0)
	{
		/* Return value for x == 0 is Inf with divide by zero exception.  */
		*signgamp = 0;
		return 1.0F / x;
	}
	if (hx < 0 && (uint32_t) hx < UC(0xff800000) && __ieee754_rintf(x) == x)
	{
		/* Return value for integer x < 0 is NaN with invalid exception.  */
		*signgamp = 0;
		return (x - x) / (x - x);
	}
	if (hx == IC(0xff800000))
	{
		/* x == -Inf.  According to ISO this is NaN.  */
		*signgamp = 0;
		return x - x;
	}
	if ((hx & IC(0x7f800000)) == IC(0x7f800000))
	{
		/* Positive infinity (return positive infinity) or NaN (return
		   NaN).  */
		*signgamp = 0;
		return x + x;
	}

	if (x >= 36.0f)
	{
		/* Overflow.  */
		*signgamp = 0;
		ret = FLT_MAX * FLT_MAX;
		return ret;
	} else
	{
		SAVE_AND_SET_ROUND(FE_TONEAREST);
		if (x > 0.0f)
		{
			int exp2_adj;
			float tret;
			
			*signgamp = 0;
			tret = gammaf_positive(x, &exp2_adj);
			ret = __ieee754_scalbnf(tret, exp2_adj);
		} else if (x >= -FLT_EPSILON / 4.0f)
		{
			*signgamp = 0;
			ret = 1.0f / x;
		} else
		{
			float tx = __ieee754_truncf(x);
			
			*signgamp = (tx == 2.0f * __ieee754_truncf(tx / 2.0f)) ? -1 : 1;
			if (x <= -42.0f)
				/* Underflow.  */
				ret = FLT_MIN * FLT_MIN;
			else
			{
				float frac = tx - x;
				float sinpix;
				int exp2_adj;
				float tret;
				
				if (frac > 0.5f)
					frac = 1.0f - frac;
				sinpix = (frac <= 0.25f ? __ieee754_sinf((float) M_PI * frac) : __ieee754_cosf((float) M_PI * (0.5f - frac)));
				
				tret = (float) M_PI / (-x * sinpix * gammaf_positive(-x, &exp2_adj));
				ret = __ieee754_scalbnf(tret, -exp2_adj);
			}
		}
		RESTORE_ROUND();
	}
	if (isinf(ret) && x != 0.0f)
	{
		if (*signgamp < 0)
		{
			ret = -copysignf(FLT_MAX, ret) * FLT_MAX;
			ret = -ret;
		} else
		{
			ret = copysignf(FLT_MAX, ret) * FLT_MAX;
		}
	} else if (ret == 0.0f)
	{
		if (*signgamp < 0)
		{
			ret = -copysignf(FLT_MIN, ret) * FLT_MIN;
			ret = -ret;
		} else
		{
			ret = copysignf(FLT_MIN, ret) * FLT_MIN;
		}
	}
	return ret;
}
