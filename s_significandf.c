#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

float __significandf(float x)
{
	uint32_t hx, ix;

	GET_FLOAT_WORD(hx, x);
	ix = hx & UC(0x7f800000);
	if (ix != 0 && ix != UC(0x7f800000))
	{
		SET_FLOAT_WORD(x, (hx & UC(0x807fffff)) | UC(0x3f800000));
	}
	return x;
}

__typeof(__significandf) significandf __attribute__((weak, alias("__significandf")));
