/* @(#)e_j0.c 1.3 95/01/18 */
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
/* Modified by Naohiko Shimizu/Tokai University, Japan 1997/08/26,
   for performance improvement on pipelined processors.
*/

/* __ieee754_j0(x)
 * Bessel function of the first kind of order zero.
 * Method -- j0(x):
 *	1. For tiny x, we use j0(x) = 1 - x^2/4 + x^4/64 - ...
 *	2. Reduce x to |x| since j0(x)=j0(-x),  and
 *	   for x in (0,2)
 *		j0(x) = 1-z/4+ z^2*R0/S0,  where z = x*x;
 *	   (precision:  |j0-1+z/4-z^2R0/S0 |<2**-63.67 )
 *	   for x in (2,inf)
 *      j0(x) = sqrt(2/(pi*x))*(p0(x)*cos(x0)-q0(x)*sin(x0))
 *     where x0 = x-pi/4. It is better to compute sin(x0),cos(x0)
 *	   as follow:
 *		cos(x0) = cos(x)cos(pi/4)+sin(x)sin(pi/4)
 *			= 1/sqrt(2) * (cos(x) + sin(x))
 *		sin(x0) = sin(x)cos(pi/4)-cos(x)sin(pi/4)
 *			= 1/sqrt(2) * (sin(x) - cos(x))
 *     (To avoid cancellation, use
 *		sin(x) +- cos(x) = -cos(2x)/(sin(x) -+ cos(x))
 *      to compute the worse one.)
 *
 *	3 Special cases
 *		j0(nan)= nan
 *		j0(0) = 1
 *		j0(inf) = 0
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

double __ieee754_j0(double x)
{
	double z, s, c, ss, cc, r, u, v, r1, r2, s1, s2, z2, z4;
	int32_t hx, ix;

	static const double hugeval = 1e300;
	static const double one = 1.0;
	static const double zero = 0.0;
	static const double invsqrtpi = 5.64189583547756279280e-01;	/* 0x3FE20DD7, 0x50429B6D */

	/* R0/S0 on [0, 2.00] */
	static const double R02 = 1.56249999999999947958e-02;	/* 0x3F8FFFFF, 0xFFFFFFFD */
	static const double R03 = -1.89979294238854721751e-04;	/* 0xBF28E6A5, 0xB61AC6E9 */
	static const double R04 = 1.82954049532700665670e-06;	/* 0x3EBEB1D1, 0x0C503919 */
	static const double R05 = -4.61832688532103189199e-09;	/* 0xBE33D5E7, 0x73D63FCE */
	static const double S01 = 1.56191029464890010492e-02;	/* 0x3F8FFCE8, 0x82C8C2A4 */
	static const double S02 = 1.16926784663337450260e-04;	/* 0x3F1EA6D2, 0xDD57DBF4 */
	static const double S03 = 5.13546550207318111446e-07;	/* 0x3EA13B54, 0xCE84D5A9 */
	static const double S04 = 1.16614003333790000205e-09;	/* 0x3E1408BC, 0xF4745D8F */

	GET_HIGH_WORD(hx, x);
	ix = hx & IC(0x7fffffff);
	if (ix >= IC(0x7ff00000))
		return one / (x * x);
	x = __ieee754_fabs(x);
	if (ix >= IC(0x40000000))
	{									/* |x| >= 2.0 */
		__ieee754_sincos(x, &s, &c);
		ss = s - c;
		cc = s + c;
		if (ix < IC(0x7fe00000))
		{								/* make sure x+x not overflow */
			z = -__ieee754_cos(x + x);
			if ((s * c) < zero)
				cc = z / ss;
			else
				ss = z / cc;
		}
		/*
		 * j0(x) = 1/sqrt(pi) * (P(0,x)*cc - Q(0,x)*ss) / sqrt(x)
		 * y0(x) = 1/sqrt(pi) * (P(0,x)*ss + Q(0,x)*cc) / sqrt(x)
		 */
		if (ix > IC(0x48000000))
			z = (invsqrtpi * cc) / __ieee754_sqrt(x);
		else
		{
			u = __j0_y0_pzero(x);
			v = __j0_y0_qzero(x);
			z = invsqrtpi * (u * cc - v * ss) / __ieee754_sqrt(x);
		}
		return z;
	}
	if (ix < IC(0x3f200000))
	{									/* |x| < 2**-13 */
		math_force_eval(hugeval + x);	/* raise inexact if x != 0 */
		if (ix < IC(0x3e400000))
			return one;					/* |x|<2**-27 */
		else
			return one - 0.25 * x * x;
	}
	z = x * x;
#ifdef DO_NOT_USE_THIS
	r = z * (R02 + z * (R03 + z * (R04 + z * R05)));
	s = one + z * (S01 + z * (S02 + z * (S03 + z * S04)));
#else
	r1 = z * R02;
	z2 = z * z;
	r2 = R03 + z * R04;
	z4 = z2 * z2;
	r = r1 + z2 * r2 + z4 * R05;
	s1 = one + z * S01;
	s2 = S02 + z * S03;
	s = s1 + z2 * s2 + z4 * S04;
#endif
	if (ix < IC(0x3FF00000))
	{									/* |x| < 1.00 */
		return one + z * (-0.25 + (r / s));
	} else
	{
		u = 0.5 * x;
		return ((one + u) * (one - u) + z * (r / s));
	}
}


/* wrapper j0 */
double __j0(double x)
{
	if (isgreater(__ieee754_fabs(x), X_TLOSS) && _LIB_VERSION != _IEEE_ && _LIB_VERSION != _POSIX_)
		/* j0(|x|>X_TLOSS) */
		return __kernel_standard(x, x, 0.0, KMATHERR_J0_TLOSS);

	return __ieee754_j0(x);
}

__typeof(__j0) j0 __attribute__((weak, alias("__j0")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__j0l) __j0l __attribute__((alias("__j0")));
__typeof(__j0l) j0l __attribute__((weak, alias("__j0l")));
#endif
