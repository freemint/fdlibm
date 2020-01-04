/* @(#)e_fmod.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

/*
 * __ieee754_fmod(x,y)
 * Return x mod y in exact arithmetic
 * Method: shift and subtract
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_fmod

double __ieee754_fmod(double x, double y)
{
	int32_t n, hx, hy, hz, ix, iy, sx, i;
	uint32_t lx, ly, lz;

	static const double one = 1.0;
	static const double Zero[] = { 0.0, -0.0 };

	GET_DOUBLE_WORDS(hx, lx, x);
	GET_DOUBLE_WORDS(hy, ly, y);
	sx = hx & IC(0x80000000);			/* sign of x */
	hx ^= sx;							/* |x| */
	hy &= IC(0x7fffffff);				/* |y| */

	/* purge off exception values */
	if ((hy | ly) == 0 || (hx >= IC(0x7ff00000)) ||	/* y=0,or x not finite */
		((hy | ((ly | -ly) >> 31)) > IC(0x7ff00000)))	/* or y is NaN */
		return (x * y) / (x * y);
	if (hx <= hy)
	{
		if ((hx < hy) || (lx < ly))
			return x;					/* |x|<|y| return x */
		if (lx == ly)
			return Zero[(uint32_t) sx >> 31];	/* |x|=|y| return x*0 */
	}

	/* determine ix = ilogb(x) */
	if (hx < IC(0x00100000))
	{									/* subnormal x */
		if (hx == 0)
		{
			for (ix = -1043, i = lx; i > 0; i <<= 1)
				ix -= 1;
		} else
		{
			for (ix = -1022, i = (hx << 11); i > 0; i <<= 1)
				ix -= 1;
		}
	} else
	{
		ix = (hx >> 20) - 1023;
	}

	/* determine iy = ilogb(y) */
	if (hy < IC(0x00100000))
	{									/* subnormal y */
		if (hy == 0)
		{
			for (iy = -1043, i = ly; i > 0; i <<= 1)
				iy -= 1;
		} else
		{
			for (iy = -1022, i = (hy << 11); i > 0; i <<= 1)
				iy -= 1;
		}
	} else
	{
		iy = (hy >> 20) - 1023;
	}

	/* set up {hx,lx}, {hy,ly} and align y to x */
	if (ix >= -1022)
		hx = IC(0x00100000) | (IC(0x000fffff) & hx);
	else
	{									/* subnormal x, shift x to normal */
		n = -1022 - ix;
		if (n <= 31)
		{
			hx = (hx << n) | (lx >> (32 - n));
			lx <<= n;
		} else
		{
			hx = lx << (n - 32);
			lx = 0;
		}
	}
	if (iy >= -1022)
		hy = IC(0x00100000) | (IC(0x000fffff) & hy);
	else
	{									/* subnormal y, shift y to normal */
		n = -1022 - iy;
		if (n <= 31)
		{
			hy = (hy << n) | (ly >> (32 - n));
			ly <<= n;
		} else
		{
			hy = ly << (n - 32);
			ly = 0;
		}
	}

	/* fix point fmod */
	n = ix - iy;
	while (n--)
	{
		hz = hx - hy;
		lz = lx - ly;
		if (lx < ly)
			hz -= 1;
		if (hz < 0)
		{
			hx = hx + hx + (lx >> 31);
			lx = lx + lx;
		} else
		{
			if ((hz | lz) == 0)			/* return sign(x)*0 */
				return Zero[(uint32_t) sx >> 31];
			hx = hz + hz + (lz >> 31);
			lx = lz + lz;
		}
	}
	hz = hx - hy;
	lz = lx - ly;
	if (lx < ly)
		hz -= 1;
	if (hz >= 0)
	{
		hx = hz;
		lx = lz;
	}

	/* convert back to floating value and restore the sign */
	if ((hx | lx) == 0)					/* return sign(x)*0 */
		return Zero[(uint32_t) sx >> 31];
	while (hx < IC(0x00100000))
	{									/* normalize x */
		hx = hx + hx + (lx >> 31);
		lx = lx + lx;
		iy -= 1;
	}
	if (iy >= -1022)
	{									/* normalize output */
		hx = ((hx - IC(0x00100000)) | ((iy + 1023) << 20));
		INSERT_WORDS(x, hx | sx, lx);
	} else
	{									/* subnormal output */
		n = -1022 - iy;
		if (n <= 20)
		{
			lx = (lx >> n) | ((uint32_t) hx << (32 - n));
			hx >>= n;
		} else if (n <= 31)
		{
			lx = (hx << (32 - n)) | (lx >> n);
			hx = sx;
		} else
		{
			lx = hx >> (n - 32);
			hx = sx;
		}
		INSERT_WORDS(x, hx | sx, lx);
		x *= one;						/* create necessary signal */
	}
	return x;							/* exact output */
}

#endif

double __fmod(double x, double y)
{
	if (_LIB_VERSION != _IEEE_ && (isinf(x) || y == 0.0) && !isunordered(x, y))
		/* fmod(+-Inf,y) or fmod(x,0) */
		return __kernel_standard(x, y, y, KMATHERR_FMOD);
	return __ieee754_fmod(x, y);
}

__typeof(__fmod) fmod __attribute__((weak, alias("__fmod")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__fmodl) __fmodl __attribute__((alias("__fmod")));
__typeof(__fmodl) fmodl __attribute__((weak, alias("__fmod")));
#endif
