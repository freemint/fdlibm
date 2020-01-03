#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_frexp

long double __ieee754_frexpl(long double x, int *eptr)
{
	static const long double two65 = 3.68934881474191032320e+19L;	/* 0x4040, 0x80000000, 0x00000000 */

	uint32_t se, hx, ix, lx;

	GET_LDOUBLE_WORDS(se, hx, lx, x);
	ix = 0x7fff & se;
	*eptr = 0;
	if (ix == 0x7fff || ((ix | hx | lx) == 0))
		return x;						/* 0,inf,nan */
	if (ix == 0x0000)
	{									/* subnormal */
		x *= two65;
		GET_LDOUBLE_EXP(se, x);
		ix = se & 0x7fff;
		*eptr = -65;
	}
	*eptr += (int)(ix - 16382);
	se = (se & 0x8000) | 0x3ffe;
	SET_LDOUBLE_EXP(x, se);
	return x;
}

#endif

long double __frexpl(long double x, int *exp)
{
	return __ieee754_frexpl(x, exp);
}

__typeof(__frexpl) frexpl __attribute__((weak, alias("__frexpl")));

#endif
