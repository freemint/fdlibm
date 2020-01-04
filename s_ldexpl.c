#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_ldexp

long double __ieee754_ldexpl(long double value, int exp)
{
	if (!isfinite(value) || value == 0.0)
		return value;
	return __ieee754_scalbnl(value, exp);
}

#endif

long double __ldexpl(long double x, int n)
{
	x = __ieee754_ldexpl(x, n);
	if (!isfinite(x) || x == 0.0)
		__set_errno(ERANGE);
	return x;
}

__typeof(__ldexpl) ldexpl __attribute__((weak, alias("__ldexpl")));

#endif
