/* Compute remainder and a congruent to the quotient.
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

#ifndef __have_fpu_remquo

double __ieee754_remquo(double x, double y, int *quo)
{
	int32_t hx, hy;
	uint32_t sx, lx, ly;
	int32_t qs;
	int cquo;
	
	static const double zero = 0.0;
	
	GET_DOUBLE_WORDS(hx, lx, x);
	GET_DOUBLE_WORDS(hy, ly, y);
	sx = hx & IC(0x80000000);
	qs = sx ^ (hy & IC(0x80000000));
	hy &= IC(0x7fffffff);
	hx &= IC(0x7fffffff);

	/* Purge off exception values.  */
	if ((hy | ly) == 0)
		return (x * y) / (x * y);		/* y = 0 */
	if ((hx >= IC(0x7ff00000))			/* x not finite */
		|| ((hy >= IC(0x7ff00000))		/* p is NaN */
			&& (((hy - IC(0x7ff00000)) | ly) != 0)))
		return (x * y) / (x * y);

	if (hy <= IC(0x7fbfffff))
		x = __ieee754_fmod(x, 8 * y);	/* now x < 8y */

	if (((hx - hy) | (lx - ly)) == 0)
	{
		*quo = qs ? -1 : 1;
		return zero * x;
	}

	x = __ieee754_fabs(x);
	y = __ieee754_fabs(y);
	cquo = 0;

	if (x >= 4 * y)
	{
		x -= 4 * y;
		cquo += 4;
	}
	if (x >= 2 * y)
	{
		x -= 2 * y;
		cquo += 2;
	}

	if (hy < IC(0x00200000))
	{
		if (x + x > y)
		{
			x -= y;
			++cquo;
			if (x + x >= y)
			{
				x -= y;
				++cquo;
			}
		}
	} else
	{
		double y_half = 0.5 * y;

		if (x > y_half)
		{
			x -= y;
			++cquo;
			if (x >= y_half)
			{
				x -= y;
				++cquo;
			}
		}
	}

	*quo = qs ? -cquo : cquo;

	if (sx)
		x = -x;
	return x;
}

#endif

double __remquo(double x, double y, int *quo)
{
	return __ieee754_remquo(x, y, quo);
}

__typeof(__remquo) remquo __attribute__((weak, alias("__remquo")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(remquol) __remquol __attribute__((alias("__remquo")));
__typeof(__remquol) remquol __attribute__((weak, alias("__remquo")));
#endif
