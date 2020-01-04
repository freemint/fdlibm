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

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_lround

long int __ieee754_lroundl(long double x)
{
	int32_t j0;
	uint32_t se, i1, i0;
	long int result;
	int32_t sign;

	GET_LDOUBLE_WORDS(se, i0, i1, x);
	j0 = (se & IEEE854_LONG_DOUBLE_MAXEXP) - IEEE854_LONG_DOUBLE_BIAS;
	sign = (se & 0x8000) != 0 ? -1 : 1;

	if (j0 < 31)
	{
		if (j0 < 0)
			return j0 < -1 ? 0 : sign;
		else
		{
			uint32_t j = i0 + (UC(0x40000000) >> j0);

			if (j < i0)
			{
				j >>= 1;
				j |= UC(0x80000000);
				++j0;
			}

			result = j >> (31 - j0);
		}
	} else if (j0 < (int32_t) (8 * sizeof(long int)) - 1)
	{
		if (j0 >= 63)
		{
			result = (long int) i0 << (j0 - 31);
			if ((j0 - 63) < 32)
				result |= (unsigned long int)i1 << (j0 - 63);
		} else
		{
			uint32_t j = i1 + (UC(0x80000000) >> (j0 - 31));

			if (j < i1)
				++i0;

			if (j0 == 31)
				result = (long int) i0;
			else
				result = ((long int) i0 << (j0 - 31)) | (j >> (63 - j0));
		}
	} else
	{
		/* The number is too large.  It is left implementation defined
		   what happens.  */
		return (long int) x;
	}

	if (sign < 0)
		result = -result;
	return result;
}

#endif

long int __lroundl(long double x)
{
	return __ieee754_lroundl(x);
}

__typeof(__lroundl) lroundl __attribute__((weak, alias("__lroundl")));

#endif
