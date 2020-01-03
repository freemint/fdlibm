
/* @(#)w_acos.c 1.3 95/01/18 */
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
 * wrap_acos(x)
 */

#include "fdlibm.h"


double acos(double x)		/* wrapper acos */
{
	if (_LIB_VERSION != _IEEE_ && isgreater(__ieee754_fabs(x), 1.0))
	{
		/* acos(|x|>1) */
		feraiseexcept(FE_INVALID);
		return __kernel_standard(x, x, __builtin_nan(""), KMATHERR_ACOS);
	}

	return __ieee754_acos(x);
}
