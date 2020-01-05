/* Base 2 logarithm.
   Copyright (C) 2011-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2.1 of the License, or
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

#ifndef __have_fpu_log2

long double __ieee754_log2l(long double x)
{
	/* Decompose x into
	   x = 2^e * y
	   where
	   e is an integer,
	   1/2 < y < 2.
	   Then log2(x) = e + log2(y) = e + log(y)/log(2).  */
	int e;
	long double y;

	y = __ieee754_frexpl(x, &e);
	if (y < M_SQRT1_2l)
	{
		y = 2.0L * y;
		e = e - 1;
	}

	return (long double) e + __ieee754_logl(y) * M_LOG2El;
}

#endif

/* wrapper log2(x) */
long double __log2l(long double x)
{
	if (islessequal(x, 0.0) && _LIB_VERSION != _IEEE_)
	{
		if (x == 0.0)
		{
			feraiseexcept(FE_DIVBYZERO);
			return __kernel_standard_l(x, x, -HUGE_VALL, KMATHERRL_LOG2_ZERO);	/* log2(0) */
		} else
		{
			feraiseexcept(FE_INVALID);
			return __kernel_standard_l(x, x, __builtin_nanl(""), KMATHERRL_LOG2_MINUS);	/* log2(x<0) */
		}
	}

	return __ieee754_log2l(x);
}

__typeof(__log2l) log2l __attribute__((weak, alias("__log2l")));

#endif
