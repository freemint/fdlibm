#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_ceil

long double __ieee754_ceill(long double  x)
{
	int32_t j0;
	uint32_t i, j, se, i0, i1, sx;

	static const long double hugeval = 1.0e4930L;
	
	GET_LDOUBLE_WORDS(se, i0, i1, x);
	sx = (se >> 15) & 1;
	j0 = (se & IEEE854_LONG_DOUBLE_MAXEXP) - IEEE854_LONG_DOUBLE_BIAS;
	if (j0 < 31)
	{
		if (j0 < 0)
		{								/* raise inexact if x != 0 */
			math_force_eval(hugeval + x);
			/* return 0*sign(x) if |x|<1 */
			if (sx)
			{
				se = 0x8000;
				i0 = UC(0x80000000);
				i1 = 0;
			} else if ((i0 | i1) != 0)
			{
				se = 0x3fff;
				i0 = UC(0x80000000);
				i1 = 0;
			}
		} else
		{
			i = UC(0x7fffffff) >> j0;
			if (((i0 & i) | i1) == 0)
				return x;				/* x is integral */
			math_force_eval(hugeval + x);	/* raise inexact flag */
			if (sx == 0)
			{
				if (j0 > 0 && (i0 + (UC(0x80000000) >> j0)) > i0)
					i0 += UC(0x80000000) >> j0;
				else
				{
					i = UC(0x7fffffff);
					++se;
				}
			}
			i0 &= (~i);
			i1 = 0;
		}
	} else if (j0 > 62)
	{
		if (j0 == (IEEE854_LONG_DOUBLE_MAXEXP - IEEE854_LONG_DOUBLE_BIAS))
			return x + x;				/* inf or NaN */
		else
			return x;					/* x is integral */
	} else
	{
		i = UC(0xffffffff) >> (j0 - 31);
		if ((i1 & i) == 0)
			return x;					/* x is integral */
		math_force_eval(hugeval + x);		/* raise inexact flag */
		if (sx == 0)
		{
			if (j0 == 31)
				i0 += 1;
			else
			{
				j = i1 + (UC(1) << (63 - j0));
				if (j < i1)
					i0 += 1;		/* got a carry */
				i1 = j;
			}
		}
		i1 &= (~i);
	}
	SET_LDOUBLE_WORDS(x, se, i0, i1);
	return x;
}

#endif

long double __ceill(long double  x)
{
	return __ieee754_ceill(x);
}

__typeof(__ceill) ceill __attribute__((weak, alias("__ceill")));

#endif
