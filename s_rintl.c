#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

/*
 * rintl(x)
 * Return x rounded to integral value according to the prevailing
 * rounding mode.
 * Method:
 *	Using floating addition.
 * Exception:
 *	Inexact flag raised if x not equal to rintl(x).
 */

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_rint

long double __ieee754_rintl(long double x)
{
	int32_t j0;
	uint32_t se, i1, i0;

	static const long double hugeval = 1.0e4930L;
	
	GET_LDOUBLE_WORDS(se, i0, i1, x);
	j0 = (se & IEEE854_LONG_DOUBLE_MAXEXP) - IEEE854_LONG_DOUBLE_BIAS;
	if (j0 < 31)
	{
		if (j0 < 0)
		{
			math_force_eval(hugeval + x);
			se &= 0x8000;
			if (j0 == -1 && (i0 & UC(0x7fffffff)) != 0)
			{
				se |= 0x3fff;
				i0 = UC(0x80000000);
			} else
			{
				i0 = 0;
			}
			i1 = 0;
		} else
		{
			uint32_t i = UC(0x7fffffff) >> j0;
			uint32_t j;
			
			if (((i0 & i) | i1) == 0)
				/* X is integral.  */
				return x;

			/* Raise inexact if x != 0.  */
			math_force_eval(hugeval + x);
			j = i0;
			if ((i0 & (i >> 1)) != 0 || i1 != 0 || (i0 & (UC(0x80000000) >> j0)))
			{
				j += (UC(0x40000000) >> j0);

				if (j < i0)
					se += 1;
			}
			i0 = (j & ~i) | IC(0x80000000);
			i1 = 0;
		}
	} else if (j0 > 62)
	{
		if (j0 == (IEEE854_LONG_DOUBLE_MAXEXP - IEEE854_LONG_DOUBLE_BIAS))
			/* Inf or NaN.  */
			return x + x;
		else
			return x;
	} else
	{
		uint32_t i = UC(0xffffffff) >> (j0 - 31);
		uint32_t j;
		
		if ((i1 & i) == 0)
			/* X is integral.  */
			return x;

		/* Raise inexact if x != 0.  */
		math_force_eval(hugeval + x);

		if ((i1 & (i >> 1)) != 0 || (j0 == 32 && (i0 & 1)) || (i1 & (UC(1) << (63 - j0))))
		{
			j = i1 + (UC(1) << (62 - j0));

			if (j < i1)
			{
				uint32_t k = i0 + 1;

				if (k < i0)
				{
					se += 1;
					k |= UC(0x80000000);
				}
				i0 = k;
			}
			i1 = j;
		}
		i1 &= ~i;
	}

	SET_LDOUBLE_WORDS(x, se, i0, i1);
	return x;
}

#endif

long double __rintl(long double x)
{
	return __ieee754_rintl(x);
}

__typeof(__rintl) rintl __attribute__((weak, alias("__rintl")));

#endif
