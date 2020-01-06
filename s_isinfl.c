/*
 * Written by J.T. Conklin <jtc@netbsd.org>.
 * Change for long double by Jakub Jelinek <jj@ultra.linux.cz>
 * Public domain.
 */

/*
 * isinfl(x) returns 1 if x is inf, -1 if x is -inf, else 0;
 * no branching!
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

int __isinfl(long double x)
{
	int32_t se, hx, lx;

	GET_LDOUBLE_WORDS(se, hx, lx, x);
	lx |= (hx & IC(0x7fffffff)) | ((se & 0x7fff) ^ 0x7fff);
	lx |= -lx;
	se &= 0x8000;
	return (int)(~(lx >> 31) & (1 - (se >> 14)));
}

__typeof(__isinfl) isinfl __attribute__((weak, alias("__isinfl")));

#endif
