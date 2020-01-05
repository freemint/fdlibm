/* Round argument to nearest integral value according to current rounding
   direction.
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

long long int __llrint(double x)
{
	int32_t j0;
	uint32_t i0, i1;
	long long int result;
	int32_t sx;
	int rm;

	rm = fegetround();

	GET_DOUBLE_WORDS(i0, i1, x);
	j0 = ((i0 >> IEEE754_DOUBLE_SHIFT) & IEEE754_DOUBLE_MAXEXP) - IEEE754_DOUBLE_BIAS;
	sx = i0 & IC(0x80000000);
	i0 &= UC(0xfffff);
	
	if (j0 < IEEE754_DOUBLE_SHIFT)
	{
		uint32_t i;
		
		if (j0 == -IEEE754_DOUBLE_BIAS)
			return 0;
		/* Raise inexact if x != 0.  */
		if (j0 < 0 || (i0 | i1) != 0)
			feraiseexcept(FE_INEXACT);
		if (j0 < 0)
		{
			switch (rm)
			{
				case FE_TOWARDZERO: return 0;
				case FE_DOWNWARD: return sx ? -1 : 0;
				case FE_UPWARD: return sx ? 0 : 1;
			}
			/* case FE_TONEAREST: */
			if (j0 == -1 && (i0 | i1) != 0)
				return sx ? -1 : 1;
			return 0;
		}

		switch (rm)
		{
		case FE_TOWARDZERO:
			i0 |= UC(0x100000);
			break;
		case FE_DOWNWARD:
			i = UC(0x000fffff) >> j0;
			i0 |= UC(0x100000);
			if (sx && ((i0 & i) | i1) != 0)
				i0 += UC(0x00100000) >> j0;
			break;
		case FE_UPWARD:
			i = UC(0x000fffff) >> j0;
			i0 |= UC(0x100000);
			if (!sx && ((i0 & i) | i1) != 0)
				i0 += IC(0x00100000) >> j0;
			break;
		case FE_TONEAREST:
		default:
			i = UC(0x000fffff) >> j0;
			i0 |= UC(0x100000);
			if ((i0 & (i >> 1)) != 0 || i1 != 0 || (i0 & (UC(0x00100000) >> j0)))
			{
				i0 += UC(0x00080000) >> j0;
			}
			break;
		}
		result = i0 >> (IEEE754_DOUBLE_SHIFT - j0);
	} else if (j0 < (int32_t) (8 * sizeof(long long int)) - 1)
	{
		if (j0 >= 52)
		{
			i0 |= UC(0x100000);
			if ((j0 - IEEE754_DOUBLE_SHIFT) >= (int32_t) (8 * sizeof(long long int)))
				result = 0;
			else
				result = (long long int) i0 << (j0 - IEEE754_DOUBLE_SHIFT);
			if ((j0 - 52) < 32)
				result |= (unsigned long long int)i1 << (j0 - 52);
		} else
		{
			uint32_t i = UC(0xffffffff) >> (j0 - IEEE754_DOUBLE_SHIFT);
			uint32_t j;
		
			switch (rm)
			{
			case FE_TOWARDZERO:
				i0 |= UC(0x100000);
				break;
			case FE_DOWNWARD:
				i0 |= UC(0x100000);
				if (sx && (i1 & i) != 0)
				{
					if (j0 == IEEE754_DOUBLE_SHIFT)
						i0 += 1;
					else
					{
						j = i1 + (UC(1) << (52 - j0));
						if (j < i1)
							i0 += 1;			/* got a carry */
						i1 = j;
					}
				}
				break;
			case FE_UPWARD:
				i0 |= UC(0x100000);
				if (!sx && (i1 & i) != 0)
				{
					if (j0 == IEEE754_DOUBLE_SHIFT)
						i0 += 1;
					else
					{
						j = i1 + (UC(1) << (52 - j0));
						if (j < i1)
							i0 += 1;			/* got a carry */
						i1 = j;
					}
				}
				break;
			case FE_TONEAREST:
			default:
				i0 |= UC(0x100000);
				if ((i1 & (i >> 1)) != 0 || (j0 == IEEE754_DOUBLE_SHIFT && (i0 & 1)) || (i1 & (UC(1) << (52 - j0))))
				{
					j = i1 + (UC(1) << (51 - j0));
					if (j < i1)
						i0 += 1;
					i1 = j;
				}
				break;
			}
			i1 &= ~i;
			if (j0 == IEEE754_DOUBLE_SHIFT)
				result = (long long int) i0;
			else
				result = ((long long int) i0 << (j0 - IEEE754_DOUBLE_SHIFT)) | (i1 >> (52 - j0));
		}
	} else
	{
		/* The number is too large.  It is left implementation defined
		   what happens.  */
		feraiseexcept(FE_INVALID|FE_INEXACT);
		if (j0 == (IEEE754_DOUBLE_MAXEXP - IEEE754_DOUBLE_BIAS))
		{
			/* inf or nan */
			return LONG_LONG_MIN;
		}
		return (long long int) x;
	}

	return sx ? -result : result;
}

__typeof(__llrint) llrint __attribute__((weak, alias("__llrint")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(llrintl) __llrintl __attribute__((alias("__llrint")));
__typeof(__llrintl) llrintl __attribute__((weak, alias("__llrint")));
#endif

#endif
