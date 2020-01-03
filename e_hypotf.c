/* e_hypotf.c -- float version of e_hypot.c.
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

#ifndef __have_fpu_hypot
float __ieee754_hypotf(float x, float y)
{
	float a, b,	t1,	t2,	y1,	y2,	w;
	int32_t j, k, ha, hb;

	GET_FLOAT_WORD(ha, x);
	ha &= IC(0x7fffffff);
	GET_FLOAT_WORD(hb, y);
	hb &= IC(0x7fffffff);
	if (hb > ha)
	{
		a = y;
		b = x;
		j = ha;
		ha = hb;
		hb = j;
	} else
	{
		a = x;
		b = y;
	}
	SET_FLOAT_WORD(a, ha);				/* a <- |a| */
	SET_FLOAT_WORD(b, hb);				/* b <- |b| */
	if ((ha - hb) > IC(0x0f000000))
	{
		return a + b;
	}									/* x/y > 2**30 */
	k = 0;
	if (ha > IC(0x58800000))
	{									/* a>2**50 */
		if (!FLT_UWORD_IS_FINITE(ha))
		{								/* Inf or NaN */
			w = a + b;					/* for sNaN */
			if (FLT_UWORD_IS_INFINITE(ha))
				w = a;
			if (FLT_UWORD_IS_INFINITE(hb))
				w = b;
			return w;
		}
		/* scale a and b by 2**-68 */
		ha -= IC(0x22000000);
		hb -= IC(0x22000000);
		k += 68;
		SET_FLOAT_WORD(a, ha);
		SET_FLOAT_WORD(b, hb);
	}
	if (hb < IC(0x26800000))
	{									/* b < 2**-50 */
		if (FLT_UWORD_IS_ZERO(hb))
		{
			return a;
		} else if (FLT_UWORD_IS_SUBNORMAL(hb))
		{
			SET_FLOAT_WORD(t1, IC(0x7e800000));	/* t1=2^126 */
			b *= t1;
			a *= t1;
			k -= 126;
		} else
		{								/* scale a and b by 2^68 */
			ha += IC(0x22000000);		/* a *= 2^68 */
			hb += IC(0x22000000);		/* b *= 2^68 */
			k -= 68;
			SET_FLOAT_WORD(a, ha);
			SET_FLOAT_WORD(b, hb);
		}
	}
	/* medium size a and b */
	w = a - b;
	if (w > b)
	{
		SET_FLOAT_WORD(t1, ha & IC(0xfffff000));
		t2 = a - t1;
		w = __ieee754_sqrtf(t1 * t1 - (b * (-b) - t2 * (a + t1)));
	} else
	{
		a = a + a;
		SET_FLOAT_WORD(y1, hb & IC(0xfffff000));
		y2 = b - y1;
		SET_FLOAT_WORD(t1, ha + IC(0x00800000));
		t2 = a - t1;
		w = __ieee754_sqrtf(t1 * y1 - (w * (-w) - (t1 * y2 + t2 * b)));
	}
	if (k != 0)
	{
		SET_FLOAT_WORD(t1, IC(0x3f800000) + (k << 23));
		return t1 * w;
	}
	return w;
}
#endif


float __hypotf(float x, float y)
{
	float z = __ieee754_hypotf(x, y);

	if (!isfinite(z) && isfinite(x) && isfinite(y) && _LIB_VERSION != _IEEE_)
		return __kernel_standard_f(x, y, z, KMATHERRF_HYPOT);	/* hypot overflow */

	return z;
}

__typeof(__hypotf) hypotf __attribute__((weak, alias("__hypotf")));
