/* Compute cubic root of float value.
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

float __cbrtf(float x)
{
	float xm, ym, u, t2;
	int xe;
	int x_class;
	
	static const float factor[5] = {
		0.62996052494743658238361,			/* 1 / 2^(2/3) */
		0.79370052598409973737585,			/* 1 / 2^(1/3) */
		1.0,
		1.2599210498948731647672,			/* 2^(1/3) */
		1.5874010519681994747517			/* 2^(2/3) */
	};
	
	/* Reduce X.  XM now is an range 1.0 to 0.5.  */
	xm = __ieee754_frexpf(__ieee754_fabsf(x), &xe);

	/* If X is not finite or is null return it (with raising exceptions
	   if necessary.
	   Note: *Our* version of `frexp' sets XE to zero if the argument is
	   Inf or NaN.  This is not portable but faster.  */
	if (xe == 0 && ((x_class = fpclassify(x)) == FP_ZERO || x_class == FP_NAN || x_class == FP_INFINITE))
		return x + x;

	u = (0.492659620528969547F + (0.697570460207922770F - 0.191502161678719066F * xm) * xm);

	t2 = u * u * u;

	ym = u * (t2 + 2.0F * xm) / (2.0F * t2 + xm) * factor[2 + xe % 3];

	return __ieee754_ldexpf(x > 0.0F ? ym : -ym, xe / 3);
}

__typeof(__cbrtf) cbrtf __attribute__((weak, alias("__cbrtf")));
