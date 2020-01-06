/* @(#)s_erf.c 5.1 93/09/24 */
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
/* Modified by Naohiko Shimizu/Tokai University, Japan 1997/08/25,
   for performance improvement on pipelined processors.
*/

/* double erf(double x)
 * double erfc(double x)
 *			     x
 *		      2      |\
 *     erf(x)  =  ---------  | exp(-t*t)dt
 *	 	   sqrt(pi) \|
 *			     0
 *
 *     erfc(x) =  1-erf(x)
 *  Note that
 *		erf(-x) = -erf(x)
 *		erfc(-x) = 2 - erfc(x)
 *
 * Method:
 *	1. For |x| in [0, 0.84375]
 *	    erf(x)  = x + x*R(x^2)
 *          erfc(x) = 1 - erf(x)           if x in [-.84375,0.25]
 *                  = 0.5 + ((0.5-x)-x*R)  if x in [0.25,0.84375]
 *	   where R = P/Q where P is an odd poly of degree 8 and
 *	   Q is an odd poly of degree 10.
 *						 -57.90
 *			| R - (erf(x)-x)/x | <= 2
 *
 *
 *	   Remark. The formula is derived by noting
 *          erf(x) = (2/sqrt(pi))*(x - x^3/3 + x^5/10 - x^7/42 + ....)
 *	   and that
 *          2/sqrt(pi) = 1.128379167095512573896158903121545171688
 *	   is close to one. The interval is chosen because the fix
 *	   point of erf(x) is near 0.6174 (i.e., erf(x)=x when x is
 *	   near 0.6174), and by some experiment, 0.84375 is chosen to
 * 	   guarantee the error is less than one ulp for erf.
 *
 *      2. For |x| in [0.84375,1.25], let s = |x| - 1, and
 *         c = 0.84506291151 rounded to single (24 bits)
 *         	erf(x)  = sign(x) * (c  + P1(s)/Q1(s))
 *         	erfc(x) = (1-c)  - P1(s)/Q1(s) if x > 0
 *			  1+(c+P1(s)/Q1(s))    if x < 0
 *         	|P1/Q1 - (erf(|x|)-c)| <= 2**-59.06
 *	   Remark: here we use the taylor series expansion at x=1.
 *		erf(1+s) = erf(1) + s*Poly(s)
 *			 = 0.845.. + P1(s)/Q1(s)
 *	   That is, we use rational approximation to approximate
 *			erf(1+s) - (c = (single)0.84506291151)
 *	   Note that |P1/Q1|< 0.078 for x in [0.84375,1.25]
 *	   where
 *		P1(s) = degree 6 poly in s
 *		Q1(s) = degree 6 poly in s
 *
 *      3. For x in [1.25,1/0.35(~2.857143)],
 *         	erfc(x) = (1/x)*exp(-x*x-0.5625+R1/S1)
 *         	erf(x)  = 1 - erfc(x)
 *	   where
 *		R1(z) = degree 7 poly in z, (z=1/x^2)
 *		S1(z) = degree 8 poly in z
 *
 *      4. For x in [1/0.35,28]
 *         	erfc(x) = (1/x)*exp(-x*x-0.5625+R2/S2) if x > 0
 *			= 2.0 - (1/x)*exp(-x*x-0.5625+R2/S2) if -6<x<0
 *			= 2.0 - tiny		(if x <= -6)
 *         	erf(x)  = sign(x)*(1.0 - erfc(x)) if x < 6, else
 *         	erf(x)  = sign(x)*(1.0 - tiny)
 *	   where
 *		R2(z) = degree 6 poly in z, (z=1/x^2)
 *		S2(z) = degree 7 poly in z
 *
 *      Note1:
 *	   To compute exp(-x*x-0.5625+R/S), let s be a single
 *	   precision number and s := x; then
 *		-x*x = -s*s + (s-x)*(s+x)
 *	        exp(-x*x-0.5626+R/S) =
 *			exp(-s*s-0.5625)*exp((s-x)*(s+x)+R/S);
 *      Note2:
 *	   Here 4 and 5 make use of the asymptotic series
 *			  exp(-x*x)
 *		erfc(x) ~ ---------- * ( 1 + Poly(1/x^2) )
 *			  x*sqrt(pi)
 *	   We use rational approximation to approximate
 *      	g(s)=f(1/x^2) = log(erfc(x)*x) - x*x + 0.5625
 *	   Here is the error bound for R1/S1 and R2/S2
 *      	|R1/S1 - f(x)|  < 2**(-62.57)
 *      	|R2/S2 - f(x)|  < 2**(-61.52)
 *
 *      5. For inf > x >= 28
 *         	erf(x)  = sign(x) *(1 - tiny)  (raise inexact)
 *         	erfc(x) = tiny*tiny (raise underflow) if x > 0
 *			= 2 - tiny if x<0
 *
 *      7. Special case:
 *         	erf(0)  = 0, erf(inf)  = 1, erf(-inf) = -1,
 *         	erfc(0) = 1, erfc(inf) = 0, erfc(-inf) = 2,
 *	   	erfc/erf(NaN) is NaN
 */


#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif


double __erfc(double x)
{
	int32_t hx, ix;
	double R, S, P, Q, s, y, z, r;

	static const double tiny = 1e-300;
	static const double half = 5.00000000000000000000e-01;	/* 0x3FE00000, 0x00000000 */
	static const double one = 1.00000000000000000000e+00;	/* 0x3FF00000, 0x00000000 */
	static const double two = 2.00000000000000000000e+00;	/* 0x40000000, 0x00000000 */
	/* c = (float)0.84506291151 */
	static const double erx = 8.45062911510467529297e-01;	/* 0x3FEB0AC1, 0x60000000 */

#include "t_erf.h"

	GET_HIGH_WORD(hx, x);
	ix = hx & IC(0x7fffffff);
	if (ix >= IC(0x7ff00000))
	{									/* erfc(nan)=nan */
		/* erfc(+-inf)=0,2 */
		return (double) (((uint32_t) hx >> 31) << 1) + one / x;
	}

	if (ix < IC(0x3feb0000))
	{									/* |x|<0.84375 */
		double r1, r2, s1, s2, s3, z2, z4;

		if (ix < IC(0x3c700000))			/* |x|<2**-56 */
			return one - x;
		z = x * x;
#ifdef DO_NOT_USE_THIS
		r = pp0 + z * (pp1 + z * (pp2 + z * (pp3 + z * pp4)));
		s = one + z * (qq1 + z * (qq2 + z * (qq3 + z * (qq4 + z * qq5))));
#else
		r1 = pp0 + z * pp1;
		z2 = z * z;
		r2 = pp2 + z * pp3;
		z4 = z2 * z2;
		s1 = one + z * qq1;
		s2 = qq2 + z * qq3;
		s3 = qq4 + z * qq5;
		r = r1 + z2 * r2 + z4 * pp4;
		s = s1 + z2 * s2 + z4 * s3;
#endif
		y = r / s;
		if (hx < IC(0x3fd00000))
		{								/* x<1/4 */
			return one - (x + x * y);
		} else
		{
			r = x * y;
			r += (x - half);
			return half - r;
		}
	}
	if (ix < IC(0x3ff40000))
	{									/* 0.84375 <= |x| < 1.25 */
		double s2, s4, s6, P1, P2, P3, P4, Q1, Q2, Q3, Q4;

		s = __ieee754_fabs(x) - one;
#ifdef DO_NOT_USE_THIS
		P = pa0 + s * (pa1 + s * (pa2 + s * (pa3 + s * (pa4 + s * (pa5 + s * pa6)))));
		Q = one + s * (qa1 + s * (qa2 + s * (qa3 + s * (qa4 + s * (qa5 + s * qa6)))));
#else
		P1 = pa0 + s * pa1;
		s2 = s * s;
		Q1 = one + s * qa1;
		s4 = s2 * s2;
		P2 = pa2 + s * pa3;
		s6 = s4 * s2;
		Q2 = qa2 + s * qa3;
		P3 = pa4 + s * pa5;
		Q3 = qa4 + s * qa5;
		P4 = pa6;
		Q4 = qa6;
		P = P1 + s2 * P2 + s4 * P3 + s6 * P4;
		Q = Q1 + s2 * Q2 + s4 * Q3 + s6 * Q4;
#endif
		if (hx >= 0)
		{
			z = one - erx;
			return z - P / Q;
		} else
		{
			z = erx + P / Q;
			return one + z;
		}
	}
	if (ix < IC(0x403c0000))
	{									/* |x|<28 */
		x = __ieee754_fabs(x);
		s = one / (x * x);
		if (ix < IC(0x4006DB6D))
		{								/* |x| < 1/.35 ~ 2.857143 */
#ifdef DO_NOT_USE_THIS
			R = ra0 + s * (ra1 + s * (ra2 + s * (ra3 + s * (ra4 + s * (ra5 + s * (ra6 + s * ra7))))));
			S = one + s * (sa1 + s * (sa2 + s * (sa3 + s * (sa4 + s * (sa5 + s * (sa6 + s * (sa7 + s * sa8)))))));
#else
			double R1, R2, R3, R4, S1, S2, S3, S4, s2, s4, s6, s8;

			R1 = ra0 + s * ra1;
			s2 = s * s;
			S1 = one + s * sa1;
			s4 = s2 * s2;
			R2 = ra2 + s * ra3;
			s6 = s4 * s2;
			S2 = sa2 + s * sa3;
			s8 = s4 * s4;
			R3 = ra4 + s * ra5;
			S3 = sa4 + s * sa5;
			R4 = ra6 + s * ra7;
			S4 = sa6 + s * sa7;
			R = R1 + s2 * R2 + s4 * R3 + s6 * R4;
			S = S1 + s2 * S2 + s4 * S3 + s6 * S4 + s8 * sa8;
#endif
		} else
		{								/* |x| >= 1/.35 ~ 2.857143 */
			double R1, R2, R3, S1, S2, S3, S4, s2, s4, s6;

			if (hx < 0 && ix >= IC(0x40180000))
				return two - tiny;		/* x < -6 */
#ifdef DO_NOT_USE_THIS
			R = rb0 + s * (rb1 + s * (rb2 + s * (rb3 + s * (rb4 + s * (rb5 + s * rb6)))));
			S = one + s * (sb1 + s * (sb2 + s * (sb3 + s * (sb4 + s * (sb5 + s * (sb6 + s * sb7))))));
#else
			R1 = rb0 + s * rb1;
			s2 = s * s;
			S1 = one + s * sb1;
			s4 = s2 * s2;
			R2 = rb2 + s * rb3;
			s6 = s4 * s2;
			S2 = sb2 + s * sb3;
			R3 = rb4 + s * rb5;
			S3 = sb4 + s * sb5;
			S4 = sb6 + s * sb7;
			R = R1 + s2 * R2 + s4 * R3 + s6 * rb6;
			S = S1 + s2 * S2 + s4 * S3 + s6 * S4;
#endif
		}
		z = x;
		SET_LOW_WORD(z, 0);
		r = __ieee754_exp(-z * z - 0.5625) * __ieee754_exp((z - x) * (z + x) + R / S);
		if (hx > 0)
			return r / x;
		else
			return two - r / x;
	}
	if (hx > 0)
		return tiny * tiny;
	return two - tiny;
}

__typeof(__erfc) erfc __attribute__((weak, alias("__erfc")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__erfcl) __erfcl __attribute__((alias("__erfc")));
__typeof(__erfcl) erfcl __attribute__((weak, alias("__erfc")));
#endif
