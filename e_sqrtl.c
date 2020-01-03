/* Emulation for sqrtl.
   Contributed by Paolo Bonzini

   Copyright 2002-2003, 2007, 2009-2013 Free Software Foundation, Inc.

   This file was taken from gnulib.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */


#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_sqrt

/* A simple Newton-Raphson method. */
long double __ieee754_sqrtl(long double x)
{
	long double delta, y;
	int exponent;

	/* Check for NaN */
	if (isnanl(x))
		return x;

	/* Check for negative numbers */
	if (x < 0.0L)
		return (long double) sqrt(-1);

	/* Check for zero and infinites */
	if (x + x == x)
		return x;

	__ieee754_frexpl(x, &exponent);
	y = __ieee754_ldexpl(x, -exponent / 2);

	do
	{
		delta = y;
		y = (y + x / y) * 0.5L;
		delta -= y;
	} while (delta != 0.0L);

	return y;
}

#endif

long double __sqrtl(long double x)
{
	if (isless(x, 0.0) && _LIB_VERSION != _IEEE_)
		return __kernel_standard_l(x, x, __builtin_nanl(""), KMATHERRL_SQRT); /* sqrt(negative) */
	return __ieee754_sqrtl(x);
}

__typeof(__sqrtl) sqrtl __attribute__((weak, alias("__sqrtl")));

#endif
