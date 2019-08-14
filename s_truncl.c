#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_trunc

long double __ieee754_truncl(long double x)
{
	int32_t i0, j0;
	uint32_t se, i1;
	int32_t sx;

	GET_LDOUBLE_WORDS(se, i0, i1, x);
	sx = se & 0x8000;
	j0 = (se & IEEE854_LONG_DOUBLE_MAXEXP) - IEEE854_LONG_DOUBLE_BIAS;
	if (j0 < 31)
	{
		if (j0 < 0)
			/* The magnitude of the number is < 1 so the result is +-0.  */
			SET_LDOUBLE_WORDS(x, sx, 0, 0);
		else
			SET_LDOUBLE_WORDS(x, se, i0 & ~(UC(0x7fffffff) >> j0), 0);
	} else if (j0 > 63)
	{
		if (j0 == (IEEE854_LONG_DOUBLE_MAXEXP - IEEE854_LONG_DOUBLE_BIAS))
			/* x is inf or NaN.  */
			return x + x;
	} else
	{
		/* avoid undefined behaviour for shift-count == 32 */
		if (j0 != 63)
			SET_LDOUBLE_WORDS(x, se, i0, i1 & ~(UC(0xffffffff) >> (j0 - 31)));
	}

	return x;
}

#endif

long double __truncl(long double x)
{
	return __ieee754_truncl(x);
}

__typeof(__truncl) truncl __attribute__((weak, alias("__truncl")));

#endif
