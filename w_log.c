
/* @(#)w_log.c 1.3 95/01/18 */
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
 * wrapper log(x)
 */

#include "fdlibm.h"


/* wrapper log(x) */
double log(double x)
{
	if (_LIB_VERSION != _IEEE_ && islessequal(x, 0.0))
	{
		if (x == 0.0)
		{
			feraiseexcept(FE_DIVBYZERO);
			return __kernel_standard(x, x, -HUGE_VAL, KMATHERR_LOG_ZERO);	/* log(0) */
		} else
		{
			feraiseexcept(FE_INVALID);
			return __kernel_standard(x, x, __builtin_nan(""), KMATHERR_LOG_MINUS);	/* log(x<0) */
		}
	}

	return __ieee754_log(x);
}
