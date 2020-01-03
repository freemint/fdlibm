
/* @(#)w_sinh.c 1.3 95/01/18 */
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
 * wrapper sinh(x)
 */

#include "fdlibm.h"

double sinh(double x)		/* wrapper sinh */
{
	double z = __ieee754_sinh(x);

	if (_LIB_VERSION != _IEEE_ && !isfinite(z) && isfinite(x))
		return __kernel_standard(x, x, z, KMATHERR_SINH);	/* sinh overflow */

	return z;
}
