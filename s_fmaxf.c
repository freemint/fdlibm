#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

float __fmaxf(float x, float y)
{
	return (isgreaterequal (x, y) || isnan (y)) ? x : y;
}

__typeof(__fmaxf) fmaxf __attribute__((weak, alias("__fmaxf")));
