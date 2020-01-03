/* Copyright (C) 2002, 2007 by  Red Hat, Incorporated. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * is freely granted, provided that this notice is preserved.
 */

#include "fdlibm.h"

#undef fpclassify

int __fpclassify (double x)
{
	int retval = FP_NORMAL;
	uint32_t msw, lsw;

	GET_DOUBLE_WORDS(msw, lsw, x);

	lsw |= msw & UC(0xfffff);
	msw &= UC(0x7ff00000);
	if ((msw | lsw) == 0)
		retval = FP_ZERO;
	else if (msw == 0)
		retval = FP_SUBNORMAL;
	else if (msw == UC(0x7ff00000))
		retval = lsw != 0 ? FP_NAN : FP_INFINITE;

	return retval;
}

__typeof(__fpclassify) fpclassify __attribute__((weak, alias("__fpclassify")));
#ifdef __NO_LONG_DOUBLE_MATH
int __fpclassifyl(long double x) __attribute__((alias("__fpclassify")));
__typeof(__fpclassifyl) fpclassifyl __attribute__((weak, alias("__fpclassifyl")));
#endif
