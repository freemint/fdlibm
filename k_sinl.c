/* Quad-precision floating point sine on <-pi/4,pi/4>.
   Copyright (C) 1999-2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Based on quad-precision sine by Jakub Jelinek <jj@ultra.linux.cz>

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

/* The polynomials have not been optimized for extended-precision and
   may contain more terms than needed.  */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

long double __kernel_sinl(long double x, long double y, int iy)
{
	long double absx, h, l, z, sin_l, cos_l_m1;
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
	
	/* sin x ~ ONE * x + x^3 ( SIN1 + SIN2 * x^2 + ... + SIN7 * x^12 + SIN8 * x^14 )
	   x in <0,0.1484375>  */
	static const long double SIN1 = -1.66666666666666666666666666666666538e-01L;
	static const long double SIN2 = 8.33333333333333333333333333307532934e-03L;
	static const long double SIN3 = -1.98412698412698412698412534478712057e-04L;
	static const long double SIN4 = 2.75573192239858906520896496653095890e-06L;
	static const long double SIN5 = -2.50521083854417116999224301266655662e-08L;
	static const long double SIN6 = 1.60590438367608957516841576404938118e-10L;
	static const long double SIN7 = -7.64716343504264506714019494041582610e-13L;
	static const long double SIN8 = 2.81068754939739570236322404393398135e-15L;
	
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

	absx = __ieee754_fabsl(x);
	if (absx < 0.1484375L)
	{
		/* Argument is small enough to approximate it by a Chebyshev
		   polynomial of degree 17.  */
		if (absx < 0x1p-33L)
			if (!((int32_t) x))
				return x;				/* generate inexact */
		z = x * x;
		return x + (x * (z * (SIN1 + z * (SIN2 + z * (SIN3 + z * (SIN4 +
																  z * (SIN5 + z * (SIN6 + z * (SIN7 + z * SIN8)))))))));
	} else
	{
		/* So that we don't have to use too large polynomial,  we find
		   l and h such that x = l + h,  where fabsl(l) <= 1.0/256 with 83
		   possible values for h.  We look up cosl(h) and sinl(h) in
		   pre-computed tables,  compute cosl(l) and sinl(l) using a
		   Chebyshev polynomial of degree 10(11) and compute
		   sinl(h+l) = sinl(h)cosl(l) + cosl(h)sinl(l).  */
		index = (int) (128 * (absx - (0.1484375L - 1.0L / 256.0L)));
		h = 0.1484375L + index / 128.0;
		index *= 4;
		if (iy)
			l = (x < 0 ? -y : y) - (h - absx);
		else
			l = absx - h;
		z = l * l;
		sin_l = l * (ONE + z * (SSIN1 + z * (SSIN2 + z * (SSIN3 + z * (SSIN4 + z * SSIN5)))));
		cos_l_m1 = z * (SCOS1 + z * (SCOS2 + z * (SCOS3 + z * (SCOS4 + z * SCOS5))));
		z = __sincosl_table[index + SINCOSL_SIN_HI]
			+ (__sincosl_table[index + SINCOSL_SIN_LO]
			   + (__sincosl_table[index + SINCOSL_SIN_HI] * cos_l_m1)
			   + (__sincosl_table[index + SINCOSL_COS_HI] * sin_l));
		return (x < 0) ? -z : z;
	}

#undef SINCOSL_COS_HI
#undef SINCOSL_COS_LO
#undef SINCOSL_SIN_HI
#undef SINCOSL_SIN_LO
}
