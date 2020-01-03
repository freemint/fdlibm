
/* @(#)w_cosh.c 1.3 95/01/18 */
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
 * wrapper cosh(x)
 */

#include "fdlibm.h"

double cosh(double x)		/* wrapper cosh */
{
	double z = __ieee754_cosh(x);

	if (_LIB_VERSION != _IEEE_ && !isfinite(z) && isfinite(x))
		return __kernel_standard(x, x, z, KMATHERR_COSH);	/* cosh overflow */

	return z;
}
