
/* @(#)w_hypot.c 1.3 95/01/18 */
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
 * wrapper hypot(x,y)
 */

#include "fdlibm.h"


double hypot(double x, double y)/* wrapper hypot */
{
	double z = __ieee754_hypot(x, y);

	if (!isfinite(z) && isfinite(x) && isfinite(y) && _LIB_VERSION != _IEEE_)
		return __kernel_standard(x, y, z, KMATHERR_HYPOT);	/* hypot overflow */

	return z;
}
