#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_ldexp

float __ieee754_ldexpf(float value, int exp)
{
	if (!isfinite(value) || value == 0.0F)
		return value;
	return __ieee754_scalbnf(value, exp);
}

#endif

float __ldexpf(float x, int n)
{
	x = __ieee754_ldexpf(x, n);
	if (!isfinite(x) || x == 0.0F)
		__set_errno(ERANGE);
	return x;
}

__typeof(__ldexpf) ldexpf __attribute__((weak, alias("__ldexpf")));
