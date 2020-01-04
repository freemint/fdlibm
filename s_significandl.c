#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

long double __significandl(long double x)
{
	uint32_t exp, ix;

	GET_LDOUBLE_EXP(exp, x);
	ix = exp & 0x7fff;
	if (ix != 0 && ix != 0x7fff)
		SET_LDOUBLE_EXP(x, (exp & 0x8000) | 0x3fff);
	return x;
}

__typeof(__significandl) significandl __attribute__((weak, alias("__significandl")));

#endif
