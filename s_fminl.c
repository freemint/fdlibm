#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

long double __fminl(long double x, long double y)
{
	return (islessequal (x, y) || isnan (y)) ? x : y;
}

__typeof(__fminl) fminl __attribute__((weak, alias("__fminl")));

#endif
