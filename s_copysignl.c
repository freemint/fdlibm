#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

long double __copysignl(long double x, long double y)
{
	uint32_t es1, es2;
	GET_LDOUBLE_EXP(es1, x);
	GET_LDOUBLE_EXP(es2, y);
	SET_LDOUBLE_EXP(x, (es1 & 0x7fff) | (es2 & 0x8000));
    return x;
}

__typeof(__copysignl) copysignl __attribute__((weak, alias("__copysignl")));

#endif
