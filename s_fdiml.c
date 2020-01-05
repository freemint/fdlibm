/* Return positive difference between arguments.
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
long double __fdiml(long double x, long double y)
{
	int clsx = fpclassify(x);
	int clsy = fpclassify(y);
	long double r;

	if (clsx == FP_NAN || clsy == FP_NAN || (y < 0 && clsx == FP_INFINITE && clsy == FP_INFINITE))
		/* Raise invalid flag.  */
		return x - y;

	if (x <= y)
		return 0.0L;

	r = x - y;

	if (isinf(r))
		__set_errno(ERANGE);

	return r;
}

__typeof(__fdiml) fdiml __attribute__((weak, alias("__fdiml")));

#endif
