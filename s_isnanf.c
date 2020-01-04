#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

int __isnanf(float x)
{
	int32_t ix;

	GET_FLOAT_WORD(ix, x);
	ix &= IC(0x7fffffff);
	return ix > IC(0x7f800000);
}

__typeof(__isnanf) isnanf __attribute__((weak, alias("__isnanf")));
