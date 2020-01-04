/* Compute 2^x.
   Copyright (C) 2012-2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

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

#ifndef __have_fpu_exp2

long double __ieee754_exp2l(long double x)
{
	if (isless(x, (long double) LDBL_MAX_EXP))
	{
		if (isgreaterequal(x, (long double) (LDBL_MIN_EXP - LDBL_MANT_DIG - 1)))
		{
			int intx = (int) x;
			long double fractx = x - intx;

			return __ieee754_scalbnl(__ieee754_expm1l(M_LN2l * fractx) + 1.0L, intx);
		}
		/* Underflow or exact zero.  */
		if (isinf(x))
			return 0;
		feraiseexcept(FE_UNDERFLOW);
		return 0;
	}
	/* Infinity, NaN or overflow.  */
	return LDBL_MAX * x;
}

#endif

long double __exp2l(long double x)
{
	long double z = __ieee754_exp2l(x);

	if (!isfinite(z) && isfinite(x) && _LIB_VERSION != _IEEE_)
		/* exp2 overflow: 244, exp2 underflow: 245 */
		return __kernel_standard_l(x, x, z, signbit(x) ? KMATHERRL_EXP2_UNDERFLOW : KMATHERRL_EXP2_OVERFLOW);

	return z;
}


__typeof(__exp2l) exp2l __attribute__((weak, alias("__exp2l")));
__typeof(__exp2l) __pow2l __attribute__((alias("__exp2l")));
__typeof(__pow2l) pow2l __attribute__((weak, alias("__pow2l")));

#endif
