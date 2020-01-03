
/* @(#)w_exp.c 1.4 04/04/22 */
/*
 * ====================================================
 * Copyright (C) 2004 by Sun Microsystems, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/* 
 * wrapper exp(x)
 */

#include "fdlibm.h"

double exp(double x)		/* wrapper exp */
{
	double z = __ieee754_exp(x);

	if (_LIB_VERSION != _IEEE_ && (!isfinite(z) || z == 0.0) && isfinite(x))
		return __kernel_standard(x, x, z, signbit(x) ? KMATHERR_EXP_UNDERFLOW : KMATHERR_EXP_OVERFLOW);

	return z;
}
