
/* @(#)w_jn.c 1.3 95/01/18 */
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
 * wrapper jn(int n, double x), yn(int n, double x)
 * floating point Bessel's function of the 1st and 2nd kind
 * of order n
 *          
 * Special cases:
 *	y0(0)=y1(0)=yn(n,0) = -inf with division by zero signal;
 *	y0(-ve)=y1(-ve)=yn(n,-ve) are NaN with invalid signal.
 * Note 2. About jn(n,x), yn(n,x)
 *	For n=0, j0(x) is called,
 *	for n=1, j1(x) is called,
 *	for n<x, forward recursion us used starting
 *	from values of j0(x) and j1(x).
 *	for n>x, a continued fraction approximation to
 *	j(n,x)/j(n-1,x) is evaluated and then backward
 *	recursion is used starting from a supposed value
 *	for j(n,x). The resulting value of j(0,x) is
 *	compared with the actual value to correct the
 *	supposed value of j(n,x).
 *
 *	yn(n,x) is similar in all respects, except
 *	that forward recursion is used for all
 *	values of n>1.
 *	
 */

#include "fdlibm.h"

/* wrapper jn */
double jn(int n, double x)	/* wrapper jn */
{
	if (isgreater(__ieee754_fabs(x), X_TLOSS) && _LIB_VERSION != _IEEE_ && _LIB_VERSION != _POSIX_)
		/* jn(n,|x|>X_TLOSS) */
		return __kernel_standard(n, x, 0.0, KMATHERR_JN_TLOSS);

	return __ieee754_jn(n, x);
}

/* wrapper yn */
double yn(int n, double x)	/* wrapper yn */
{
	if ((islessequal(x, 0.0) || isgreater(x, X_TLOSS)) && _LIB_VERSION != _IEEE_)
	{
		if (x < 0.0)
		{
			/* d = zero/(x-x) */
			feraiseexcept(FE_INVALID);
			return __kernel_standard(n, x, -HUGE_VAL, KMATHERR_YN_MINUS);
		} else if (x == 0.0)
			/* d = -one/(x-x) */
			return __kernel_standard(n, x, -HUGE_VAL, KMATHERR_YN_ZERO);
		else if (_LIB_VERSION != _POSIX_)
			/* yn(n,x>X_TLOSS) */
			return __kernel_standard(n, x, 0.0, KMATHERR_YN_TLOSS);
	}

	return __ieee754_yn(n, x);
}
