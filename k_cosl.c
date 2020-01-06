/* Extended-precision floating point cosine on <-pi/4,pi/4>.
   Copyright (C) 1999-2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Based on quad-precision cosine by Jakub Jelinek <jj@ultra.linux.cz>

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

long double __kernel_cosl(long double x, long double y)
{
	long double h, l, z, sin_l, cos_l_m1;
	int index;

	/* The polynomials have not been optimized for extended-precision and
	   may contain more terms than needed.  */
	
	static const long double ONE = 1.00000000000000000000000000000000000E+00L;
	
	/* cos x ~ ONE + x^2 ( SCOS1 + SCOS2 * x^2 + ... + SCOS4 * x^6 + SCOS5 * x^8 )
	   x in <0,1/256>  */
	static const long double SCOS1 = -5.00000000000000000000000000000000000E-01L;
	static const long double SCOS2 = 4.16666666666666666666666666556146073E-02L;
	static const long double SCOS3 = -1.38888888888888888888309442601939728E-03L;
	static const long double SCOS4 = 2.48015873015862382987049502531095061E-05L;
	static const long double SCOS5 = -2.75573112601362126593516899592158083E-07L;
	
	/* cos x ~ ONE + x^2 ( COS1 + COS2 * x^2 + ... + COS7 * x^12 + COS8 * x^14 )
	   x in <0,0.1484375>  */
	static const long double COS1 = -4.99999999999999999999999999999999759E-01L;
	static const long double COS2 = 4.16666666666666666666666666651287795E-02L;
	static const long double COS3 = -1.38888888888888888888888742314300284E-03L;
	static const long double COS4 = 2.48015873015873015867694002851118210E-05L;
	static const long double COS5 = -2.75573192239858811636614709689300351E-07L;
	static const long double COS6 = 2.08767569877762248667431926878073669E-09L;
	static const long double COS7 = -1.14707451049343817400420280514614892E-11L;
	static const long double COS8 = 4.77810092804389587579843296923533297E-14L;
	
	/* sin x ~ ONE * x + x^3 ( SSIN1 + SSIN2 * x^2 + ... + SSIN4 * x^6 + SSIN5 * x^8 )
	   x in <0,1/256>  */
	static const long double SSIN1 = -1.66666666666666666666666666666666659E-01L;
	static const long double SSIN2 = 8.33333333333333333333333333146298442E-03L;
	static const long double SSIN3 = -1.98412698412698412697726277416810661E-04L;
	static const long double SSIN4 = 2.75573192239848624174178393552189149E-06L;
	static const long double SSIN5 = -2.50521016467996193495359189395805639E-08L;
	
#define SINCOSL_COS_HI 0
#define SINCOSL_COS_LO 1
#define SINCOSL_SIN_HI 2
#define SINCOSL_SIN_LO 3
	
	if (signbit(x))
	{
		x = -x;
		y = -y;
	}
	if (x < 0.1484375L)
	{
		/* Argument is small enough to approximate it by a Chebyshev
		   polynomial of degree 16.  */
		if (x < 0x1p-33L)
			if (!((int32_t) x))
				return ONE;				/* generate inexact */
		z = x * x;
		return ONE + (z * (COS1 + z * (COS2 + z * (COS3 + z * (COS4 + z * (COS5 + z * (COS6 + z * (COS7 + z * COS8))))))));
	} else
	{
		/* So that we don't have to use too large polynomial,  we find
		   l and h such that x = l + h,  where fabsl(l) <= 1.0/256 with 83
		   possible values for h.  We look up cosl(h) and sinl(h) in
		   pre-computed tables,  compute cosl(l) and sinl(l) using a
		   Chebyshev polynomial of degree 10(11) and compute
		   cosl(h+l) = cosl(h)cosl(l) - sinl(h)sinl(l).  */
		index = (int) (128 * (x - (0.1484375L - 1.0L / 256.0L)));
		h = 0.1484375L + index / 128.0;
		index *= 4;
		l = y - (h - x);
		z = l * l;
		sin_l = l * (ONE + z * (SSIN1 + z * (SSIN2 + z * (SSIN3 + z * (SSIN4 + z * SSIN5)))));
		cos_l_m1 = z * (SCOS1 + z * (SCOS2 + z * (SCOS3 + z * (SCOS4 + z * SCOS5))));
		return __sincosl_table[index + SINCOSL_COS_HI]
			+ (__sincosl_table[index + SINCOSL_COS_LO]
			   - (__sincosl_table[index + SINCOSL_SIN_HI] * sin_l
				  - __sincosl_table[index + SINCOSL_COS_HI] * cos_l_m1));
	}

#undef SINCOSL_COS_HI
#undef SINCOSL_COS_LO
#undef SINCOSL_SIN_HI
#undef SINCOSL_SIN_LO
}
