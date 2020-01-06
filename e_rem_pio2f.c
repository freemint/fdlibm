/* e_rem_pio2f.c -- float version of e_rem_pio2.c
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

/* __ieee754_rem_pio2f(x,y)
 *
 * return the remainder of x rem pi/2 in y[0]+y[1]
 * use __kernel_rem_pio2f()
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif


int32_t __ieee754_rem_pio2f(float x, float *y)
{
	float z, w, t, r, fn;
	float tx[3];
	int32_t e0, i, j, nx, n, ix, hx;

	/* This array is like the one in e_rem_pio2.c, but the numbers are
	   single precision and the last 8 bits are forced to 0.  */
	static const int32_t npio2_hw[] = {
		IC(0x3fc90f00), IC(0x40490f00), IC(0x4096cb00), IC(0x40c90f00), IC(0x40fb5300), IC(0x4116cb00),
		IC(0x412fed00), IC(0x41490f00), IC(0x41623100), IC(0x417b5300), IC(0x418a3a00), IC(0x4196cb00),
		IC(0x41a35c00), IC(0x41afed00), IC(0x41bc7e00), IC(0x41c90f00), IC(0x41d5a000), IC(0x41e23100),
		IC(0x41eec200), IC(0x41fb5300), IC(0x4203f200), IC(0x420a3a00), IC(0x42108300), IC(0x4216cb00),
		IC(0x421d1400), IC(0x42235c00), IC(0x4229a500), IC(0x422fed00), IC(0x42363600), IC(0x423c7e00),
		IC(0x4242c700), IC(0x42490f00)
	};
	
	/*
	 * invpio2:  24 bits of 2/pi
	 * pio2_1:   first  17 bit of pi/2
	 * pio2_1t:  pi/2 - pio2_1
	 * pio2_2:   second 17 bit of pi/2
	 * pio2_2t:  pi/2 - (pio2_1+pio2_2)
	 * pio2_3:   third  17 bit of pi/2
	 * pio2_3t:  pi/2 - (pio2_1+pio2_2+pio2_3)
	 */
	
	static const float zero = 0.0000000000e+00;	/* 0x00000000 */
	static const float half = 5.0000000000e-01;			/* 0x3f000000 */
	static const float two8 = 2.5600000000e+02;			/* 0x43800000 */
	static const float invpio2 = 6.3661980629e-01;			/* 0x3f22f984 */
	static const float pio2_1 = 1.5707855225e+00;			/* 0x3fc90f80 */
	static const float pio2_1t = 1.0804334124e-05;			/* 0x37354443 */
	static const float pio2_2 = 1.0804273188e-05;			/* 0x37354400 */
	static const float pio2_2t = 6.0770999344e-11;			/* 0x2e85a308 */
	static const float pio2_3 = 6.0770943833e-11;			/* 0x2e85a300 */
	static const float pio2_3t = 6.1232342629e-17;			/* 0x248d3132 */

	GET_FLOAT_WORD(hx, x);
	ix = hx & IC(0x7fffffff);
	if (ix <= IC(0x3f490fd8))				/* |x| ~<= pi/4 , no need for reduction */
	{
		y[0] = x;
		y[1] = 0;
		return 0;
	}
	if (ix < IC(0x4016cbe4))
	{									/* |x| < 3pi/4, special case with n=+-1 */
		if (hx > 0)
		{
			z = x - pio2_1;
			if ((ix & IC(0xfffffff0)) != IC(0x3fc90fd0))
			{							/* 24+24 bit pi OK */
				y[0] = z - pio2_1t;
				y[1] = (z - y[0]) - pio2_1t;
			} else
			{							/* near pi/2, use 24+24+24 bit pi */
				z -= pio2_2;
				y[0] = z - pio2_2t;
				y[1] = (z - y[0]) - pio2_2t;
			}
			return 1;
		} else
		{								/* negative x */
			z = x + pio2_1;
			if ((ix & IC(0xfffffff0)) != IC(0x3fc90fd0))
			{							/* 24+24 bit pi OK */
				y[0] = z + pio2_1t;
				y[1] = (z - y[0]) + pio2_1t;
			} else
			{							/* near pi/2, use 24+24+24 bit pi */
				z += pio2_2;
				y[0] = z + pio2_2t;
				y[1] = (z - y[0]) + pio2_2t;
			}
			return -1;
		}
	}
	if (ix <= IC(0x43490f80))
	{									/* |x| ~<= 2^7*(pi/2), medium size */
		t = __ieee754_fabsf(x);
		n = (int32_t) (t * invpio2 + half);
		fn = (float) n;
		r = t - fn * pio2_1;
		w = fn * pio2_1t;				/* 1st round good to 40 bit */
		if (n < 32 && (int32_t)(ix & IC(0xffffff00)) != npio2_hw[n - 1])
		{
			y[0] = r - w;				/* quick check no cancellation */
		} else
		{
			uint32_t high;

			j = ix >> IEEE754_FLOAT_SHIFT;
			y[0] = r - w;
			GET_FLOAT_WORD(high, y[0]);
			i = j - ((high >> IEEE754_FLOAT_SHIFT) & IEEE754_FLOAT_MAXEXP);
			if (i > 8)
			{							/* 2nd iteration needed, good to 57 */
				t = r;
				w = fn * pio2_2;
				r = t - w;
				w = fn * pio2_2t - ((t - r) - w);
				y[0] = r - w;
				GET_FLOAT_WORD(high, y[0]);
				i = j - ((high >> IEEE754_FLOAT_SHIFT) & IEEE754_FLOAT_MAXEXP);
				if (i > 25)
				{						/* 3rd iteration need, 74 bits acc */
					t = r;				/* will cover all possible cases */
					w = fn * pio2_3;
					r = t - w;
					w = fn * pio2_3t - ((t - r) - w);
					y[0] = r - w;
				}
			}
		}
		y[1] = (r - y[0]) - w;
		if (hx < 0)
		{
			y[0] = -y[0];
			y[1] = -y[1];
			return -n;
		} else
			return n;
	}
	/*
	 * all other (large) arguments
	 */
	if (!FLT_UWORD_IS_FINITE(ix))
	{									/* x is inf or NaN */
		y[0] = y[1] = x - x;
		return 0;
	}
	/* set z = scalbn(|x|,ilogb(x)-7) */
	e0 = (ix >> IEEE754_FLOAT_SHIFT) - (IEEE754_FLOAT_BIAS + 7);				/* e0 = ilogb(z)-7; */
	SET_FLOAT_WORD(z, ix - (e0 << IEEE754_FLOAT_SHIFT));
	for (i = 0; i < 2; i++)
	{
		tx[i] = (float) ((int32_t) (z));
		z = (z - tx[i]) * two8;
	}
	tx[2] = z;
	nx = 3;
	while (tx[nx - 1] == zero)
		nx--;							/* skip zero term */
	n = __kernel_rem_pio2f(tx, y, e0, nx, 2);
	if (hx < 0)
	{
		y[0] = -y[0];
		y[1] = -y[1];
		return -n;
	}
	return n;
}
