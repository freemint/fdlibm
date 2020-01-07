#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

/*
 * we cannot replace that single function, because it is
 * not compiled separately in libgcc.a and leads
 * to duplicate definition errors :(
 */
#if 0

#ifndef __NO_LONG_DOUBLE_MATH

#define EXPDBITS	11
#define MANTXMASK IC(0x7fffffff)

double __truncxfdf2(long double ld);

/* convert long double to double */
double __truncxfdf2(long double ld)
{
	int32_t exp;
	ieee_double_shape_type dl;
	ieee_long_double_shape_type ldl;

	ldl.value = ld;

	exp = ldl.parts.sign_exponent & IEEE854_LONG_DOUBLE_MAXEXP;
	dl.parts.msw = (int32_t)(ldl.parts.sign_exponent & 0x8000) << 16;
	if (exp == 0 &&
		(ldl.parts.msw & MANTXMASK) == 0 &&
		ldl.parts.lsw == 0)
	{
		dl.parts.lsw = 0;
	} else
	{
		if (exp == IEEE854_LONG_DOUBLE_MAXEXP)
		{
			exp = IEEE754_DOUBLE_MAXEXP;
		} else
		{
			exp = exp - IEEE854_LONG_DOUBLE_BIAS + IEEE754_DOUBLE_BIAS;
			/* ??? quick and dirty: keep `exp' sane */
			if (exp >= IEEE754_DOUBLE_MAXEXP)
				exp = IEEE754_DOUBLE_MAXEXP - 1;
		}
		dl.parts.msw |= exp << (32 - (EXPDBITS + 1));
		/* +1-1: add one for sign bit, but take one off for explicit-integer-bit */
		dl.parts.msw |= (ldl.parts.msw & MANTXMASK) >> (EXPDBITS + 1 - 1);
		dl.parts.lsw = (ldl.parts.msw & MANTXMASK) << (32 - (EXPDBITS + 1 - 1));
		dl.parts.lsw |= ldl.parts.lsw >> (EXPDBITS + 1 - 1);
	}
		
	return dl.value;
}

#endif

#endif
