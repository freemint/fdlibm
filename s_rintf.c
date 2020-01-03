#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_rint

float __ieee754_rintf(float x)
{
	int32_t i0, j0;

	static const float hugeval = 1.0e30;
	
	GET_FLOAT_WORD(i0, x);
	j0 = ((i0 >> IEEE754_FLOAT_SHIFT) & IEEE754_FLOAT_MAXEXP) - IEEE754_FLOAT_BIAS;
	if (j0 < IEEE754_FLOAT_SHIFT)
	{
		if (j0 < 0)
		{
			math_force_eval(hugeval + x);

			if (j0 == -1 && (i0 & IC(0x007fffff)) != 0)
			{
				i0 &= IC(0x80000000);
				i0 |= IC(0x3f800000);
			} else
			{
				i0 &= IC(0x80000000);
			}
		} else
		{
			uint32_t i = UC(0x007fffff) >> j0;

			if ((i0 & i) == 0)
				/* X is integral.  */
				return x;

			/* Raise inexact if x != 0.  */
			math_force_eval(hugeval + x);

			if ((i0 & (i >> 1)) != 0 || (i0 & (UC(0x00800000) >> j0)))
			{
				i0 += UC(0x00400000) >> j0;
			}
			i0 &= ~i;
		}
	} else
	{
		if (j0 == (IEEE754_FLOAT_MAXEXP - IEEE754_FLOAT_BIAS))
			/* Inf or NaN.  */
			return x + x;
		else
			return x;
	}

	SET_FLOAT_WORD(x, i0);
	return x;
}

#endif

float __rintf(float x)
{
	return __ieee754_rintf(x);
}

__typeof(__rintf) rintf __attribute__((weak, alias("__rintf")));
