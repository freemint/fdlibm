
/* @(#)w_j0.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/*
 * wrapper j0(double x), y0(double x)
 */

#include "fdlibm.h"

double j0(double x)		/* wrapper j0 */
{
	if (isgreater(__ieee754_fabs(x), X_TLOSS) && _LIB_VERSION != _IEEE_ && _LIB_VERSION != _POSIX_)
		/* j0(|x|>X_TLOSS) */
		return __kernel_standard(x, x, 0.0, KMATHERR_J0_TLOSS);

	return __ieee754_j0(x);
}

double y0(double x)		/* wrapper y0 */
{
	if ((islessequal(x, 0.0) || isgreater(x, X_TLOSS)) && _LIB_VERSION != _IEEE_)
	{
		if (x < 0.0)
		{
			/* d = zero/(x-x) */
			feraiseexcept(FE_INVALID);
			return __kernel_standard(x, x, -HUGE_VAL, KMATHERR_Y0_MINUS);
		} else if (x == 0.0)
			/* d = -one/(x-x) */
			return __kernel_standard(x, x, -HUGE_VAL, KMATHERR_Y0_ZERO);
		else if (_LIB_VERSION != _POSIX_)
			/* y0(x>X_TLOSS) */
			return __kernel_standard(x, x, 0.0, KMATHERR_Y0_TLOSS);
	}

	return __ieee754_y0(x);
}
