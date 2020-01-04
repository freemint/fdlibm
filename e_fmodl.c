/* e_fmodl.c -- long double version of e_fmod.c.
 * Conversion to IEEE quad long double by Jakub Jelinek, jj@ultra.linux.cz.
 */
/*
 * ====================================================
 * Copyright (C) 1993, 2011 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/*
 * __ieee754_fmodl(x,y)
 * Return x mod y in exact arithmetic
 * Method: shift and subtract
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_fmod

long double __ieee754_fmodl(long double x, long double y)
{
	long double intpart;

	if (isinf(y))
	{
		if (isinf(x))
			return x < 0 ? -0.0L : 0.0L;
		return x;
	}
	if (__ieee754_fabsl(x) <= __ieee754_fabsl(y))
		return x;
	(void) __ieee754_modfl(x / y, &intpart);
	return x - y * intpart;
}

#endif

long double __fmodl(long double x, long double y)
{
	if ((isinf(x) || y == 0.0) && _LIB_VERSION != _IEEE_ && !isunordered(x, y))
		/* fmod(+-Inf,y) or fmod(x,0) */
		return __kernel_standard_l(x, y, y, KMATHERRL_FMOD);
	return __ieee754_fmodl(x, y);
}

__typeof(__fmodl) fmodl __attribute__((weak, alias("__fmodl")));

#endif
