#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

int __isnanl(long double x)
{
	int32_t se, hx, lx;

	GET_LDOUBLE_WORDS(se, hx, lx, x);
	se = (se & 0x7fff) << 1;
	lx |= hx & IC(0x7fffffff);
	se |= (uint32_t) (lx | (-lx)) >> 31;
	se = 0xfffe - se;
	return (int) (((uint32_t) (se)) >> 31);
}

__typeof(__isnanl) isnanl __attribute__((weak, alias("__isnanl")));

#endif
