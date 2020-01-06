#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_sincos

void __ieee754_sincos(double x, double *sinx, double *cosx)
{
	*sinx = __ieee754_sin(x);
	*cosx = __ieee754_cos(x);
}

#endif

void __sincos(double x, double *sinx, double *cosx)
{
	int32_t ix;

	/* High word of x. */
	GET_HIGH_WORD(ix, x);

	/* |x| ~< pi/4 */
	ix &= IC(0x7fffffff);
	if (ix >= IC(0x7ff00000))
	{
		/* sin(Inf or NaN) is NaN */
		*sinx = *cosx = x - x;
	} else
	{
		__ieee754_sincos(x, sinx, cosx);
	}
}

__typeof(__sincos) sincos __attribute__((weak, alias("__sincos")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__sincosl) __sincosl __attribute__((alias("__sincos")));
__typeof(__sincosl) sincosl __attribute__((weak, alias("__sincos")));
#endif
