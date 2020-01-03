/*
 * ====================================================
 * Copyright (C) 2004 by Sun Microsystems, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/* __ieee754_pow(x,y) return x**y
 *
 *		      n
 * Method:  Let x =  2   * (1+f)
 *	1. Compute and return log2(x) in two pieces:
 *		log2(x) = w1 + w2,
 *	   where w1 has 53-24 = 29 bit trailing zeros.
 *	2. Perform y*log2(x) = n+y' by simulating multi-precision 
 *	   arithmetic, where |y'|<=0.5.
 *	3. Return x**y = 2**n*exp(y'*log2)
 *
 * Special cases:
 *	1.  (anything) ** 0  is 1
 *	2.  (anything) ** 1  is itself
 *	3a. (anything) ** NAN is NAN except
 *	3b. +1         ** NAN is 1
 *	4.  NAN ** (anything except 0) is NAN
 *	5.  +-(|x| > 1) **  +INF is +INF
 *	6.  +-(|x| > 1) **  -INF is +0
 *	7.  +-(|x| < 1) **  +INF is +0
 *	8.  +-(|x| < 1) **  -INF is +INF
 *	9.  +-1         ** +-INF is 1
 *	10. +0 ** (+anything except 0, NAN)               is +0
 *	11. -0 ** (+anything except 0, NAN, odd integer)  is +0
 *	12. +0 ** (-anything except 0, NAN)               is +INF
 *	13. -0 ** (-anything except 0, NAN, odd integer)  is +INF
 *	14. -0 ** (odd integer) = -( +0 ** (odd integer) )
 *	15. +INF ** (+anything except 0,NAN) is +INF
 *	16. +INF ** (-anything except 0,NAN) is +0
 *	17. -INF ** (anything)  = -0 ** (-anything)
 *	18. (-anything) ** (integer) is (-1)**(integer)*(+anything**integer)
 *	19. (-anything except 0 and inf) ** (non-integer) is NAN
 *
 * Accuracy:
 *	pow(x,y) returns x**y nearly rounded. In particular
 *			pow(integer,integer)
 *	always returns the correct integer provided it is 
 *	representable.
 *
 * Constants :
 * The hexadecimal values are the intended ones for the following 
 * constants. The decimal values may be used, provided that the 
 * compiler will convert from decimal to binary accurately enough 
 * to produce the hexadecimal values shown.
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_pow


double __ieee754_pow(double x, double y)
{
	double z, ax, z_h, z_l, p_h, p_l;
	double y1, t1, t2, r, s, t, u, v, w;
	int32_t i, j, k, yisint, n;
	int32_t hx, hy, ix, iy;
	uint32_t lx, ly;

	static const double bp[] = { 1.0, 1.5 };
	static const double dp_h[] = { 0.0, 5.84962487220764160156e-01 };		/* 0x3FE2B803, 0x40000000 */
	static const double dp_l[] = { 0.0, 1.35003920212974897128e-08 };		/* 0x3E4CFDEB, 0x43CFD006 */
	static const double zero = 0.0;
	static const double one = 1.0;
	static const double two = 2.0;
	static const double two53 = 9007199254740992.0;	/* 0x43400000, 0x00000000 */
	/* poly coefs for (3/2)*(log(x)-2s-2/3*s**3 */
	static const double L1 = 5.99999999999994648725e-01;	/* 0x3FE33333, 0x33333303 */
	static const double L2 = 4.28571428578550184252e-01;	/* 0x3FDB6DB6, 0xDB6FABFF */
	static const double L3 = 3.33333329818377432918e-01;	/* 0x3FD55555, 0x518F264D */
	static const double L4 = 2.72728123808534006489e-01;	/* 0x3FD17460, 0xA91D4101 */
	static const double L5 = 2.30660745775561754067e-01;	/* 0x3FCD864A, 0x93C9DB65 */
	static const double L6 = 2.06975017800338417784e-01;	/* 0x3FCA7E28, 0x4A454EEF */
	static const double P1 = 1.66666666666666019037e-01;	/* 0x3FC55555, 0x5555553E */
	static const double P2 = -2.77777777770155933842e-03;	/* 0xBF66C16C, 0x16BEBD93 */
	static const double P3 = 6.61375632143793436117e-05;	/* 0x3F11566A, 0xAF25DE2C */
	static const double P4 = -1.65339022054652515390e-06;	/* 0xBEBBBD41, 0xC5D26BF1 */
	static const double P5 = 4.13813679705723846039e-08;	/* 0x3E663769, 0x72BEA4D0 */
	static const double lg2 = 6.93147180559945286227e-01;	/* 0x3FE62E42, 0xFEFA39EF */
	static const double lg2_h = 6.93147182464599609375e-01;	/* 0x3FE62E43, 0x00000000 */
	static const double lg2_l = -1.90465429995776804525e-09;	/* 0xBE205C61, 0x0CA86C39 */
	static const double ovt = 8.0085662595372944372e-0017;	/* -(1024-log2(ovfl+.5ulp)) */
	static const double cp = 9.61796693925975554329e-01;	/* 0x3FEEC709, 0xDC3A03FD =2/(3ln2) */
	static const double cp_h = 9.61796700954437255859e-01;	/* 0x3FEEC709, 0xE0000000 =(float)cp */
	static const double cp_l = -7.02846165095275826516e-09;	/* 0xBE3E2FE0, 0x145B01F5 =tail of cp_h */
	static const double ivln2 = 1.44269504088896338700e+00;	/* 0x3FF71547, 0x652B82FE =1/ln2 */
	static const double ivln2_h = 1.44269502162933349609e+00;	/* 0x3FF71547, 0x60000000 =24b 1/ln2 */
	static const double ivln2_l = 1.92596299112661746887e-08;	/* 0x3E54AE0B, 0xF85DDF44 =1/ln2 tail */

	GET_DOUBLE_WORDS(hx, lx, x);
	GET_DOUBLE_WORDS(hy, ly, y);
	ix = hx & IC(0x7fffffff);
	iy = hy & IC(0x7fffffff);

	/* y==zero: x**0 = 1 */
	if ((iy | ly) == 0)
	{
		/* unless x is signaling NaN */
		if (issignaling(x))
			return __builtin_nan("");
		return one;
	}

	/* x|y==NaN return NaN unless x==1 then return 1 */
	if (ix > IC(0x7ff00000) || ((ix == IC(0x7ff00000)) && (lx != 0)) || iy > IC(0x7ff00000) || ((iy == IC(0x7ff00000)) && (ly != 0)))
	{
		if (((ix - IC(0x3ff00000)) | lx) == 0 && !(hx & UC(0x80000000)))
			return one;
		else
			return __builtin_nan("");
	}

	/* determine if y is an odd int when x < 0
	 * yisint = 0   ... y is not an integer
	 * yisint = 1   ... y is an odd int
	 * yisint = 2   ... y is an even int
	 */
	yisint = 0;
	if (hx < 0)
	{
		if (iy >= IC(0x43400000))
			yisint = 2;					/* even integer y */
		else if (iy >= IC(0x3ff00000))
		{
			k = (iy >> IEEE754_DOUBLE_SHIFT) - IEEE754_DOUBLE_BIAS;		/* exponent */
			if (k > IEEE754_DOUBLE_SHIFT)
			{
				j = ly >> (52 - k);
				if ((j << (52 - k)) == (int32_t)ly)
					yisint = 2 - (j & 1);
			} else if (ly == 0)
			{
				j = iy >> (IEEE754_DOUBLE_SHIFT - k);
				if ((j << (IEEE754_DOUBLE_SHIFT - k)) == iy)
					yisint = 2 - (j & 1);
			}
		}
	}

	/* special value of y */
	if (ly == 0)
	{
		if (iy == IC(0x7ff00000))
		{								/* y is +-inf */
			if (((ix - IC(0x3ff00000)) | lx) == 0)
				return one;				/* +-1**+-inf = 1 */
			else if (ix >= IC(0x3ff00000))	/* (|x|>1)**+-inf = inf,0 */
				return (hy >= 0) ? y : zero;
			else						/* (|x|<1)**-,+inf = inf,0 */
				return (hy < 0) ? -y : zero;
		}
		if (iy == IC(0x3ff00000))
		{								/* y is  +-1 */
			if (hy < 0)
				return one / x;
			else
				return x;
		}
		if (hy == IC(0x40000000))
			return x * x;				/* y is  2 */
		if (hy == IC(0x3fe00000))
		{								/* y is  0.5 */
			if (hx >= 0)				/* x >= +0 */
				return __ieee754_sqrt(x);
		}
	}

	ax = __ieee754_fabs(x);
	/* special value of x */
	if (lx == 0)
	{
		if (ix == IC(0x7ff00000) || ix == 0 || ix == IC(0x3ff00000))
		{
			z = ax;						/*x is +-0,+-inf,+-1 */
			if (hy < 0)
				z = one / z;			/* z = (1/|x|) */
			if (hx < 0)
			{
				if (((ix - IC(0x3ff00000)) | yisint) == 0)
				{
					z = (z - z) / (z - z);	/* (-1)**non-int is NaN */
				} else if (yisint == 1)
					z = -z;				/* (x<0)**odd = -(|x|**odd) */
			}
			return z;
		}
	}

	/* (x<0)**(non-int) is NaN */
	if (((((uint32_t) hx >> 31) - 1) | yisint) == 0)
		return (x - x) / (x - x);

	/* |y| is huge */
	if (iy > IC(0x41e00000))
	{									/* if |y| > 2**31 */
		if (iy > IC(0x43f00000))
		{								/* if |y| > 2**64, must o/uflow */
			if (ix <= IC(0x3fefffff))
			{
				if (hy < 0)
				{
					feraiseexcept(FE_OVERFLOW);
					return HUGE_VAL;
				}
				feraiseexcept(FE_UNDERFLOW);
				return 0;
			}
			if (ix >= IC(0x3ff00000))
			{
				if (hy > 0)
				{
					feraiseexcept(FE_OVERFLOW);
					return HUGE_VAL;
				}
				feraiseexcept(FE_UNDERFLOW);
				return 0;
			}
		}
		/* over/underflow if x is not close to one */
		if (ix < IC(0x3fefffff))
		{
			if (hy < 0)
			{
				feraiseexcept(FE_OVERFLOW);
				return HUGE_VAL;
			}
			feraiseexcept(FE_UNDERFLOW);
			return 0;
		}
		if (ix > IC(0x3ff00000))
		{
			if (hy > 0)
			{
				feraiseexcept(FE_OVERFLOW);
				return HUGE_VAL;
			}
			feraiseexcept(FE_UNDERFLOW);
			return 0;
		}
		/* now |1-x| is tiny <= 2**-20, suffice to compute 
		   log(x) by x-x^2/2+x^3/3-x^4/4 */
		t = ax - 1;						/* t has 20 trailing zeros */
		w = (t * t) * (0.5 - t * (0.3333333333333333333333 - t * 0.25));
		u = ivln2_h * t;				/* ivln2_h has 21 sig. bits */
		v = t * ivln2_l - w * ivln2;
		t1 = u + v;
		SET_LOW_WORD(t1, 0);
		t2 = v - (t1 - u);
	} else
	{
		double s2, s_h, s_l, t_h, t_l;

		n = 0;
		/* take care subnormal number */
		if (ix < IC(0x00100000))
		{
			ax *= two53;
			n -= 53;
			GET_HIGH_WORD(ix, ax);
		}
		n += ((ix) >> IEEE754_DOUBLE_SHIFT) - IEEE754_DOUBLE_BIAS;
		j = ix & IC(0x000fffff);
		/* determine interval */
		ix = j | IC(0x3ff00000);			/* normalize ix */
		if (j <= IC(0x3988E))
			k = 0;						/* |x|<sqrt(3/2) */
		else if (j < IC(0xBB67A))
			k = 1;						/* |x|<sqrt(3)   */
		else
		{
			k = 0;
			n += 1;
			ix -= IC(0x00100000);
		}
		SET_HIGH_WORD(ax, ix);

		/* compute s = s_h+s_l = (x-1)/(x+1) or (x-1.5)/(x+1.5) */
		u = ax - bp[k];					/* bp[0]=1.0, bp[1]=1.5 */
		v = one / (ax + bp[k]);
		s = u * v;
		s_h = s;
		SET_LOW_WORD(s_h, 0);
		/* t_h=ax+bp[k] High */
		t_h = zero;
		SET_HIGH_WORD(t_h, ((ix >> 1) | IC(0x20000000)) + IC(0x00080000) + (k << 18));
		t_l = ax - (t_h - bp[k]);
		s_l = v * ((u - s_h * t_h) - s_h * t_l);
		/* compute log(ax) */
		s2 = s * s;
		r = s2 * s2 * (L1 + s2 * (L2 + s2 * (L3 + s2 * (L4 + s2 * (L5 + s2 * L6)))));
		r += s_l * (s_h + s);
		s2 = s_h * s_h;
		t_h = 3.0 + s2 + r;
		SET_LOW_WORD(t_h, 0);
		t_l = r - ((t_h - 3.0) - s2);
		/* u+v = s*(1+...) */
		u = s_h * t_h;
		v = s_l * t_h + t_l * s;
		/* 2/(3log2)*(s+...) */
		p_h = u + v;
		SET_LOW_WORD(p_h, 0);
		p_l = v - (p_h - u);
		z_h = cp_h * p_h;				/* cp_h+cp_l = 2/(3*log2) */
		z_l = cp_l * p_h + p_l * cp + dp_l[k];
		/* log2(ax) = (s+..)*2/(3*log2) = n + dp_h + z_h + z_l */
		t = (double) n;
		t1 = (((z_h + z_l) + dp_h[k]) + t);
		SET_LOW_WORD(t1, 0);
		t2 = z_l - (((t1 - t) - dp_h[k]) - z_h);
	}

	s = one;							/* s (sign of result -ve**odd) = -1 else = 1 */
	if (((((uint32_t) hx >> 31) - 1) | (yisint - 1)) == 0)
		s = -one;						/* (-ve)**(odd int) */

	/* split up y into y1+y2 and compute (y1+y2)*(t1+t2) */
	y1 = y;
	SET_LOW_WORD(y1, 0);
	p_l = (y - y1) * t1 + y * t2;
	p_h = y1 * t1;
	z = p_l + p_h;
	GET_DOUBLE_WORDS(j, i, z);
	if (j >= IC(0x40900000))
	{									/* z >= 1024 */
		if (((j - IC(0x40900000)) | i) != 0)	/* if z > 1024 */
		{
			feraiseexcept(FE_OVERFLOW);
			return copysign(HUGE_VAL, s);
		}
		if (p_l + ovt > z - p_h)
		{
			feraiseexcept(FE_OVERFLOW);
			return copysign(HUGE_VAL, s);
		}
	} else if ((j & IC(0x7fffffff)) >= IC(0x4090cc00))
	{									/* z <= -1075 */
		if (((j - IC(0xc090cc00)) | i) != 0)	/* z < -1075 */
		{
			feraiseexcept(FE_UNDERFLOW);
			return copysign(0.0, s);
		}
		if (p_l <= z - p_h)
		{
			feraiseexcept(FE_UNDERFLOW);
			return copysign(0.0, s);
		}
	}
	/*
	 * compute 2**(p_h+p_l)
	 */
	i = j & IC(0x7fffffff);
	k = (i >> IEEE754_DOUBLE_SHIFT) - IEEE754_DOUBLE_BIAS;
	n = 0;
	if (i > IC(0x3fe00000))
	{									/* if |z| > 0.5, set n = [z+0.5] */
		n = j + (IC(0x00100000) >> (k + 1));
		k = ((n & IC(0x7fffffff)) >> IEEE754_DOUBLE_SHIFT) - IEEE754_DOUBLE_BIAS;	/* new k for n */
		t = zero;
		SET_HIGH_WORD(t, n & ~(UC(0x000fffff) >> k));
		n = ((n & IC(0x000fffff)) | IC(0x00100000)) >> (IEEE754_DOUBLE_SHIFT - k);
		if (j < 0)
			n = -n;
		p_h -= t;
	}
	t = p_l + p_h;
	SET_LOW_WORD(t, 0);
	u = t * lg2_h;
	v = (p_l - (t - p_h)) * lg2 + t * lg2_l;
	z = u + v;
	w = v - (z - u);
	t = z * z;
	t1 = z - t * (P1 + t * (P2 + t * (P3 + t * (P4 + t * P5))));
	r = (z * t1) / (t1 - two) - (w + z * w);
	z = one - (r - z);
	GET_HIGH_WORD(j, z);
	j += (n << IEEE754_DOUBLE_SHIFT);
	if ((j >> IEEE754_DOUBLE_SHIFT) <= 0)
	{
		z = __ieee754_scalbn(z, (int) n);			/* subnormal output */
	} else
	{
		SET_HIGH_WORD(z, j);
	}
	return s * z;
}

#endif


/* wrapper pow */
double __pow(double x, double y)
{
	double z = __ieee754_pow(x, y);

	if (!isfinite(z))
	{
		if (_LIB_VERSION != _IEEE_)
		{
			if (isnan(x))
			{
				if (y == 0.0)
					/* pow(NaN,0.0) */
					return __kernel_standard(x, y, z, KMATHERR_POW_NAN);
			} else if (isfinite(x) && isfinite(y))
			{
				if (isnan(z))
				{
					/* pow neg**non-int */
					return __kernel_standard(x, y, z, KMATHERR_POW_NONINT);
				} else if (x == 0.0 && y < 0.0)
				{
					if (signbit(x) && signbit(z))
						/* pow(-0.0,negative) */
						return __kernel_standard(x, y, z, KMATHERR_POW_MINUS);
					else
						/* pow(+0.0,negative) */
						return __kernel_standard(x, y, z, KMATHERR_POW_ZEROMINUS);
				} else
				{
					/* pow overflow */
					return __kernel_standard(x, y, z, KMATHERR_POW_OVERFLOW);
				}
			}
		}
	} else if (z == 0.0 && isfinite(x) && isfinite(y) && _LIB_VERSION != _IEEE_)
	{
		if (x == 0.0)
		{
			if (y == 0.0)
				/* pow(0.0,0.0) */
				return __kernel_standard(x, y, z, KMATHERR_POW_ZERO);
		} else
		{
			/* pow underflow */
			return __kernel_standard(x, y, z, KMATHERR_POW_UNDERFLOW);
		}
	}

	return z;
}


__typeof(__pow) pow __attribute__((weak, alias("__pow")));
#ifdef __NO_LONG_DOUBLE_MATH
long double __powl(long double x, long double y) __attribute__((alias("__pow")));
__typeof(__powl) powl __attribute__((weak, alias("__pow")));
#endif
