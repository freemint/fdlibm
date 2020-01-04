#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_exp10

double __ieee754_exp10(double arg)
{
	if (isfinite(arg) && arg < DBL_MIN_10_EXP - DBL_DIG - 10)
	{
		feraiseexcept(FE_UNDERFLOW);
		return 0;
	}
	/* This is a very stupid and inprecise implementation.  It'll get
	   replaced sometime (soon?).  */
	return __ieee754_exp(M_LN10 * arg);
}

#endif


double __exp10(double x)
{
	double z = __ieee754_exp10(x);

	if (!isfinite(z) && isfinite(x) && _LIB_VERSION != _IEEE_)
		/* exp10 overflow (46) if x > 0, underflow (47) if x < 0.  */
		return __kernel_standard(x, x, z, signbit(x) ? KMATHERR_EXP10_UNDERFLOW : KMATHERR_EXP10_OVERFLOW);

	return z;
}



__typeof(__exp10) exp10 __attribute__((weak, alias("__exp10")));
__typeof(__exp10) __pow10 __attribute__((alias("__exp10")));
__typeof(__pow10) pow10 __attribute__((weak, alias("__pow10")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__exp10l) __exp10l __attribute__((alias("__exp10")));
__typeof(__exp10l) exp10l __attribute__((weak, alias("__exp10l")));
__typeof(__pow10l) __pow10l __attribute__((alias("__pow10")));
__typeof(__pow10l) pow10l __attribute__((weak, alias("__pow10l")));
#endif
