/* Return minimum numeric value of X and Y.
   Copyright (C) 1997 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

double __fmin(double x, double y)
{
	return (islessequal (x, y) || isnan (y)) ? x : y;
}

__typeof(__fmin) fmin __attribute__((weak, alias("__fmin")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(fminl) __fminl __attribute__((alias("__fmin")));
__typeof(__fminl) fminl __attribute__((weak, alias("__fmin")));
#endif
