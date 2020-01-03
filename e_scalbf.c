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

float __ieee754_scalbf(float x, float fn)
{
	long ifn;
	
	if (isnan(x))
		return x * fn;
	if (!isfinite(fn))
	{
		if (isnan(fn) || fn > 0.0f)
			return x * fn;
		if (x == 0.0f)
			return x;
		return x / -fn;
	}
	ifn = (long) fn;
	if ((float) ifn != fn)
	{
		if (__ieee754_rintf(fn) != fn)
		{
			feraiseexcept(FE_INVALID);
			return __builtin_nanf("");
		}
		if (ifn > 65000l)
			return __ieee754_scalblnf(x, 65000l);
		return __ieee754_scalblnf(x, -65000l);
	}
	
	return __ieee754_scalblnf(x, ifn);
}

static float __attribute__ ((noinline)) sysv_scalbf(float x, float fn)
{
	float z = __ieee754_scalbf(x, fn);

	if (isinf(z))
	{
		if (isfinite(x))
			return __kernel_standard_f(x, fn, z, KMATHERR_SCALB_OVERFLOW);	/* scalb overflow */
		else
			__set_errno(ERANGE);
	} else if (z == 0.0F && z != x)
		return __kernel_standard_f(x, fn, z, KMATHERRF_SCALB_UNDERFLOW);	/* scalb underflow */

	return z;
}

/* Wrapper scalb */
float __scalbf(float x, float fn)
{
	if (_LIB_VERSION == _SVID_)
	{
		return sysv_scalbf(x, fn);
	} else
	{
		float z = __ieee754_scalbf(x, fn);

		if (!isfinite(z) || z == 0.0F)
		{
			if (isnan(z))
			{
				if (!isnan(x) && !isnan(fn))
					z = __kernel_standard_f(x, fn, z, KMATHERRF_SCALB_INVALID);
			} else if (isinf(z))
			{
				if (!isinf(x) && !isinf(fn))
					z = __kernel_standard_f(x, fn, z, KMATHERRF_SCALB_OVERFLOW);
			} else
			{
				/* z == 0.  */
				if (x != 0.0F && !isinf(fn))
					z = __kernel_standard_f(x, fn, z, KMATHERRF_SCALB_UNDERFLOW);
			}
		}
		return z;
	}
}


__typeof(__scalbf) scalbf __attribute__((weak, alias("__scalbf")));
