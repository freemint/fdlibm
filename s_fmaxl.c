#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

long double __fmaxl(long double x, long double y)
{
	return (isgreaterequal (x, y) || isnan (y)) ? x : y;
}

__typeof(__fmaxl) fmaxl __attribute__((weak, alias("__fmaxl")));

#endif
