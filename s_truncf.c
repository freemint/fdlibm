#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_trunc

float __ieee754_truncf(float x)
{
	int32_t i0, j0;
	int32_t sx;

	GET_FLOAT_WORD(i0, x);
	sx = i0 & UC(0x80000000);
	j0 = ((i0 >> IEEE754_FLOAT_SHIFT) & IEEE754_FLOAT_MAXEXP) - IEEE754_FLOAT_BIAS;
	if (j0 < IEEE754_FLOAT_SHIFT)
	{
		if (j0 < 0)
		{
			/* The magnitude of the number is < 1 so the result is +-0.  */
			SET_FLOAT_WORD(x, sx);
		} else
		{
			SET_FLOAT_WORD(x, sx | (i0 & ~(UC(0x007fffff) >> j0)));
		}
	} else
	{
		if (j0 == (IEEE754_FLOAT_MAXEXP - IEEE754_FLOAT_BIAS))
			/* x is inf or NaN.  */
			return x + x;
	}

	return x;
}

#endif

float __truncf(float x)
{
	return __ieee754_truncf(x);
}

__typeof(__truncf) truncf __attribute__((weak, alias("__truncf")));
