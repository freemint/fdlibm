#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

float __fminf(float x, float y)
{
	return (islessequal (x, y) || isnan (y)) ? x : y;
}

__typeof(__fminf) fminf __attribute__((weak, alias("__fminf")));
