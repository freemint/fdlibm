#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_frexp

float __ieee754_frexpf(float x, int *eptr)
{
	int32_t hx, ix;
	static const float two25 = 3.3554432000e+07;				/* 0x4c000000 */

	GET_FLOAT_WORD(hx, x);
	ix = UC(0x7fffffff) & hx;
	*eptr = 0;
	if (!FLT_UWORD_IS_FINITE(ix) || FLT_UWORD_IS_ZERO(ix))
		return x;						/* 0,inf,nan */
	if (FLT_UWORD_IS_SUBNORMAL(ix))
	{									/* subnormal */
		x *= two25;
		GET_FLOAT_WORD(hx, x);
		ix = hx & UC(0x7fffffff);
		*eptr = -25;
	}
	*eptr += (int)(ix >> 23) - 126;
	hx = (hx & UC(0x807fffff)) | UC(0x3f000000);
	SET_FLOAT_WORD(x, hx);
	return x;
}

#endif

float __frexpf(float x, int *exp)
{
	return __ieee754_frexpf(x, exp);
}

__typeof(__frexpf) frexpf __attribute__((weak, alias("__frexpf")));
