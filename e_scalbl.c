/* Copyright (C) 2011-2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@gmail.com>, 2011.

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

long double __ieee754_scalbl(long double x, long double fn)
{
	long ifn;
	
	if (isnan(x))
		return x * fn;
	if (!isfinite(fn))
	{
		if (isnan(fn) || fn > 0.0L)
			return x * fn;
		if (x == 0.0L)
			return x;
		return x / -fn;
	}
	ifn = (long) fn;
	if ((long double) ifn != fn)
	{
		if (__ieee754_rintl(fn) != fn)
		{
			feraiseexcept(FE_INVALID);
			return __builtin_nanl("");
		}
		if (ifn > 65000l)
			return __ieee754_scalblnl(x, 65000l);
		return __ieee754_scalblnl(x, -65000l);
	}

	return __ieee754_scalblnl(x, ifn);
}

static long double __attribute__ ((noinline)) sysv_scalbl(long double x, long double fn)
{
	long double z = __ieee754_scalbl(x, fn);

	if (isinf(z))
	{
		if (isfinite(x))
			return __kernel_standard_l(x, fn, z, KMATHERRL_SCALB_OVERFLOW);	/* scalb overflow */
		else
			__set_errno(ERANGE);
	} else if (z == 0.0 && z != x)
		return __kernel_standard_l(x, fn, z, KMATHERRL_SCALB_UNDERFLOW);	/* scalb underflow */

	return z;
}

/* Wrapper scalb */
long double __scalbl(long double x, long double fn)
{
	if (_LIB_VERSION == _SVID_)
	{
		return sysv_scalbl(x, fn);
	} else
	{
		long double z = __ieee754_scalbl(x, fn);

		if (!isfinite(z) || z == 0.0)
		{
			if (isnan(z))
			{
				if (!isnan(x) && !isnan(fn))
					z = __kernel_standard_l(x, fn, z, KMATHERRL_SCALB_INVALID);
			} else if (isinf(z))
			{
				if (!isinf(x) && !isinf(fn))
					z = __kernel_standard_l(x, fn, z, KMATHERRL_SCALB_OVERFLOW);
			} else
			{
				/* z == 0.  */
				if (x != 0.0 && !isinf(fn))
					z = __kernel_standard_l(x, fn, z, KMATHERRL_SCALB_UNDERFLOW);
			}
		}
		return z;
	}
}


__typeof(__scalbl) scalbl __attribute__((weak, alias("__scalbl")));

#endif
