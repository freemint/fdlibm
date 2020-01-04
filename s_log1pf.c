/* s_log1pf.c -- float version of s_log1p.c.
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

#ifndef __have_fpu_log1p

float __ieee754_log1pf(float x)
{
	float hfsq, f, c, s, z, R, u;
	int32_t k, hx, hu, ax;

	static const float ln2_hi = 6.9313812256e-01;	/* 0x3f317180 */
	static const float ln2_lo = 9.0580006145e-06;	/* 0x3717f7d1 */
	static const float two25 = 3.355443200e+07;	/* 0x4c000000 */
	static const float Lp1 = 6.6666668653e-01;	/* 3F2AAAAB */
	static const float Lp2 = 4.0000000596e-01;	/* 3ECCCCCD */
	static const float Lp3 = 2.8571429849e-01;	/* 3E924925 */
	static const float Lp4 = 2.2222198546e-01;	/* 3E638E29 */
	static const float Lp5 = 1.8183572590e-01;	/* 3E3A3325 */
	static const float Lp6 = 1.5313838422e-01;	/* 3E1CD04F */
	static const float Lp7 = 1.4798198640e-01;	/* 3E178897 */
	
	static const float zero = 0.0;
	
	GET_FLOAT_WORD(hx, x);
	ax = hx & IC(0x7fffffff);

	k = 1;
	if (hx < IC(0x3ed413d7))
	{									/* x < 0.41422  */
		if (ax >= IC(0x3f800000))
		{								/* x <= -1.0 */
			if (x == 1.0F)
				return -two25 / (x - x);	/* log1p(-1)=+inf */
			else
				return (x - x) / (x - x);	/* log1p(x<-1)=NaN */
		}
		if (ax < IC(0x31000000))
		{								/* |x| < 2**-29 */
			math_force_eval(two25 + x);	/* raise inexact */
			if (ax < IC(0x24800000))	/* |x| < 2**-54 */
				return x;
			else
				return x - x * x * 0.5f;
		}
		if (hx > 0 || hx <= IC(0xbe95f61f))
		{
			k = 0;
			f = x;
			hu = 1;
			c = 0;
		}								/* -0.2929<x<0.41422 */
	}
	if (hx >= IC(0x7f800000))
		return x + x;
	if (k != 0)
	{
		if (hx < IC(0x5a000000))
		{
			u = 1.0f + x;
			GET_FLOAT_WORD(hu, u);
			k = (hu >> 23) - 127;
			/* correction term */
			c = (k > 0) ? 1.0f - (u - x) : x - (u - 1.0f);
			c /= u;
		} else
		{
			u = x;
			GET_FLOAT_WORD(hu, u);
			k = (hu >> 23) - 127;
			c = 0;
		}
		hu &= IC(0x007fffff);
		if (hu < IC(0x3504f7))
		{
			SET_FLOAT_WORD(u, hu | IC(0x3f800000));	/* normalize u */
		} else
		{
			k += 1;
			SET_FLOAT_WORD(u, hu | IC(0x3f000000));	/* normalize u/2 */
			hu = (IC(0x00800000) - hu) >> 2;
		}
		f = u - 1.0f;
	}
	hfsq = 0.5f * f * f;

	if (hu == 0)
	{									/* |f| < 2**-20 */
		if (f == zero)
		{
			if (k == 0)
				return zero;
			else
			{
				c += k * ln2_lo;
				return k * ln2_hi + c;
			}
		}
		R = hfsq * (1.0f - 0.66666666666666666f * f);
		if (k == 0)
			return f - R;
		else
			return k * ln2_hi - ((R - (k * ln2_lo + c)) - f);
	}
	s = f / (2.0f + f);
	z = s * s;
	R = z * (Lp1 + z * (Lp2 + z * (Lp3 + z * (Lp4 + z * (Lp5 + z * (Lp6 + z * Lp7))))));
	if (k == 0)
		return f - (hfsq - s * (hfsq + R));
	else
		return k * ln2_hi - ((hfsq - (s * (hfsq + R) + (k * ln2_lo + c))) - f);
}

#endif

/* wrapper log1p(x) */
float __log1pf(float x)
{
	if (islessequal(x, -1.0) && _LIB_VERSION != _IEEE_)
	{
		if (x == -1.0F)
		{
			feraiseexcept(FE_DIVBYZERO);
			return __kernel_standard_f(x, x, -HUGE_VALF, KMATHERRF_LOG_ZERO);	/* log(0) */
		} else
		{
			feraiseexcept(FE_INVALID);
			return __kernel_standard_f(x, x, __builtin_nanf(""), KMATHERRF_LOG_MINUS);	/* log(x<0) */
		}
	}

	return __ieee754_log1pf(x);
}

__typeof(__log1pf) log1pf __attribute__((weak, alias("__log1pf")));
