/*
 * finitef(x) returns 1 is x is finite, else 0;
 * no branching!
 */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

int __finitef(float x)
{
	uint32_t ix;

	GET_FLOAT_WORD(ix, x);
	return (ix & UC(0x7fffffff)) < UC(0x7f800000);
}

__typeof(__finitef) finitef __attribute__((weak, alias("__finitef")));
