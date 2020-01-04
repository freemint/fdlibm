#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

float __copysignf(float x, float y)
{
	uint32_t ix, iy;

	GET_FLOAT_WORD(ix, x);
	GET_FLOAT_WORD(iy, y);
	SET_FLOAT_WORD(x, (ix & UC(0x7fffffff)) | (iy & UC(0x80000000)));
	return x;
}

__typeof(__copysignf) copysignf __attribute__((weak, alias("__copysignf")));
