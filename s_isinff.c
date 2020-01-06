/*
 * Written by J.T. Conklin <jtc@netbsd.org>.
 * Public domain.
 */

/*
 * isinff(x) returns 1 if x is inf, -1 if x is -inf, else 0;
 * no branching!
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

int __isinff(float x)
{
	int32_t ix, t;

	GET_FLOAT_WORD(ix, x);
	t = ix & IC(0x7fffffff);
	t ^= IC(0x7f800000);
	t |= -t;
	return (int)(~(t >> 31) & (ix >> 30));
}

__typeof(__isinff) isinff __attribute__((weak, alias("__isinff")));
