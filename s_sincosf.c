#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_sincos

void __ieee754_sincosf(float x, float *sinx, float *cosx)
{
	*sinx = sinf(x);
	*cosx = cosf(x);
}

#endif

void __sincosf(float x, float *sinx, float *cosx)
{
	int32_t ix;

	/* High word of x. */
	GET_FLOAT_WORD(ix, x);

	/* |x| ~< pi/4 */
	ix &= IC(0x7fffffff);
	if (ix <= IC(0x3f490fd8))
	{
#ifdef __have_fpu_sincos
		__ieee754_sincosf(x, sinx, cosx);
#else
		*sinx = __kernel_sinf(x, 0.0, 0);
		*cosx = __kernel_cosf(x, 0.0);
#endif
	} else if (ix >= IC(0x7f800000))
	{
		/* sin(Inf or NaN) is NaN */
		*sinx = *cosx = x - x;
	} else
	{
#ifdef __have_fpu_sincos
		__ieee754_sincosf(x, sinx, cosx);
#else
		/* Argument reduction needed.  */
		float y[2];
		int32_t n;

		n = __ieee754_rem_pio2f(x, y);
		switch ((int)(n & 3))
		{
		case 0:
			*sinx = __kernel_sinf(y[0], y[1], 1);
			*cosx = __kernel_cosf(y[0], y[1]);
			break;
		case 1:
			*sinx = __kernel_cosf(y[0], y[1]);
			*cosx = -__kernel_sinf(y[0], y[1], 1);
			break;
		case 2:
			*sinx = -__kernel_sinf(y[0], y[1], 1);
			*cosx = -__kernel_cosf(y[0], y[1]);
			break;
		default:
			*sinx = -__kernel_cosf(y[0], y[1]);
			*cosx = __kernel_sinf(y[0], y[1], 1);
			break;
		}
#endif
	}
}

__typeof(__sincosf) sincosf __attribute__((weak, alias("__sincosf")));
