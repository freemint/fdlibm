
/* @(#)w_log10.c 1.3 95/01/18 */
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
 * wrapper log10(X)
 */

#include "fdlibm.h"


/* wrapper log10(x) */
double log10(double x)
{
	if (_LIB_VERSION != _IEEE_ && islessequal(x, 0.0))
	{
		if (x == 0.0)
		{
			feraiseexcept(FE_DIVBYZERO);
			return __kernel_standard(x, x, -HUGE_VAL, KMATHERR_LOG10_ZERO);	/* log10(0) */
		} else
		{
			feraiseexcept(FE_INVALID);
			return __kernel_standard(x, x, __builtin_nan(""), KMATHERR_LOG10_MINUS);	/* log10(x<0) */
		}
	}

	return __ieee754_log10(x);
}
