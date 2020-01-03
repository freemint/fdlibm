
/* @(#)w_fmod.c 1.3 95/01/18 */
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
 * wrapper fmod(x,y)
 */

#include "fdlibm.h"


double fmod(double x, double y)	/* wrapper fmod */
{
	if (_LIB_VERSION != _IEEE_ && (isinf(x) || y == 0.0) && !isunordered(x, y))
		/* fmod(+-Inf,y) or fmod(x,0) */
		return __kernel_standard(x, y, y, KMATHERR_FMOD);
	return __ieee754_fmod(x, y);
}
