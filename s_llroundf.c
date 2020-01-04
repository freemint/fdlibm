/* Round double value to long int.
   Copyright (C) 1997-2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1997.

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

#ifndef __NO_LONGLONG

long long int __llroundf(float x)
{
	int32_t j0;
	uint32_t i;
	long long int result;
	int32_t sign;

	GET_FLOAT_WORD(i, x);
	j0 = ((i >> IEEE754_FLOAT_SHIFT) & IEEE754_FLOAT_MAXEXP) - IEEE754_FLOAT_BIAS;
	sign = (i & IC(0x80000000)) != 0 ? -1 : 1;
	i &= UC(0x7fffff);
	i |= UC(0x800000);

	if (j0 < (int32_t) (8 * sizeof(long long int)) - 1)
	{
		if (j0 < 0)
			return j0 < -1 ? 0 : sign;
		else if (j0 >= IEEE754_FLOAT_SHIFT)
			result = (long long int) i << (j0 - IEEE754_FLOAT_SHIFT);
		else
		{
			i += IC(0x400000) >> j0;

			result = i >> (IEEE754_FLOAT_SHIFT - j0);
		}
	} else
	{
		/* The number is too large.  It is left implementation defined
		   what happens.  */
		return (long long int) x;
	}

	if (sign < 0)
		result = -result;
	return result;
}

__typeof(__llroundf) llroundf __attribute__((weak, alias("__llroundf")));

#endif
