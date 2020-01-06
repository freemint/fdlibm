/* k_cosf.c -- float version of k_cos.c
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

float __kernel_cosf(float x, float y)
{
	float a, hz, z, r, qx;
	int32_t ix;

	static const float one = 1.0000000000e+00;		/* 0x3f800000 */
	static const float C1 = 4.1666667908e-02;		/* 0x3d2aaaab */
	static const float C2 = -1.3888889225e-03;		/* 0xbab60b61 */
	static const float C3 = 2.4801587642e-05;		/* 0x37d00d01 */
	static const float C4 = -2.7557314297e-07;		/* 0xb493f27c */
	static const float C5 = 2.0875723372e-09;		/* 0x310f74f6 */
	static const float C6 = -1.1359647598e-11;		/* 0xad47d74e */

	GET_FLOAT_WORD(ix, x);
	ix &= IC(0x7fffffff);				/* ix = |x|'s high word */
	if (ix < IC(0x32000000))
	{									/* if x < 2**27 */
		if (((int32_t) x) == 0)
			return one;					/* generate inexact */
	}
	z = x * x;
	r = z * (C1 + z * (C2 + z * (C3 + z * (C4 + z * (C5 + z * C6)))));
	if (ix < IC(0x3e99999a))			/* if |x| < 0.3 */
		return one - (0.5f * z - (z * r - x * y));
	else
	{
		if (ix > IC(0x3f480000))
		{								/* x > 0.78125 */
			qx = 0.28125f;
		} else
		{
			SET_FLOAT_WORD(qx, ix - IC(0x01000000));	/* x/4 */
		}
		hz = 0.5f * z - qx;

		a = one - qx;
		return a - (hz - (z * r - x * y));
	}
}
