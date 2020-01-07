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

/* Calculate X * Y exactly and store the result in *HI + *LO.  It is
   given that the values are small enough that no overflow occurs and
   large enough (or zero) that no underflow occurs.  */

static void gamma_mul_split(double *hi, double *lo, double x, double y)
{
#ifdef __FP_FAST_FMA
	/* Fast built-in fused multiply-add.  */
	*hi = x * y;
	*lo = __builtin_fma(x, y, -*hi);
#elif defined FP_FAST_FMA
	/* Fast library fused multiply-add, compiler before GCC 4.6.  */
	*hi = x * y;
	*lo = fma(x, y, -*hi);
#else
	double x1, y1;
	double x2, y2;
	volatile double tmp, tmp2;
	
	/* Apply Dekker's algorithm.  */
	tmp = x * y;
	*hi = tmp;
# define C 134217729.0 /* (((int32_t)1 << ((DBL_MANT_DIG + 1) / 2)) + 1) */
	tmp = x * C;
	x1 = tmp;
	tmp = y * C;
	y1 = tmp;
	
# undef C
	tmp = x - x1;
	tmp = tmp + x1;
	x1 = tmp;
	tmp = y - y1;
	tmp = tmp + y1;
	y1 = tmp;
	tmp = x - x1;
	x2 = tmp;
	tmp = y - y1;
	y2 = tmp;
	
	tmp = x1 * y1;
	tmp = tmp - *hi;
	tmp2 = x1 * y2;
	tmp += tmp2;
	tmp2 = x2 * y1;
	tmp += tmp2;
	tmp2 = x2 * y2;
	tmp += tmp2;
	*lo = tmp;
#endif
}


/* Compute the product of X + X_EPS, X + X_EPS + 1, ..., X + X_EPS + N
   - 1, in the form R * (1 + *EPS) where the return value R is an
   approximation to the product and *EPS is set to indicate the
   approximate error in the return value.  X is such that all the
   values X + 1, ..., X + N - 1 are exactly representable, and X_EPS /
   X is small enough that factors quadratic in it can be
   neglected.  */

static double __gamma_product(double x, double x_eps, int n, double *eps)
{
	double ret = x;
	int i;
	
	SAVE_AND_SET_ROUND(FE_TONEAREST);
	*eps = x_eps / x;
	for (i = 1; i < n; i++)
	{
		double lo;
		double tmp = x + i;
		*eps += x_eps / tmp;

		gamma_mul_split(&ret, &lo, ret, tmp);
		tmp = lo / ret;
		*eps += tmp;
	}
	RESTORE_ROUND();
	return ret;
}


#ifdef __i386__
static __always_inline void libc_feholdexcept_setround_387_prec (fpu_control_t cw, int r)
{
	cw &= ~(FE_ROUNDING_MASK | FE_PRECISION_MASK);
	cw |= r | 0x3f;
	_FPU_SETCW (cw);
}

#define SET_ROUND_53BIT(r) \
	fpu_control_t cw; \
	_FPU_GETCW(cw); \
	libc_feholdexcept_setround_387_prec(cw, r | _FPU_DOUBLE)

#define RESTORE_ROUND_PREC() \
	_FPU_SETCW (cw)
#endif


#ifdef __mc68000__
static __inline void libc_feholdexcept_setround_68k_prec (fpu_control_t cw, int r)
{
	cw &= ~(FE_ROUNDING_MASK | FE_PRECISION_MASK);
	cw &= ~(FE_ALL_EXCEPT << 6);
	cw |= r;
	_FPU_SETCW (cw);
}

#define SET_ROUND_53BIT(r) \
	fpu_control_t cw; \
	_FPU_GETCW(cw); \
	libc_feholdexcept_setround_68k_prec(cw, r | _FPU_DOUBLE)

#define RESTORE_ROUND_PREC() \
	_FPU_SETCW (cw)
#endif

#ifndef SET_ROUND_53BIT
#  define SET_ROUND_53BIT(r) SAVE_AND_SET_ROUND(r)
#  define RESTORE_ROUND_PREC() RESTORE_ROUND()
#endif


/* Return gamma (X), for positive X less than 184, in the form R *
   2^(*EXP2_ADJ), where R is the return value and *EXP2_ADJ is set to
   avoid overflow or underflow in intermediate calculations.  */

static double gamma_positive(double x, int *exp2_adj)
{
	int local_signgam;

	/* Coefficients B_2k / 2k(2k-1) of x^-(2k-1) inside exp in Stirling's
	   approximation to gamma function.  */
	
	static const double gamma_coeff[] = {
		 0x1.5555555555555p-4,
		-0xb.60b60b60b60b8p-12,
		 0x3.4034034034034p-12,
		-0x2.7027027027028p-12,
		 0x3.72a3c5631fe46p-12,
		-0x7.daac36664f1f4p-12
	};
	
#define NCOEFF (sizeof (gamma_coeff) / sizeof (gamma_coeff[0]))
	
	if (x < 0.5)
	{
		*exp2_adj = 0;
		return __ieee754_exp(__ieee754_lgamma_r(x + 1, &local_signgam)) / x;
	} else if (x <= 1.5)
	{
		*exp2_adj = 0;
		return __ieee754_exp(__ieee754_lgamma_r(x, &local_signgam));
	} else if (x < 6.5)
	{
		/* Adjust into the range for using exp (lgamma).  */
		double n = __ieee754_ceil(x - 1.5);
		double x_adj = x - n;
		double eps;
		double prod = __gamma_product(x_adj, 0, n, &eps);

		*exp2_adj = 0;
		return __ieee754_exp(__ieee754_lgamma_r(x_adj, &local_signgam)) * prod * (1.0 + eps);
	} else
	{
		double eps = 0;
		double x_eps = 0;
		double x_adj = x;
		double prod = 1;
		double exp_adj;
		double x_adj_int;
		double x_adj_frac;
		int x_adj_log2;
		double x_adj_mant;
		double ret;
		double bsum;
		double x_adj2;
		size_t i;
		
		SET_ROUND_53BIT(FE_TONEAREST);
		
		if (x < 12.0)
		{
			/* Adjust into the range for applying Stirling's
			   approximation.  */
			double n = __ieee754_ceil(12.0 - x);
			volatile double x_tmp = x + n;

			x_adj = x_tmp;
			x_eps = (x - (x_adj - n));
			prod = __gamma_product(x_adj - n, x_eps, n, &eps);
		}
		/* The result is now gamma (X_ADJ + X_EPS) / (PROD * (1 + EPS)).
		   Compute gamma (X_ADJ + X_EPS) using Stirling's approximation,
		   starting by computing pow (X_ADJ, X_ADJ) with a power of 2
		   factored out.  */
		exp_adj = -eps;
		x_adj_int = __ieee754_round(x_adj);
		x_adj_frac = x_adj - x_adj_int;
		x_adj_mant = __ieee754_frexp(x_adj, &x_adj_log2);

		if (x_adj_mant < M_SQRT1_2)
		{
			x_adj_log2--;
			x_adj_mant *= 2.0;
		}
		*exp2_adj = x_adj_log2 * (int) x_adj_int;
		ret = __ieee754_pow(x_adj_mant, x_adj)
					  * __ieee754_exp2(x_adj_log2 * x_adj_frac)
					  * __ieee754_exp(-x_adj) * __ieee754_sqrt(2 * M_PI / x_adj) / prod;
		exp_adj += x_eps * __ieee754_log(x_adj);
		bsum = gamma_coeff[NCOEFF - 1];

		x_adj2 = x_adj * x_adj;

		for (i = 1; i <= NCOEFF - 1; i++)
			bsum = bsum / x_adj2 + gamma_coeff[NCOEFF - 1 - i];
		exp_adj += bsum / x_adj;
		ret += ret * __ieee754_expm1(exp_adj);
		RESTORE_ROUND_PREC();
		return ret;
	}
#undef NCOEFF
}


double __ieee754_tgamma_r(double x, int *signgamp)
{
	int32_t hx;
	uint32_t lx;
	volatile double ret;

	GET_DOUBLE_WORDS(hx, lx, x);

	if (((hx & IC(0x7fffffff)) | lx) == 0)
	{
		/* Return value for x == 0 is Inf with divide by zero exception.  */
		*signgamp = 0;
		return 1.0 / x;
	}
	if (hx < 0 && (uint32_t) hx < UC(0xfff00000) && __ieee754_rint(x) == x)
	{
		/* Return value for integer x < 0 is NaN with invalid exception.  */
		*signgamp = 0;
		return (x - x) / (x - x);
	}
	if ((uint32_t) hx == UC(0xfff00000) && lx == 0)
	{
		/* x == -Inf.  According to ISO this is NaN.  */
		*signgamp = 0;
		return x - x;
	}
	if ((hx & IC(0x7ff00000)) == IC(0x7ff00000))
	{
		/* Positive infinity (return positive infinity) or NaN (return
		   NaN).  */
		*signgamp = 0;
		return x + x;
	}

	if (x >= 172.0)
	{
		/* Overflow.  */
		*signgamp = 0;
		ret = DBL_MAX * DBL_MAX;
		return ret;
	} else
	{
		SAVE_AND_SET_ROUND(FE_TONEAREST);
		if (x > 0.0)
		{
			int exp2_adj;
			double tret;
			
			*signgamp = 0;
			tret = gamma_positive(x, &exp2_adj);
			ret = __ieee754_scalbn(tret, exp2_adj);
		} else if (x >= -DBL_EPSILON / 4.0)
		{
			*signgamp = 0;
			ret = 1.0 / x;
		} else
		{
			double tx = __ieee754_trunc(x);

			*signgamp = (tx == 2.0 * __ieee754_trunc(tx / 2.0)) ? -1 : 1;
			if (x <= -184.0)
				/* Underflow.  */
				ret = DBL_MIN * DBL_MIN;
			else
			{
				double frac = tx - x;
				double sinpix;
				int exp2_adj;
				double tret;
				
				if (frac > 0.5)
					frac = 1.0 - frac;
				sinpix = (frac <= 0.25 ? __ieee754_sin(M_PI * frac) : __ieee754_cos(M_PI * (0.5 - frac)));
				tret = M_PI / (-x * sinpix * gamma_positive(-x, &exp2_adj));
				ret = __ieee754_scalbn(tret, -exp2_adj);
			}
		}
		RESTORE_ROUND();
	}
	if (isinf(ret) && x != 0.0)
	{
		if (*signgamp < 0)
		{
			ret = -copysign(DBL_MAX, ret) * DBL_MAX;
			ret = -ret;
		} else
		{
			ret = copysign(DBL_MAX, ret) * DBL_MAX;
		}
	} else if (ret == 0.0)
	{
		if (*signgamp < 0)
		{
			ret = -copysign(DBL_MIN, ret) * DBL_MIN;
			ret = -ret;
		} else
		{
			ret = copysign(DBL_MIN, ret) * DBL_MIN;
		}
	}
	return ret;
}
