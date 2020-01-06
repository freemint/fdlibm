#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_sincos

void __ieee754_sincosl(long double x, long double *sinx, long double *cosx)
{
	*sinx = __ieee754_sinl(x);
	*cosx = __ieee754_cosl(x);
}

#endif

void __sincosl(long double x, long double *sinx, long double *cosx)
{
	int32_t se;
	uint32_t i0, i1;

	/* High word of x. */
	GET_LDOUBLE_WORDS(se, i0, i1, x);
	(void) i1;
	
	/* |x| ~< pi/4 */
	se &= 0x7fff;
	if (se < 0x3ffe || (se == 0x3ffe && i0 <= UC(0xc90fdaa2)))
	{
#ifdef __have_fpu_sincos
		__ieee754_sincosl(x, sinx, cosx);
#else
		*sinx = __kernel_sinl(x, 0.0, 0);
		*cosx = __kernel_cosl(x, 0.0);
#endif
	} else if (se == 0x7fff)
	{
		/* sin(Inf or NaN) is NaN */
		*sinx = *cosx = x - x;
	} else
	{
#ifdef __have_fpu_sincos
		__ieee754_sincosl(x, sinx, cosx);
#else
		/* Argument reduction needed.  */
		long double y[2];
		int32_t n;

		n = __ieee754_rem_pio2l(x, y);
		switch ((int)(n & 3))
		{
		case 0:
			*sinx = __kernel_sinl(y[0], y[1], 1);
			*cosx = __kernel_cosl(y[0], y[1]);
			break;
		case 1:
			*sinx = __kernel_cosl(y[0], y[1]);
			*cosx = -__kernel_sinl(y[0], y[1], 1);
			break;
		case 2:
			*sinx = -__kernel_sinl(y[0], y[1], 1);
			*cosx = -__kernel_cosl(y[0], y[1]);
			break;
		default:
			*sinx = -__kernel_cosl(y[0], y[1]);
			*cosx = __kernel_sinl(y[0], y[1], 1);
			break;
		}
#endif
	}
}

__typeof(__sincosl) sincosl __attribute__((weak, alias("__sincosl")));

#endif
