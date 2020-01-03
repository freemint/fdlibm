/* Copyright (C) 2002, 2007 by  Red Hat, Incorporated. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * is freely granted, provided that this notice is preserved.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

int __fpclassifyl(long double x)
{
	int retval = FP_NORMAL;
	uint32_t ex, hx, lx, m;

	GET_LDOUBLE_WORDS(ex, hx, lx, x);
	m = (hx & UC(0x7fffffff)) | lx;
	ex &= IEEE854_LONG_DOUBLE_MAXEXP;
	if ((ex | m) == 0)
		retval = FP_ZERO;
	else if (ex == 0 && (hx & UC(0x80000000)) == 0)
		retval = FP_SUBNORMAL;
	else if (ex == IEEE854_LONG_DOUBLE_MAXEXP)
		retval = m != 0 ? FP_NAN : FP_INFINITE;

	return retval;
}

__typeof(__fpclassifyl) fpclassifyl __attribute__((weak, alias("__fpclassifyl")));

#endif
