/* @(#)e_yn.c 5.1 93/09/24 */
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

/*
 * __ieee754_yn(n, x)
 * floating point Bessel's function of the 1st and 2nd kind
 * of order n
 *
 * Special cases:
 *	y0(0)=y1(0)=yn(n,0) = -inf with overflow signal;
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

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

double __ieee754_yn(int n, double x)
{
	int32_t i, hx, ix, lx;
	int32_t sign;
	double a, b, temp;

	static const double invsqrtpi = 5.64189583547756279280e-01;	/* 0x3FE20DD7, 0x50429B6D */
	static const double zero = 0.00000000000000000000e+00;
	
	GET_DOUBLE_WORDS(hx, lx, x);
	ix = IC(0x7fffffff) & hx;
	/* if Y(n,NaN) is NaN */
	if ((ix | (((uint32_t) (lx | -lx)) >> 31)) > IC(0x7ff00000))
		return x + x;
	if ((ix | lx) == 0)
		return -HUGE_VAL + x; /* -inf and overflow exception.  */ ;
	if (hx < 0)
		return zero / zero;
	sign = 1;
	if (n < 0)
	{
		n = -n;
		sign = 1 - ((n & 1) << 1);
	}
	if (n == 0)
		return (__ieee754_y0(x));
	if (n == 1)
		return (sign * __ieee754_y1(x));
	if (ix == IC(0x7ff00000))
		return zero;
	if (ix >= IC(0x52D00000))
	{									/* x > 2**302 */
		/* (x >> n**2)
		 *      Jn(x) = cos(x-(2n+1)*pi/4)*sqrt(2/x*pi)
		 *      Yn(x) = sin(x-(2n+1)*pi/4)*sqrt(2/x*pi)
		 *      Let s=sin(x), c=cos(x),
		 *      xn=x-(2n+1)*pi/4, sqt2 = sqrt(2),then
		 *
		 *         n    sin(xn)*sqt2    cos(xn)*sqt2
		 *      ----------------------------------
		 *         0     s-c         c+s
		 *         1    -s-c        -c+s
		 *         2    -s+c        -c-s
		 *         3     s+c         c-s
		 */
		double c;
		double s;

		__ieee754_sincos(x, &s, &c);
		switch (n & 3)
		{
		case 0:
			temp = s - c;
			break;
		case 1:
			temp = -s - c;
			break;
		case 2:
			temp = -s + c;
			break;
		case 3:
			temp = s + c;
			break;
		}
		b = invsqrtpi * temp / __ieee754_sqrt(x);
	} else
	{
		uint32_t high;

		a = __ieee754_y0(x);
		b = __ieee754_y1(x);
		/* quit if b is -inf */
		GET_HIGH_WORD(high, b);
		for (i = 1; i < n && high != UC(0xfff00000); i++)
		{
			temp = b;
			b = ((double) (i + i) / x) * b - a;
			GET_HIGH_WORD(high, b);
			a = temp;
		}
	}
	if (sign > 0)
		return b;
	return -b;
}

/* wrapper yn */
double __yn(int n, double x)
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

__typeof(__yn) yn __attribute__((weak, alias("__yn")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__ynl) __ynl __attribute__((alias("__yn")));
__typeof(__ynl) ynl __attribute__((weak, alias("__ynl")));
#endif
