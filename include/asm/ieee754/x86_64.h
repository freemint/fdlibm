#include "asm/ieee754/x86l.h"

#ifndef __NO_MATH_INLINES

__MATH_INLINE double
__ieee754_fabs(double x)
{
	return __builtin_fabs(x);
}

__MATH_INLINE float
__ieee754_fabsf(float x)
{
	return __builtin_fabsf(x);
}

__MATH_INLINE double
__ieee754_sin(double x)
{
	return (double)__ieee754_sinl(x);
}

__MATH_INLINE float
__ieee754_sinf(float x)
{
	return (float)__ieee754_sinl(x);
}

__MATH_INLINE double
__ieee754_cos(double x)
{
	return (double)__ieee754_cosl(x);
}

__MATH_INLINE float
__ieee754_cosf(float x)
{
	return (float)__ieee754_cosl(x);
}

__MATH_INLINE void
__ieee754_sincos(double x, double *sinx, double *cosx)
{
	long double lsinx, lcosx;
	__ieee754_sincosl(x, &lsinx, &lcosx);
	*sinx = (double)lsinx;
	*cosx = (double)lcosx;
}

__MATH_INLINE void
__ieee754_sincosf(float x, float *sinx, float *cosx)
{
	long double lsinx, lcosx;
	__ieee754_sincosl(x, &lsinx, &lcosx);
	*sinx = (float)lsinx;
	*cosx = (float)lcosx;
}

__MATH_INLINE double
__ieee754_tan(double x)
{
	return (double)__ieee754_tanl(x);
}

__MATH_INLINE float
__ieee754_tanf(float x)
{
	return (float)__ieee754_tanl(x);
}

__MATH_INLINE double
__ieee754_acos (double x)
{
	return (double) __ieee754_acosl(x);
}

__MATH_INLINE float
__ieee754_acosf (float x)
{
	return (float) __ieee754_acosl(x);
}

__MATH_INLINE double
__ieee754_asin (double x)
{
	return (double) __ieee754_asinl(x);
}

__MATH_INLINE float
__ieee754_asinf (float x)
{
	return (float) __ieee754_asinl(x);
}

__MATH_INLINE double
__ieee754_atan (double x)
{
	return (double) __ieee754_atanl(x);
}

__MATH_INLINE float
__ieee754_atanf (float x)
{
	return (float) __ieee754_atanl(x);
}

__MATH_INLINE double
__ieee754_atan2 (double y, double x)
{
	return (double) __ieee754_atan2l(y, x);
}

__MATH_INLINE float
__ieee754_atan2f (float y, float x)
{
	return (float) __ieee754_atan2l(y, x);
}

__MATH_INLINE double
__ieee754_sqrt (double x)
{
  double res;

  asm ("sqrtsd %1, %0" : "=x" (res) : "xm" (x));

  return res;
}

__MATH_INLINE float
__ieee754_sqrtf (float x)
{
  float res;

  asm ("sqrtss %1, %0" : "=x" (res) : "xm" (x));

  return res;
}

__MATH_INLINE double
__ieee754_fmod (double x, double y)
{
	return (double) __ieee754_fmodl(x, y);
}

__MATH_INLINE float
__ieee754_fmodf (float x, float y)
{
	return (float) __ieee754_fmodl(x, y);
}

__MATH_INLINE double
__ieee754_remainder (double x, double y)
{
	return (double) __ieee754_remainderl(x, y);
}


__MATH_INLINE float
__ieee754_remainderf (float x, float y)
{
	return (float) __ieee754_remainderl(x, y);
}

#if 0

__MATH_INLINE double
__ieee754_remquo (double x, double y, int *quo)
{
	return (double) __ieee754_remquol(x, y, quo);
}

__MATH_INLINE float
__ieee754_remquof (float x, float y, int *quo)
{
	return (float) __ieee754_remquol(x, y, quo);
}

#endif

__MATH_INLINE double
__ieee754_ceil (double x)
{
	return (double) __ieee754_ceill(x);
}

__MATH_INLINE float
__ieee754_ceilf (float x)
{
	return (float) __ieee754_ceill(x);
}

__MATH_INLINE double
__ieee754_floor (double x)
{
	return (double) __ieee754_floorl(x);
}

__MATH_INLINE float
__ieee754_floorf (float x)
{
	return (float) __ieee754_floorl(x);
}

__MATH_INLINE double
__ieee754_trunc (double x)
{
	return (double) __ieee754_truncl(x);
}

__MATH_INLINE float
__ieee754_truncf (float x)
{
	return (float) __ieee754_truncl(x);
}

__MATH_INLINE double
__ieee754_rint (double x)
{
	return (double) __ieee754_rintl(x);
}

__MATH_INLINE float
__ieee754_rintf (float x)
{
	return (float) __ieee754_rintl(x);
}

__MATH_INLINE long
__ieee754_lrint (double x)
{
	return __ieee754_lrintl(x);
}

__MATH_INLINE long
__ieee754_lrintf (float x)
{
	return __ieee754_lrintl(x);
}

__extension__ __MATH_INLINE long long
__ieee754_llrint (double x)
{
	return __ieee754_llrintl(x);
}

__extension__ __MATH_INLINE long long
__ieee754_llrintf (float x)
{
	return __ieee754_llrintl(x);
}

#if 0 /* frndint rounds in wrong direction */

__MATH_INLINE double
__ieee754_round (double x)
{
	return (double) __ieee754_roundl(x);
}

__MATH_INLINE float
__ieee754_roundf (float x)
{
	return (float) __ieee754_roundl(x);
}

#endif

__MATH_INLINE double
__ieee754_scalbn (double x, int n)
{
	return (double) __ieee754_scalbnl(x, n);
}

__MATH_INLINE float
__ieee754_scalbnf (float x, int n)
{
	return (float) __ieee754_scalbnl(x, n);
}

__MATH_INLINE double
__ieee754_scalbln (double x, long n)
{
	return (double) __ieee754_scalblnl(x, n);
}

__MATH_INLINE float
__ieee754_scalblnf (float x, long n)
{
	return (float) __ieee754_scalblnl(x, n);
}

__MATH_INLINE double
__ieee754_logb (double x)
{
	return (double) __ieee754_logbl(x);
}

__MATH_INLINE float
__ieee754_logbf (float x)
{
	return (float) __ieee754_logbl(x);
}

__MATH_INLINE double
__ieee754_expm1 (double x)
{
	return (double) __ieee754_expm1l(x);
}

__MATH_INLINE float
__ieee754_expm1f (float x)
{
	return (float) __ieee754_expm1l(x);
}

__MATH_INLINE double
__ieee754_exp2 (double x)
{
	return (double) __ieee754_exp2l(x);
}

__MATH_INLINE float
__ieee754_exp2f (float x)
{
	return (float) __ieee754_exp2l(x);
}

__MATH_INLINE double
__ieee754_log (double x)
{
	return (double) __ieee754_logl(x);
}

__MATH_INLINE float
__ieee754_logf (float x)
{
	return (float) __ieee754_logl(x);
}

__MATH_INLINE double
__ieee754_log1p (double x)
{
	return (double) __ieee754_log1pl(x);
}

__MATH_INLINE float
__ieee754_log1pf (float x)
{
	return (float) __ieee754_log1pl(x);
}

__MATH_INLINE double
__ieee754_log2 (double x)
{
	return (double) __ieee754_log2l(x);
}

__MATH_INLINE float
__ieee754_log2f (float x)
{
	return (float) __ieee754_log2l(x);
}

__MATH_INLINE double
__ieee754_log10 (double x)
{
	return (double) __ieee754_log10l(x);
}

__MATH_INLINE float
__ieee754_log10f (float x)
{
	return (float) __ieee754_log10l(x);
}

__MATH_INLINE double
__ieee754_exp (double x)
{
	return (double) __ieee754_expl(x);
}

__MATH_INLINE float
__ieee754_expf (float x)
{
	return (float) __ieee754_expl(x);
}

__MATH_INLINE double
__ieee754_modf (double x, double *ip)
{
	long double temp, result;
	result = __ieee754_modfl(x, &temp);
	if (ip)
		*ip = (double) temp;
	return (double) result;
}

__MATH_INLINE float
__ieee754_modff (float x, float *ip)
{
	long double temp, result;
	result = __ieee754_modfl(x, &temp);
	if (ip)
		*ip = (float) temp;
	return (float) result;
}

#endif /* __NO_MATH_INLINES */
