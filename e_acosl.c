#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_acos

long double __ieee754_acosl(long double x)
{
	long double y;
	
	if (x == 0.0)
		return M_PI_2l;
	if (x == 1.0)
		return 0.0;
	if (x == -1.0)
		return M_PIl;
	y = __ieee754_atanl(__ieee754_sqrtl(1.0L - (x * x)) / x);
	if (x > 0.0)
		return y;
	return y + M_PIl;
}

#endif

/* wrapper acosl */
long double __acosl(long double x)
{
	if (isgreater(__ieee754_fabsl(x), 1.0L) && _LIB_VERSION != _IEEE_)
	{
		/* acos(|x|>1) */
		feraiseexcept(FE_INVALID);
		return __kernel_standard_l(x, x, __builtin_nanl(""), KMATHERRL_ACOS);
	}

	return __ieee754_acosl(x);
}

__typeof(__acosl) acosl __attribute__((weak, alias("__acosl")));

#endif
