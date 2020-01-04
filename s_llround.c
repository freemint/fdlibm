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

long long int __llround(double x)
{
	int32_t j0;
	uint32_t i1, i0;
	long long int result;
	int32_t sign;

	GET_DOUBLE_WORDS(i0, i1, x);
	j0 = ((i0 >> IEEE754_DOUBLE_SHIFT) & IEEE754_DOUBLE_MAXEXP) - IEEE754_DOUBLE_BIAS;
	sign = (i0 & IC(0x80000000)) != 0 ? -1 : 1;
	i0 &= IC(0xfffff);
	i0 |= IC(0x100000);

	if (j0 < IEEE754_DOUBLE_SHIFT)
	{
		if (j0 < 0)
			return j0 < -1 ? 0 : sign;
		else
		{
			i0 += IC(0x80000) >> j0;

			result = i0 >> (IEEE754_DOUBLE_SHIFT - j0);
		}
	} else if (j0 < (int32_t) (8 * sizeof(long long int)) - 1)
	{
		if (j0 >= 52)
		{
			if ((j0 - IEEE754_DOUBLE_SHIFT) >= (int32_t) (8 * sizeof(long long int)))
				result = 0;
			else
				result = (long long int) i0 << (j0 - IEEE754_DOUBLE_SHIFT);
			if ((j0 - 52) < 32)
				result |= (unsigned long long int)i1 << (j0 - 52);
		} else
		{
			uint32_t j = i1 + (UC(0x80000000) >> (j0 - IEEE754_DOUBLE_SHIFT));

			if (j < i1)
				++i0;

			if (j0 == IEEE754_DOUBLE_SHIFT)
				result = (long long int) i0;
			else
				result = ((long long int) i0 << (j0 - IEEE754_DOUBLE_SHIFT)) | (j >> (52 - j0));
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

__typeof(__llround) llround __attribute__((weak, alias("__llround")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__llroundl) __llroundl __attribute__((alias("__llround")));
__typeof(__llroundl) llroundl __attribute__((weak, alias("__llround")));
#endif

#endif
