/* Compute cubic root of double value.
   Copyright (C) 1997-2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Dirk Alboth <dirka@uni-paderborn.de> and
   Ulrich Drepper <drepper@cygnus.com>, 1997.

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

#ifndef __NO_LONG_DOUBLE_MATH

long double __cbrtl(long double x)
{
	long double xm, u;
	int xe;
	int x_class;
	
	static const long double factor[5] = {
		0.62996052494743658238361L,			/* 1 / 2^(2/3) */
		0.79370052598409973737585L,			/* 1 / 2^(1/3) */
		1.0L,
		1.2599210498948731647672L,			/* 2^(1/3) */
		1.5874010519681994747517L			/* 2^(2/3) */
	};
	
	static const long double third = 0.3333333333333333333333333L;

	/* Reduce X.  XM now is an range 1.0 to 0.5.  */
	xm = __ieee754_frexpl(__ieee754_fabsl(x), &xe);

	/* If X is not finite or is null return it (with raising exceptions
	   if necessary.
	   Note: *Our* version of `frexp' sets XE to zero if the argument is
	   Inf or NaN.  This is not portable but faster.  */
	if (xe == 0 && ((x_class = fpclassify(x)) == FP_ZERO || x_class == FP_NAN || x_class == FP_INFINITE))
		return x + x;

	u = (((-1.34661104733595206551E-1 * xm
		   + 5.46646013663955245034E-1) * xm
		  - 9.54382247715094465250E-1) * xm + 1.13999833547172932737E0) * xm + 4.02389795645447521269E-1;

	u *= factor[2 + xe % 3];
	u = __ieee754_ldexpl(x > 0.0 ? u : -u, xe / 3);

	u -= (u - (x / (u * u))) * third;
	u -= (u - (x / (u * u))) * third;
	return u;
}

__typeof(__cbrtl) cbrtl __attribute__((weak, alias("__cbrtl")));

#endif
