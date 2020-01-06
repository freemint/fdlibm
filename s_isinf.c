/*
 * Written by J.T. Conklin <jtc@netbsd.org>.
 * Changed to return -1 for -Inf by Ulrich Drepper <drepper@cygnus.com>.
 * Public domain.
 */

/*
 * isinf(x) returns 1 is x is inf, -1 if x is -inf, else 0;
 * no branching!
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

int __isinf(double x)
{
	int32_t hx, lx;

	GET_DOUBLE_WORDS(hx, lx, x);
	lx |= (hx & IC(0x7fffffff)) ^ IC(0x7ff00000);
	lx |= -lx;
	return (int)(~(lx >> 31) & (hx >> 30));
}

__typeof(__isinf) isinf __attribute__((weak, alias("__isinf")));
#ifdef __NO_LONG_DOUBLE_MATH
__typeof(__isinfl) __isinfl __attribute__((alias("__isinf")));
__typeof(__isinfl) isinfl __attribute__((weak, alias("__isinf")));
#endif
