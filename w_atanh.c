
/* @(#)w_atanh.c 1.3 95/01/18 */
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
 * wrapper atanh(x)
 */

#include "fdlibm.h"


double atanh(double x)		/* wrapper atanh */
{
	if (_LIB_VERSION != _IEEE_ && isgreaterequal(__ieee754_fabs(x), 1.0))
		return __kernel_standard(x, x, HUGE_VAL, __ieee754_fabs(x) > 1.0 ? KMATHERR_ATANH_PLUSONE	/* atanh(|x|>1) */
								 : KMATHERR_ATANH_ONE);	/* atanh(|x|==1) */

	return __ieee754_atanh(x);
}
