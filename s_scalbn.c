/* @(#)s_scalbn.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/* 
 * scalbn (double x, int n)
 * scalbn(x,n) returns x* 2**n  computed by  exponent  
 * manipulation rather than by actually performing an 
 * exponentiation or a multiplication.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_scalbn

double __ieee754_scalbn(double x, int n)
{
	int32_t k, hx, lx;

	static const double two54 = 1.80143985094819840000e+16;		/* 0x43500000, 0x00000000 */
	static const double twom54 = 5.55111512312578270212e-17;	/* 0x3C900000, 0x00000000 */
	static const double hugeval = 1.0e+300;
	static const double tiny = 1.0e-300;

	GET_DOUBLE_WORDS(hx, lx, x);
	k = (hx >> IEEE754_DOUBLE_SHIFT) & IEEE754_DOUBLE_MAXEXP;		/* extract exponent */
	if (k == 0)
	{									/* 0 or subnormal x */
		if ((lx | (hx & IC(0x7fffffff))) == 0)
			return x;					/* +-0 */
		x *= two54;
		GET_HIGH_WORD(hx, x);
		k = ((hx & IC(0x7ff00000)) >> IEEE754_DOUBLE_SHIFT) - 54;
	}
	if (k == IEEE754_DOUBLE_MAXEXP)
		return x + x;					/* NaN or Inf */
	if ((int32_t)n < IC(-30000))
		return tiny * copysign(tiny, x);	/* underflow */
	if ((int32_t)n > IC(30000) || k + n > 0x7fe)
		return hugeval * copysign(hugeval, x);	/* overflow  */
	/* Now k and n are bounded we know that k = k+n does not
	   overflow.  */
	k = k + n;
	if (k > 0)							/* normal result */
	{
		SET_HIGH_WORD(x, (hx & UC(0x800fffff)) | (k << IEEE754_DOUBLE_SHIFT));
		return x;
	}
	if (k <= -54)
		return tiny * copysign(tiny, x);	/*underflow */
	k += 54;							/* subnormal result */
	SET_HIGH_WORD(x, (hx & UC(0x800fffff)) | (k << IEEE754_DOUBLE_SHIFT));
	return x * twom54;
}

#endif

double __scalbn(double x, int n)
{
	return __ieee754_scalbn(x, n);
}

__typeof(__scalbn) scalbn __attribute__((weak, alias("__scalbn")));
#ifdef __NO_LONG_DOUBLE_MATH
long double __scalbnl(long double x, int n) __attribute__((alias("__scalbn")));
__typeof(__scalbnl) scalbnl __attribute__((weak, alias("__scalbn")));
#endif


#if INT_MAX == LONG_MAX
double __scalbln(double x, long n) __attribute__((alias("__scalbn")));
double scalbln(double x, long n) __attribute__((weak, alias("__scalbln")));
#ifndef __have_fpu_scalbn
double __ieee754_scalbln(double x, long n) __attribute__((alias("__ieee754_scalbn")));
#endif
#ifdef __NO_LONG_DOUBLE_MATH
long double __scalblnl(long double x, long n) __attribute__((alias("__scalbln")));
long double scalblnl(long double x, long n) __attribute__((weak, alias("__scalblnl")));
#ifndef __have_fpu_scalbn
long double __ieee754_scalblnl(long double x, long n) __attribute__((alias("__ieee754_scalbln")));
#endif
#endif
#endif
