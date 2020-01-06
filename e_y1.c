/* @(#)e_y1.c 5.1 93/09/24 */
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
/* Modified by Naohiko Shimizu/Tokai University, Japan 1997/08/26,
   for performance improvement on pipelined processors.
*/

/* __ieee754_y1(x)
 * Bessel function of the second kind of order one.
 * Method -- y1(x):
 *	1. screen out x<=0 cases: y1(0)=-inf, y1(x<0)=NaN
 *	2. For x<2.
 *	   Since
 *		y1(x) = 2/pi*(j1(x)*(ln(x/2)+Euler)-1/x-x/2+5/64*x^3-...)
 *	   therefore y1(x)-2/pi*j1(x)*ln(x)-1/x is an odd function.
 *	   We use the following function to approximate y1,
 *		y1(x) = x*U(z)/V(z) + (2/pi)*(j1(x)*ln(x)-1/x), z= x^2
 *	   where for x in [0,2] (abs err less than 2**-65.89)
 *		U(z) = U0[0] + U0[1]*z + ... + U0[4]*z^4
 *		V(z) = 1  + v0[0]*z + ... + v0[4]*z^5
 *	   Note: For tiny x, 1/x dominate y1 and hence
 *		y1(tiny) = -2/pi/tiny, (choose tiny<2**-54)
 *	3. For x>=2.
 *		y1(x) = sqrt(2/(pi*x))*(p1(x)*sin(x1)+q1(x)*cos(x1))
 *	   where x1 = x-3*pi/4. It is better to compute sin(x1),cos(x1)
 *	   by method mentioned above.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

double __ieee754_y1(double x)
{
	double z, s, c, ss, cc, u, v, u1, u2, v1, v2, v3, z2, z4;
	int32_t hx, ix, lx;

	static const double one = 1.0;
	static const double invsqrtpi = 5.64189583547756279280e-01;	/* 0x3FE20DD7, 0x50429B6D */
	static const double tpi = 6.36619772367581382433e-01;	/* 0x3FE45F30, 0x6DC9C883 */
	static const double zero = 0.0;

	static const double U0[5] = {
		-1.96057090646238940668e-01,		/* 0xBFC91866, 0x143CBC8A */
		5.04438716639811282616e-02,			/* 0x3FA9D3C7, 0x76292CD1 */
		-1.91256895875763547298e-03,		/* 0xBF5F55E5, 0x4844F50F */
		2.35252600561610495928e-05,			/* 0x3EF8AB03, 0x8FA6B88E */
		-9.19099158039878874504e-08			/* 0xBE78AC00, 0x569105B8 */
	};

	static const double V0[5] = {
		1.99167318236649903973e-02,			/* 0x3F94650D, 0x3F4DA9F0 */
		2.02552581025135171496e-04,			/* 0x3F2A8C89, 0x6C257764 */
		1.35608801097516229404e-06,			/* 0x3EB6C05A, 0x894E8CA6 */
		6.22741452364621501295e-09,			/* 0x3E3ABF1D, 0x5BA69A86 */
		1.66559246207992079114e-11			/* 0x3DB25039, 0xDACA772A */
	};

	GET_DOUBLE_WORDS(hx, lx, x);
	ix = IC(0x7fffffff) & hx;
	/* if Y1(NaN) is NaN, Y1(-inf) is NaN, Y1(inf) is 0 */
	if (ix >= IC(0x7ff00000))
		return one / (x + x * x);
	if ((ix | lx) == 0)
		return -HUGE_VAL + x; /* -inf and overflow exception.  */ ;
	if (hx < 0)
		return zero / zero;
	if (ix >= IC(0x40000000))
	{									/* |x| >= 2.0 */
		__ieee754_sincos(x, &s, &c);
		ss = -s - c;
		cc = s - c;
		if (ix < IC(0x7fe00000))
		{								/* make sure x+x not overflow */
			z = __ieee754_cos(x + x);
			if ((s * c) > zero)
				cc = z / ss;
			else
				ss = z / cc;
		}
		/* y1(x) = sqrt(2/(pi*x))*(p1(x)*sin(x0)+q1(x)*cos(x0))
		 * where x0 = x-3pi/4
		 *      Better formula:
		 *              cos(x0) = cos(x)cos(3pi/4)+sin(x)sin(3pi/4)
		 *                      =  1/sqrt(2) * (sin(x) - cos(x))
		 *              sin(x0) = sin(x)cos(3pi/4)-cos(x)sin(3pi/4)
		 *                      = -1/sqrt(2) * (cos(x) + sin(x))
		 * To avoid cancellation, use
		 *              sin(x) +- cos(x) = -cos(2x)/(sin(x) -+ cos(x))
		 * to compute the worse one.
		 */
		if (ix > IC(0x48000000))
			z = (invsqrtpi * ss) / __ieee754_sqrt(x);
		else
		{
			u = __j1_y1_pone(x);
			v = __j1_y1_qone(x);
			z = invsqrtpi * (u * ss + v * cc) / __ieee754_sqrt(x);
		}
		return z;
	}
	if (ix <= IC(0x3c900000))
	{									/* x < 2**-54 */
		return (-tpi / x);
	}
	z = x * x;
#ifdef DO_NOT_USE_THIS
	u = U0[0] + z * (U0[1] + z * (U0[2] + z * (U0[3] + z * U0[4])));
	v = one + z * (V0[0] + z * (V0[1] + z * (V0[2] + z * (V0[3] + z * V0[4]))));
#else
	u1 = U0[0] + z * U0[1];
	z2 = z * z;
	u2 = U0[2] + z * U0[3];
	z4 = z2 * z2;
	u = u1 + z2 * u2 + z4 * U0[4];
	v1 = one + z * V0[0];
	v2 = V0[1] + z * V0[2];
	v3 = V0[3] + z * V0[4];
	v = v1 + z2 * v2 + z4 * v3;
#endif
	return (x * (u / v) + tpi * (__ieee754_j1(x) * __ieee754_log(x) - one / x));
}

/* wrapper y1 */
double __y1(double x)
{
	if ((islessequal(x, 0.0) || isgreater(x, X_TLOSS)) && _LIB_VERSION != _IEEE_)
	{
		if (x < 0.0)
		{
			/* d = zero/(x-x) */
			feraiseexcept(FE_INVALID);
			return __kernel_standard(x, x, -HUGE_VAL, KMATHERR_Y1_MINUS);
		} else if (x == 0.0)
			/* d = -one/(x-x) */
			return __kernel_standard(x, x, -HUGE_VAL, KMATHERR_Y1_ZERO);
		else if (_LIB_VERSION != _POSIX_)
			/* y1(x>X_TLOSS) */
			return __kernel_standard(x, x, 0.0, KMATHERR_Y1_TLOSS);
	}

	return __ieee754_y1(x);
}

__typeof(__y1) y1 __attribute__((weak, alias("__y1")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__y1l) __y1l __attribute__((alias("__y1")));
__typeof(__y1l) y1l __attribute__((weak, alias("__y1l")));
#endif
