
/* @(#)w_acosh.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 *
 */

/* 
 * wrapper acosh(x)
 */

#include "fdlibm.h"

/* wrapper acosh */
double acosh(double x)
{
	if (_LIB_VERSION != _IEEE_ && isless(x, 1.0))
		/* acosh(x<1) */
		return __kernel_standard(x, x, __builtin_nan(""), KMATHERR_ACOSH);

	return __ieee754_acosh(x);
}
