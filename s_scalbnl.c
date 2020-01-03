#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __have_fpu_scalbn

long double __ieee754_scalbnl(long double x, int n)
{
	int32_t k, es, hx, lx;

	static const long double two64 = 1.8446744073709551616e19L;
	static const long double twom64 = 5.421010862427522170037e-20L;
	static const long double hugeval = 1.0e+4900L;
	static const long double tiny = 1.0e-4900L;

	GET_LDOUBLE_WORDS(es, hx, lx, x);
	k = es & IEEE854_LONG_DOUBLE_MAXEXP;					/* extract exponent */
	if (k == 0)
	{									/* 0 or subnormal x */
		if ((lx | (hx & UC(0x7fffffff))) == 0)
			return x;					/* +-0 */
		x *= two64;
		GET_LDOUBLE_EXP(hx, x);
		k = (hx & IEEE854_LONG_DOUBLE_MAXEXP) - 64;
	}
	if (k == IEEE854_LONG_DOUBLE_MAXEXP)
		return x + x;					/* NaN or Inf */
	if ((int32_t)n < IC(-30000))
		return tiny * copysignl(tiny, x);
	if ((int32_t)n > IC(30000) || (k + n) > 0x7ffe)
		return hugeval * copysignl(hugeval, x);	/* overflow  */
	/* Now k and n are bounded we know that k = k+n does not
	   overflow.  */
	k = k + n;
	if (k > 0)		/* normal result */
	{
		SET_LDOUBLE_EXP(x, (es & 0x8000) | k);
		return x;
	}
	if (k <= -64)
		return tiny * copysignl(tiny, x);	/*underflow */
	k += 64;							/* subnormal result */
	SET_LDOUBLE_EXP(x, (es & 0x8000) | k);
	return x * twom64;
}

#endif

long double __scalbnl(long double x, int n)
{
	return __ieee754_scalbnl(x, n);
}

__typeof(__scalbnl) scalbnl __attribute__((weak, alias("__scalbnl")));

#if INT_MAX == LONG_MAX
long double __scalblnl(long double x, long n) __attribute__((alias("__scalbnl")));
long double scalblnl(long double x, long n) __attribute__((weak, alias("__scalblnl")));
#ifndef __have_fpu_scalbn
long double __ieee754_scalblnl(long double x, long n) __attribute__((alias("__ieee754_scalbnl")));
#endif
#endif

#endif
