/* Extended-precision floating point argument reduction.
   Copyright (C) 1999-2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Based on quad-precision code by Jakub Jelinek <jj@ultra.linux.cz>

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

int32_t __ieee754_rem_pio2l(long double x, long double *y)
{
	double tx[3], ty[3];
	int32_t se, j0;
	uint32_t i0, i1;
	int32_t sx;
	int32_t n, exp;

	GET_LDOUBLE_WORDS(se, i0, i1, x);
	sx = (se >> 15) & 1;
	j0 = (se & IEEE854_LONG_DOUBLE_MAXEXP) - IEEE854_LONG_DOUBLE_BIAS;

	if (j0 < -1)
	{
		/* |x| < pi/4.  */
		y[0] = x;
		y[1] = 0;
		return 0;
	}

	if (j0 >= IC(0x8000))
	{
		/* x is infinite or NaN.  */
		y[0] = x - x;
		y[1] = y[0];
		return 0;
	}

	/* Split the 64 bits of the mantissa into three 24-bit integers
	   stored in a double array.  */
	exp = j0 - 23;
	tx[0] = (double) (i0 >> 8);
	tx[1] = (double) (((i0 << 16) | (i1 >> 16)) & IC(0xffffff));
	tx[2] = (double) ((i1 << 8) & IC(0xffffff));

	n = __kernel_rem_pio2l(tx, ty, exp, 3, 2);

	/* The result is now stored in two double values, we need to convert
	   it into two long double values.  */
	if (sx == 0)
	{
		y[0] = (long double) ty[0] + (long double) ty[1];
		y[1] = ty[1] - (y[0] - ty[0]);
		return n;
	} else
	{
		y[0] = -((long double) ty[0] + (long double) ty[1]);
		y[1] = -ty[1] - (y[0] + ty[0]);
		return -n;
	}
}

#endif
