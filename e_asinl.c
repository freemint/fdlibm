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
  Long double expansions are
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

/* __ieee754_asin(x)
 * Method :
 *	Since  asin(x) = x + x^3/6 + x^5*3/40 + x^7*15/336 + ...
 *	we approximate asin(x) on [0,0.5] by
 *		asin(x) = x + x*x^2*R(x^2)
 *
 *	For x in [0.5,1]
 *		asin(x) = pi/2-2*asin(sqrt((1-x)/2))
 *	Let y = (1-x), z = y/2, s := sqrt(z), and pio2_hi+pio2_lo=pi/2;
 *	then for x>0.98
 *		asin(x) = pi/2 - 2*(s+s*z*R(z))
 *			= pio2_hi - (2*(s+s*z*R(z)) - pio2_lo)
 *	For x<=0.98, let pio4_hi = pio2_hi/2, then
 *		f = hi part of s;
 *		c = sqrt(z) - f = (z-f*f)/(s+f)		...f+c=sqrt(z)
 *	and
 *		asin(x) = pi/2 - 2*(s+s*z*R(z))
 *			= pio4_hi+(pio4-2s)-(2s*z*R(z)-pio2_lo)
 *			= pio4_hi+(pio4-2f)-(2s*z*R(z)-(pio2_lo+2c))
 *
 * Special cases:
 *	if x is NaN, return x itself;
 *	if |x|>1, return NaN with invalid signal.
 *
 */


#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_asin

long double __ieee754_asinl(long double x)
{
	long double t, w, p, q, c, r, s;
	int32_t ix;
	uint32_t se, i0, i1, k;

	static const long double one = 1.0L;
	static const long double hugeval = 1.0e+4932L;
	static const long double pio2_hi = 0x1.921fb54442d1846ap+0L;	/* pi/2 rounded to nearest to 64 bits.  */
	static const long double pio2_lo = -0x7.6733ae8fe47c65d8p-68L;	/* pi/2 - pio2_hi rounded to nearest to 64 bits.  */
	static const long double pio4_hi = 0xc.90fdaa22168c235p-4L;		/* pi/4 rounded to nearest to 64 bits.  */
	/* coefficient for R(x^2) */
	/* asin(x) = x + x^3 pS(x^2) / qS(x^2)
	   0 <= x <= 0.5
	   peak relative error 1.9e-21  */
	static const long double pS0 = -1.008714657938491626019651170502036851607E1L;
	static const long double pS1 = 2.331460313214179572063441834101394865259E1L;
	static const long double pS2 = -1.863169762159016144159202387315381830227E1L;
	static const long double pS3 = 5.930399351579141771077475766877674661747E0L;
	static const long double pS4 = -6.121291917696920296944056882932695185001E-1L;
	static const long double pS5 = 3.776934006243367487161248678019350338383E-3L;
	static const long double qS0 = -6.052287947630949712886794360635592886517E1L;
	static const long double qS1 = 1.671229145571899593737596543114258558503E2L;
	static const long double qS2 = -1.707840117062586426144397688315411324388E2L;
	static const long double qS3 = 7.870295154902110425886636075950077640623E1L;
	static const long double qS4 = -1.568433562487314651121702982333303458814E1L;
	/* 1.000000000000000000000000000000000000000E0 */
	
	GET_LDOUBLE_WORDS(se, i0, i1, x);
	ix = se & 0x7fff;
	ix = (ix << 16) | (i0 >> 16);
	if (ix >= IC(0x3fff8000))
	{									/* |x|>= 1 */
		if (ix == IC(0x3fff8000) && ((i0 - UC(0x80000000)) | i1) == 0)
			/* asin(1)=+-pi/2 with inexact */
			return x * pio2_hi + x * pio2_lo;
		return (x - x) / (x - x);		/* asin(|x|>1) is NaN */
	} else if (ix < IC(0x3ffe8000))
	{									/* |x|<0.5 */
		if (ix < IC(0x3fde8000))
		{								/* if |x| < 2**-33 */
			if (hugeval + x > one)
				return x;				/* return x with inexact if x!=0 */
		} else
		{
			t = x * x;
			p = t * (pS0 + t * (pS1 + t * (pS2 + t * (pS3 + t * (pS4 + t * pS5)))));
			q = qS0 + t * (qS1 + t * (qS2 + t * (qS3 + t * (qS4 + t))));
			w = p / q;
			return x + x * w;
		}
	}
	/* 1> |x|>= 0.5 */
	w = one - __ieee754_fabsl(x);
	t = w * 0.5;
	p = t * (pS0 + t * (pS1 + t * (pS2 + t * (pS3 + t * (pS4 + t * pS5)))));
	q = qS0 + t * (qS1 + t * (qS2 + t * (qS3 + t * (qS4 + t))));
	s = __ieee754_sqrtl(t);
	if (ix >= IC(0x3ffef999))
	{									/* if |x| > 0.975 */
		w = p / q;
		t = pio2_hi - (2.0 * (s + s * w) - pio2_lo);
	} else
	{
		GET_LDOUBLE_WORDS(k, i0, i1, s);
		i1 = 0;
		SET_LDOUBLE_WORDS(w, k, i0, i1);
		c = (t - w * w) / (s + w);
		r = p / q;
		p = 2.0 * s * r - (pio2_lo - 2.0 * c);
		q = pio4_hi - 2.0 * w;
		t = pio4_hi - (p - q);
	}
	if ((se & 0x8000) == 0)
		return t;
	return -t;
}

#endif

/* wrapper asin */
long double __asinl(long double  x)
{
	if (isgreater(__ieee754_fabsl(x), 1.0) && _LIB_VERSION != _IEEE_)
	{
		/* asin(|x|>1) */
		feraiseexcept(FE_INVALID);
		return __kernel_standard_l(x, x, __builtin_nanl(""), KMATHERRL_ASIN);
	}

	return __ieee754_asinl(x);
}

__typeof(__asinl) asinl __attribute__((weak, alias("__asinl")));

#endif
