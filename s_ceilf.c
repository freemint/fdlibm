#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_ceil

float __ieee754_ceilf(float x)
{
	int32_t i0, j0;
	uint32_t i;

	static const float hugeval = 1.0e30;
	
	GET_FLOAT_WORD(i0, x);
	j0 = ((i0 >> IEEE754_FLOAT_SHIFT) & IEEE754_FLOAT_MAXEXP) - IEEE754_FLOAT_BIAS;
	if (j0 < IEEE754_FLOAT_SHIFT)
	{
		if (j0 < 0)
		{								/* raise inexact if x != 0 */
			math_force_eval(hugeval + x);	/* return 0*sign(x) if |x|<1 */
			if (i0 < 0)
			{
				i0 = IC(0x80000000);
			} else if (i0 != 0)
			{
				i0 = IC(0x3f800000);
			}
		} else
		{
			i = UC(0x007fffff) >> j0;
			if ((i0 & i) == 0)
				return x;				/* x is integral */
			math_force_eval(hugeval + x);	/* raise inexact flag */
			if (i0 > 0)
				i0 += UC(0x00800000) >> j0;
			i0 &= (~i);
		}
	} else
	{
		if (j0 == (IEEE754_FLOAT_MAXEXP - IEEE754_FLOAT_BIAS))
			return x + x;				/* inf or NaN */
		else
			return x;					/* x is integral */
	}
	SET_FLOAT_WORD(x, i0);
	return x;
}

#endif

float __ceilf(float x)
{
	return __ieee754_ceilf(x);
}

__typeof(__ceilf) ceilf __attribute__((weak, alias("__ceilf")));
