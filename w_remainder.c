
/* @(#)w_remainder.c 1.3 95/01/18 */
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
 * wrapper remainder(x,p)
 */

#include "fdlibm.h"

double remainder(double x, double y)	/* wrapper remainder */
{
	if (((y == 0.0 && !isnan(x))
		 || (isinf(x) && !isnan(y))) && _LIB_VERSION != _IEEE_)
		return __kernel_standard(x, y, y, KMATHERR_REMAINDER);	/* remainder domain */

	return __ieee754_remainder(x, y);
}
