/* e_asinf.c -- float version of e_asin.c.
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

/*
  Modifications for single precision expansion are
  Copyright (C) 2001 Stephen L. Moshier <moshier@na-net.ornl.gov>
  and are incorporated herein by permission of the author.  The author
  reserves the right to distribute this material elsewhere under different
  copying permissions.  These modifications are distributed here under
  the following terms:

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, see
    <http://www.gnu.org/licenses/>.  */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_asin

float __ieee754_asinf(float x)
{
	float t, w, p, q, c, r, s;
	int32_t hx, ix;

	static const float one = 1.0000000000e+00;	/* 0x3F800000 */
	static const float hugeval = 1.000e+30;
	static const float pio2_hi = 1.57079637050628662109375f;
	static const float pio2_lo = -4.37113900018624283e-8f;
	static const float pio4_hi = 0.785398185253143310546875f;
	/* asin x = x + x^3 p(x^2)
	   -0.5 <= x <= 0.5;
	   Peak relative error 4.8e-9 */
	static const float p0 = 1.666675248e-1f;
	static const float p1 = 7.495297643e-2f;
	static const float p2 = 4.547037598e-2f;
	static const float p3 = 2.417951451e-2f;
	static const float p4 = 4.216630880e-2f;
	
	GET_FLOAT_WORD(hx, x);
	ix = hx & IC(0x7fffffff);
	if (ix == IC(0x3f800000))
	{
		/* asin(1)=+-pi/2 with inexact */
		return x * pio2_hi + x * pio2_lo;
	} else if (ix > IC(0x3f800000))
	{									/* |x|>= 1 */
		return (x - x) / (x - x);		/* asin(|x|>1) is NaN */
	} else if (ix < IC(0x3f000000))
	{									/* |x|<0.5 */
		if (ix < IC(0x32000000))
		{								/* if |x| < 2**-27 */
			if (hugeval + x > one)
				return x;				/* return x with inexact if x!=0 */
		} else
		{
			t = x * x;
			w = t * (p0 + t * (p1 + t * (p2 + t * (p3 + t * p4))));
			return x + x * w;
		}
	}
	/* 1> |x|>= 0.5 */
	w = one - __ieee754_fabsf(x);
	t = w * 0.5f;
	p = t * (p0 + t * (p1 + t * (p2 + t * (p3 + t * p4))));
	s = __ieee754_sqrtf(t);
	if (ix >= IC(0x3F79999A))
	{									/* if |x| > 0.975 */
		t = pio2_hi - (2.0f * (s + s * p) - pio2_lo);
	} else
	{
		int32_t iw;

		w = s;
		GET_FLOAT_WORD(iw, w);
		SET_FLOAT_WORD(w, iw & IC(0xfffff000));
		c = (t - w * w) / (s + w);
		r = p;
		p = 2.0f * s * r - (pio2_lo - 2.0f * c);
		q = pio4_hi - 2.0f * w;
		t = pio4_hi - (p - q);
	}
	if (hx > 0)
		return t;
	return -t;
}

#endif

/* wrapper asin */
float __asinf(float x)
{
	if (isgreater(__ieee754_fabsf(x), 1.0) && _LIB_VERSION != _IEEE_)
	{
		/* asin(|x|>1) */
		feraiseexcept(FE_INVALID);
		return __kernel_standard_f(x, x, __builtin_nanf(""), KMATHERRF_ASIN);
	}

	return __ieee754_asinf(x);
}

__typeof(__asinf) asinf __attribute__((weak, alias("__asinf")));
