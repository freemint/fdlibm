/* Copyright (C) 2002, 2007 by  Red Hat, Incorporated. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * is freely granted, provided that this notice is preserved.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

int __fpclassifyf(float x)
{
	int retval = FP_NORMAL;
	uint32_t wx;

	GET_FLOAT_WORD(wx, x);
	wx &= UC(0x7fffffff);
	if (wx == 0)
		retval = FP_ZERO;
	else if (wx < UC(0x800000))
		retval = FP_SUBNORMAL;
	else if (wx >= UC(0x7f800000))
		retval = wx > UC(0x7f800000) ? FP_NAN : FP_INFINITE;

	return retval;
}

__typeof(__fpclassifyf) fpclassifyf __attribute__((weak, alias("__fpclassifyf")));
