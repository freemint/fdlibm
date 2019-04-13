#if defined(__GNUC__) && defined(__HAVE_68881__)

#ifndef FE_ROUNDING_MASK
#define FE_ROUNDING_MASK (FE_TONEAREST|FE_DOWNWARD|FE_UPWARD|FE_TOWARDZERO)
#endif

# define math_opt_barrier(x) \
  ({ __typeof (x) __x;					\
    __asm ("" : "=f" (__x) : "0" (x));			\
    __x; })
# define math_force_eval(x) \
  do {							\
    __typeof (x) __x = (x);				\
    __asm __volatile ("" : : "f" (__x));		\
  } while (0)

#ifndef __NO_MATH_INLINES

/* Get the m68881 condition codes, to quickly check multiple conditions.  */
#define __m81_test(__val, __fpsr) \
  __asm ("ftst%.x %1; fmove%.l %/fpsr,%0" : "=dm" (__fpsr) : "f" (__val))

/* Bit values returned by __m81_test.  */
#define __M81_COND_NAN  (1UL << 24)
#define __M81_COND_INF  (2UL << 24)
#define __M81_COND_ZERO (4UL << 24)
#define __M81_COND_NEG  (8UL << 24)

__MATH_INLINE double
__ieee754_sin (double x)
{
  double value;

  __asm ("fsin%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_sinf (float x)
{
  float value;

  __asm ("fsin%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_sinl (long double x)
{
  long double value;

  __asm ("fsin%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_sin

__MATH_INLINE double
__ieee754_cos (double x)
{
  double value;

  __asm ("fcos%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_cosf (float x)
{
  float value;

  __asm ("fcos%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_cosl (long double x)
{
  long double value;

  __asm ("fcos%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_cos

__MATH_INLINE void
__ieee754_sincos (double __x, double *__sin, double *__cos)
{
  __asm ("fsincos%.x %2,%0,%1"
	 : "=f" (*__cos), "=f" (*__sin)
	 : "f" (__x));
}

__MATH_INLINE void
__ieee754_sincosf (float __x, float *__sin, float *__cos)
{
  __asm ("fsincos%.x %2,%0,%1"
	 : "=f" (*__cos), "=f" (*__sin)
	 : "f" (__x));
}

__MATH_INLINE void
__ieee754_sincosl (long double __x, long double *__sin, long double *__cos)
{
  __asm ("fsincos%.x %2,%0,%1"
	 : "=f" (*__cos), "=f" (*__sin)
	 : "f" (__x));
}
#define __have_fpu_sincos

__MATH_INLINE double
__ieee754_tan (double x)
{
  double value;

  __asm ("ftan%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_tanf (float x)
{
  float value;

  __asm ("ftan%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_tanl (long double x)
{
  long double value;

  __asm ("ftan%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_tan

__MATH_INLINE double
__ieee754_asin (double x)
{
  double value;

  __asm ("fasin%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_asinf (float x)
{
  float value;

  __asm ("fasin%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_asinl (long double x)
{
  long double value;

  __asm ("fasin%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_asin

__MATH_INLINE double
__ieee754_acos (double x)
{
  double value;

  __asm ("facos%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_acosf (float x)
{
  float value;

  __asm ("facos%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_acosl (long double x)
{
  long double value;

  __asm ("facos%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_acos

__MATH_INLINE double
__ieee754_atan (double x)
{
  double value;

  __asm ("fatan%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_atanf (float x)
{
  float value;

  __asm ("fatan%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_atanl (long double x)
{
  long double value;

  __asm ("fatan%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_atan

__MATH_INLINE double
__ieee754_atan2 (double y, double x)
{
  double pi, pi_over_2;

  __asm ("fmovecr%.x #0,%0"		/* extended precision pi */
	 : "=f" (pi)
	 : /* no inputs */ );
  __asm ("fscale%.b #-1,%0"		/* no loss of accuracy */
	 : "=f" (pi_over_2)
	 : "0" (pi));
  if (x > 0)
    {
      if (y > 0)
	{
	  if (x > y)
	    return __ieee754_atan (y / x);
	  else
	    return pi_over_2 - __ieee754_atan (x / y);
	}
      else
	{
	  if (x > -y)
	    return __ieee754_atan (y / x);
	  else
	    return - pi_over_2 - __ieee754_atan (x / y);
	}
    }
  else
    {
      if (y < 0)
	{
	  if (-x > -y)
	    return - pi + __ieee754_atan (y / x);
	  else
	    return - pi_over_2 - __ieee754_atan (x / y);
	}
      else
	{
	  if (-x > y)
	    return pi + __ieee754_atan (y / x);
	  else if (y > 0)
	    return pi_over_2 - __ieee754_atan (x / y);
	  else
	    {
	      double value;

	      __set_errno(EDOM);
	      value = __builtin_nan("");	/* quiet NaN */
	      return value;
	    }
	}
    }
}

__MATH_INLINE float
__ieee754_atan2f (float y, float x)
{
  float pi, pi_over_2;

  __asm ("fmovecr%.x #0,%0"		/* extended precision pi */
	 : "=f" (pi)
	 : /* no inputs */ );
  __asm ("fscale%.b #-1,%0"		/* no loss of accuracy */
	 : "=f" (pi_over_2)
	 : "0" (pi));
  if (x > 0)
    {
      if (y > 0)
	{
	  if (x > y)
	    return __ieee754_atanf (y / x);
	  else
	    return pi_over_2 - __ieee754_atanf (x / y);
	}
      else
	{
	  if (x > -y)
	    return __ieee754_atanf (y / x);
	  else
	    return - pi_over_2 - __ieee754_atanf (x / y);
	}
    }
  else
    {
      if (y < 0)
	{
	  if (-x > -y)
	    return - pi + __ieee754_atanf (y / x);
	  else
	    return - pi_over_2 - __ieee754_atanf (x / y);
	}
      else
	{
	  if (-x > y)
	    return pi + __ieee754_atanf (y / x);
	  else if (y > 0)
	    return pi_over_2 - __ieee754_atanf (x / y);
	  else
	    {
	      float value;

	      __set_errno(EDOM);
	      value = __builtin_nanf("");
	      return value;
	    }
	}
    }
}

__MATH_INLINE long double
__ieee754_atan2l (long double y, long double x)
{
  long double pi, pi_over_2;

  __asm ("fmovecr%.x #0,%0"		/* extended precision pi */
	 : "=f" (pi)
	 : /* no inputs */ );
  __asm ("fscale%.b #-1,%0"		/* no loss of accuracy */
	 : "=f" (pi_over_2)
	 : "0" (pi));
  if (x > 0)
    {
      if (y > 0)
	{
	  if (x > y)
	    return __ieee754_atanl (y / x);
	  else
	    return pi_over_2 - __ieee754_atanl (x / y);
	}
      else
	{
	  if (x > -y)
	    return __ieee754_atanl (y / x);
	  else
	    return - pi_over_2 - __ieee754_atanl (x / y);
	}
    }
  else
    {
      if (y < 0)
	{
	  if (-x > -y)
	    return - pi + __ieee754_atanl (y / x);
	  else
	    return - pi_over_2 - __ieee754_atanl (x / y);
	}
      else
	{
	  if (-x > y)
	    return pi + __ieee754_atanl (y / x);
	  else if (y > 0)
	    return pi_over_2 - __ieee754_atanl (x / y);
	  else
	    {
	      long double value;

	      __set_errno(EDOM);
	      value = __builtin_nanl("");
	      return value;
	    }
	}
    }
}
#define __have_fpu_atan2

__MATH_INLINE double
__ieee754_sinh (double x)
{
  double value;

  __asm ("fsinh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_sinhf (float x)
{
  float value;

  __asm ("fsinh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_sinhl (long double x)
{
  long double value;

  __asm ("fsinh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_sinh

__MATH_INLINE double
__ieee754_cosh (double x)
{
  double value;

  __asm ("fcosh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_coshf (float x)
{
  float value;

  __asm ("fcosh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_coshl (long double x)
{
  long double value;

  __asm ("fcosh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_cosh

__MATH_INLINE double
__ieee754_tanh (double x)
{
  double value;

  __asm ("ftanh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_tanhf (float x)
{
  float value;

  __asm ("ftanh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_tanhl (long double x)
{
  long double value;

  __asm ("ftanh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_tanh

__MATH_INLINE double
__ieee754_atanh (double x)
{
  double value;

  __asm ("fatanh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_atanhf (float x)
{
  float value;

  __asm ("fatanh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_atanhl (long double x)
{
  long double value;

  __asm ("fatanh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_atanh

__MATH_INLINE double
__ieee754_exp (double x)
{
  double value;

  __asm ("fetox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_expf (float x)
{
  float value;

  __asm ("fetox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_expl (long double x)
{
  long double value;

  __asm ("fetox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_exp

__MATH_INLINE double
__ieee754_expm1 (double x)
{
  double value;

  __asm ("fetoxm1%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_expm1f (float x)
{
  float value;

  __asm ("fetoxm1%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_expm1l (long double x)
{
  long double value;

  __asm ("fetoxm1%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_expm1

__MATH_INLINE double
__ieee754_exp2 (double x)
{
  double value;

  __asm ("ftwotox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_exp2f (float x)
{
  float value;

  __asm ("ftwotox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_exp2l (long double x)
{
  long double value;

  __asm ("ftwotox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_exp2

__MATH_INLINE double
__ieee754_pow2 (double x)
{
  double value;

  __asm ("ftwotox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_pow2f (float x)
{
  float value;

  __asm ("ftwotox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_pow2l (long double x)
{
  long double value;

  __asm ("ftwotox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_pow2

__MATH_INLINE double
__ieee754_exp10 (double x)
{
  double value;

  __asm ("ftentox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_exp10f (float x)
{
  float value;

  __asm ("ftentox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_exp10l (long double x)
{
  long double value;

  __asm ("ftentox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_exp10

__MATH_INLINE double
__ieee754_pow10 (double x)
{
  double value;

  __asm ("ftentox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_pow10f (float x)
{
  float value;

  __asm ("ftentox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_pow10l (long double x)
{
  long double value;

  __asm ("ftentox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_pow10

__MATH_INLINE double
__ieee754_log (double x)
{
  double value;

  __asm ("flogn%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_logf (float x)
{
  float value;

  __asm ("flogn%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_logl (long double x)
{
  long double value;

  __asm ("flogn%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_log

__MATH_INLINE double
__ieee754_log1p (double x)
{
  double value;

  __asm ("flognp1%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_log1pf (float x)
{
  float value;

  __asm ("flognp1%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_log1pl (long double x)
{
  long double value;

  __asm ("flognp1%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_log1p

__MATH_INLINE double
__ieee754_log10 (double x)
{
  double value;

  __asm ("flog10%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_log10f (float x)
{
  float value;

  __asm ("flog10%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_log10l (long double x)
{
  long double value;

  __asm ("flog10%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_log10

__MATH_INLINE double
__ieee754_log2 (double x)
{
  double value;

  __asm ("flog2%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_log2f (float x)
{
  float value;

  __asm ("flog2%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_log2l (long double x)
{
  long double value;

  __asm ("flog2%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_log2

__MATH_INLINE double
__ieee754_sqrt (double x)
{
  double value;

  __asm ("fsqrt%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_sqrtf (float x)
{
  float value;

  __asm ("fsqrt%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_sqrtl (long double x)
{
  long double value;

  __asm ("fsqrt%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_sqrt

__MATH_INLINE double
__ieee754_fabs (double x)
{
  double value;

  __asm ("fabs%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_fabsf (float x)
{
  float value;

  __asm ("fabs%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_fabsl (long double x)
{
  long double value;

  __asm ("fabs%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define __have_fpu_fabs

__MATH_INLINE double
__ieee754_ceil (double x)
{
  long rounding_mode, round_up;
  double value;

  __asm __volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_up = (rounding_mode & ~FE_ROUNDING_MASK) | FE_UPWARD;
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_up));
  __asm __volatile ("fint%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}

__MATH_INLINE float
__ieee754_ceilf (float x)
{
  long rounding_mode, round_up;
  float value;

  __asm __volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_up = (rounding_mode & ~FE_ROUNDING_MASK) | FE_UPWARD;
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_up));
  __asm __volatile ("fint%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}

__MATH_INLINE long double
__ieee754_ceill (long double x)
{
  long rounding_mode, round_up;
  long double value;

  __asm __volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_up = (rounding_mode & ~FE_ROUNDING_MASK) | FE_UPWARD;
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_up));
  __asm __volatile ("fint%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}
#define __have_fpu_ceil

__MATH_INLINE double
__ieee754_floor (double x)
{
  long rounding_mode, round_down;
  double value;

  __asm __volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_down = (rounding_mode & ~FE_ROUNDING_MASK) | FE_DOWNWARD;;
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_down));
  __asm __volatile ("fint%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}

__MATH_INLINE float
__ieee754_floorf (float x)
{
  long rounding_mode, round_down;
  float value;

  __asm __volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_down = (rounding_mode & ~FE_ROUNDING_MASK) | FE_DOWNWARD;
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_down));
  __asm __volatile ("fint%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}

__MATH_INLINE long double
__ieee754_floorl (long double x)
{
  long rounding_mode, round_down;
  long double value;

  __asm __volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_down = (rounding_mode & ~FE_ROUNDING_MASK) | FE_DOWNWARD;
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_down));
  __asm __volatile ("fint%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}
#define __have_fpu_floor

__MATH_INLINE double
__ieee754_trunc (double x)
{
  double value;

  __asm __volatile ("fintrz%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_truncf (float x)
{
  float value;

  __asm __volatile ("fintrz%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_truncl (long double x)
{
  long double value;

  __asm __volatile ("fintrz%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  return value;
}
#define __have_fpu_trunc

__MATH_INLINE double
__ieee754_rint (double x)
{
  double value;
  __asm __volatile ("fint%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_rintf (float x)
{
  float value;
  __asm __volatile ("fint%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_rintl (long double x)
{
  long double value;
  __asm __volatile ("fint%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  return value;
}
#define __have_fpu_rint

__MATH_INLINE long int
__ieee754_lrint (double x)
{
  long int value;
  __asm __volatile ("fmove.l %1,%0"
		  : "=dm" (value)
		  : "f" (x));
  return value;
}

__MATH_INLINE long int
__ieee754_lrintf (float x)
{
  long int value;
  __asm __volatile ("fmove.l %1,%0"
		  : "=dm" (value)
		  : "f" (x));
  return value;
}

__MATH_INLINE long int
__ieee754_lrintl (long double x)
{
  long int value;
  __asm __volatile ("fmove.l %1,%0"
		  : "=dm" (value)
		  : "f" (x));
  return value;
}
#define __have_fpu_lrint

__MATH_INLINE double
__ieee754_round (double x)
{
  long rounding_mode, round_nearest;
  double value;

  __asm __volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_nearest = (rounding_mode & ~FE_ROUNDING_MASK) | FE_TONEAREST;
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_nearest));
  __asm __volatile ("fint%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}

__MATH_INLINE float
__ieee754_roundf (float x)
{
  long rounding_mode, round_nearest;
  float value;

  __asm __volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_nearest = (rounding_mode & ~FE_ROUNDING_MASK) | FE_TONEAREST;
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_nearest));
  __asm __volatile ("fint%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}

__MATH_INLINE long double
__ieee754_roundl (long double x)
{
  long rounding_mode, round_nearest;
  long double value;

  __asm __volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_nearest = (rounding_mode & ~FE_ROUNDING_MASK) | FE_TONEAREST;
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_nearest));
  __asm __volatile ("fint%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}
#define __have_fpu_round

__MATH_INLINE long int
__ieee754_lround(double x)
{
  long rounding_mode, round_nearest;
  long value;

  __asm __volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_nearest = rounding_mode & ~0x30;
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_nearest));
  __asm __volatile ("fint%.x %2,%1\n"
  			"\tfmove%.l %1,%0"
		  : "=dm" (value), "=f"(x)
		  : "1" (x));
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}

__MATH_INLINE long int __ieee754_lroundf(float x)
{
  long rounding_mode, round_nearest;
  long value;

  __asm __volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_nearest = rounding_mode & ~0x30;
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_nearest));
  __asm __volatile ("fint%.x %2,%1\n"
  			"\tfmove%.l %1,%0"
		  : "=dm" (value), "=f"(x)
		  : "1" (x));
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}

__MATH_INLINE long int __ieee754_lroundl(long double x)
{
  long rounding_mode, round_nearest;
  long value;

  __asm __volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_nearest = rounding_mode & ~0x30;
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_nearest));
  __asm __volatile ("fint%.x %2,%1\n"
  			"\tfmove%.l %1,%0"
		  : "=dm" (value), "=f"(x)
		  : "1" (x));
  __asm __volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}
#define __have_fpu_lround

__MATH_INLINE double
__ieee754_fmod (double x, double y)
{
  double value;

  __asm ("fmod%.x %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "f" (y));
  return value;
}

__MATH_INLINE float
__ieee754_fmodf (float x, float y)
{
  float value;

  __asm ("fmod%.x %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "f" (y));
  return value;
}

__MATH_INLINE long double
__ieee754_fmodl (long double x, long double y)
{
  long double value;

  __asm ("fmod%.x %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "f" (y));
  return value;
}
#define __have_fpu_fmod

__MATH_INLINE double
__ieee754_drem (double x, double y)
{
  double value;

  __asm ("frem%.x %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "f" (y));
  return value;
}

__MATH_INLINE float
__ieee754_dremf (float x, float y)
{
  float value;

  __asm ("frem%.x %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "f" (y));
  return value;
}

__MATH_INLINE long double
__ieee754_dreml (long double x, long double y)
{
  long double value;

  __asm ("frem%.x %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "f" (y));
  return value;
}
#define __have_fpu_drem

__MATH_INLINE double
__ieee754_remainder (double x, double y)
{
  double value;

  __asm ("frem%.x %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "f" (y));
  return value;
}

__MATH_INLINE float
__ieee754_remainderf (float x, float y)
{
  float value;

  __asm ("frem%.x %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "f" (y));
  return value;
}

__MATH_INLINE long double
__ieee754_remainderl (long double x, long double y)
{
  long double value;

  __asm ("frem%.x %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "f" (y));
  return value;
}
#define __have_fpu_remainder


__MATH_INLINE double
__ieee754_remquo(double x, double y, int *quo)
{
  double result;
  int cquo;
  unsigned long fpsr;

  __asm ("frem%.x %2,%0\n\tfmove%.l %/fpsr,%1"
	 : "=f" (result), "=dm" (fpsr) : "f" (y), "0" (x));
  cquo = (fpsr >> 16) & 0x7f;
  if (fpsr & (1L << 23))
    cquo = -cquo;
  *quo = cquo;
  return result;
}

__MATH_INLINE float
__ieee754_remquof(float x, float y, int *quo)
{
  float result;
  int cquo;
  unsigned long fpsr;

  __asm ("frem%.x %2,%0\n\tfmove%.l %/fpsr,%1"
	 : "=f" (result), "=dm" (fpsr) : "f" (y), "0" (x));
  cquo = (fpsr >> 16) & 0x7f;
  if (fpsr & (1L << 23))
    cquo = -cquo;
  *quo = cquo;
  return result;
}

__MATH_INLINE long double
__ieee754_remquol(long double x, long double y, int *quo)
{
  long double result;
  int cquo;
  unsigned long fpsr;

  __asm ("frem%.x %2,%0\n\tfmove%.l %/fpsr,%1"
	 : "=f" (result), "=dm" (fpsr) : "f" (y), "0" (x));
  cquo = (fpsr >> 16) & 0x7f;
  if (fpsr & (1L << 23))
    cquo = -cquo;
  *quo = cquo;
  return result;
}
#define __have_fpu_remquo


__MATH_INLINE double
__ieee754_scalbn (double x, int n)
{
  double value;
  long exp = (long)(n);

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (exp));
  return value;
}

__MATH_INLINE float
__ieee754_scalbnf (float x, int n)
{
  float value;
  long exp = (long)(n);

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (exp));
  return value;
}

__MATH_INLINE long double
__ieee754_scalbnl (long double x, int n)
{
  long double value;
  long exp = (long)(n);

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (exp));
  return value;
}
#define __have_fpu_scalbn

__MATH_INLINE double
__ieee754_scalbln (double x, long n)
{
  double value;
  long exp = (long)(n);

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (exp));
  return value;
}

__MATH_INLINE float
__ieee754_scalblnf (float x, long n)
{
  float value;
  long exp = (long)(n);

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (exp));
  return value;
}

__MATH_INLINE long double
__ieee754_scalblnl (long double x, long n)
{
  long double value;
  long exp = (long)(n);

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (exp));
  return value;
}
#define __have_fpu_scalbln

__MATH_INLINE double
__ieee754_logb (double x)
{
  double exponent;

  __asm ("fgetexp%.x %1,%0"
	 : "=f" (exponent)
	 : "f" (x));
  return exponent;
}

__MATH_INLINE float
__ieee754_logbf (float x)
{
  float exponent;

  __asm ("fgetexp%.x %1,%0"
	 : "=f" (exponent)
	 : "f" (x));
  return exponent;
}

__MATH_INLINE long double
__ieee754_logbl (long double x)
{
  long double exponent;

  __asm ("fgetexp%.x %1,%0"
	 : "=f" (exponent)
	 : "f" (x));
  return exponent;
}
#define __have_fpu_logb

__MATH_INLINE int
__ieee754_ilogb (double x)
{
  double exponent;
  unsigned long x_cond;

  __m81_test (x, x_cond);
  /* We must return consistent values for zero and NaN.  */
  if (x_cond & __M81_COND_ZERO)
    return FP_ILOGB0;
  if (x_cond & (__M81_COND_NAN | __M81_COND_INF))
    return FP_ILOGBNAN;

  __asm ("fgetexp%.x %1,%0"
	 : "=f" (exponent)
	 : "f" (x));
  return (int)exponent;
}

__MATH_INLINE int
__ieee754_ilogbf (float x)
{
  float exponent;
  unsigned long x_cond;

  __m81_test (x, x_cond);
  /* We must return consistent values for zero and NaN.  */
  if (x_cond & __M81_COND_ZERO)
    return FP_ILOGB0;
  if (x_cond & (__M81_COND_NAN | __M81_COND_INF))
    return FP_ILOGBNAN;

  __asm ("fgetexp%.x %1,%0"
	 : "=f" (exponent)
	 : "f" (x));
  return (int)exponent;
}

__MATH_INLINE int
__ieee754_ilogbl (long double x)
{
  long double exponent;
  unsigned long x_cond;

  __m81_test (x, x_cond);
  /* We must return consistent values for zero and NaN.  */
  if (x_cond & __M81_COND_ZERO)
    return FP_ILOGB0;
  if (x_cond & (__M81_COND_NAN | __M81_COND_INF))
    return FP_ILOGBNAN;

  __asm ("fgetexp%.x %1,%0"
	 : "=f" (exponent)
	 : "f" (x));
  return (int)exponent;
}
#define __have_fpu_ilogb


__MATH_INLINE double
__ieee754_ldexp (double x, int n)
{
  double value;

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (n));
  return value;
}

__MATH_INLINE float
__ieee754_ldexpf (float x, int n)
{
  float value;

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (n));
  return value;
}

__MATH_INLINE long double
__ieee754_ldexpl (long double x, int n)
{
  long double value;

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (n));
  return value;
}
#define __have_fpu_ldexp

__MATH_INLINE double
__ieee754_frexp (double value, int *expptr)
{
  double float_exponent, mantissa;
  int int_exponent;
  unsigned long fpsr;

  __m81_test(value, fpsr);
  if (fpsr & (__M81_COND_NAN|__M81_COND_INF|__M81_COND_ZERO))
    {
      /* Not finite or zero.  */
      *expptr = 0;
      return value;
    }
  __asm ("fgetexp%.x %1,%0"
	 : "=f" (float_exponent)	/* integer-valued float */
	 : "f" (value));
  int_exponent = (int) float_exponent + 1;
  *expptr = int_exponent;
  __asm ("fscale%.l %2, %0"
	 : "=f" (mantissa)
	 : "0" (value), "dmi" (-int_exponent));
  return mantissa;
}

__MATH_INLINE float
__ieee754_frexpf (float value, int *expptr)
{
  float float_exponent, mantissa;
  int int_exponent;
  unsigned long fpsr;

  __m81_test(value, fpsr);
  if (fpsr & (__M81_COND_NAN|__M81_COND_INF|__M81_COND_ZERO))
    {
      /* Not finite or zero.  */
      *expptr = 0;
      return value;
    }
  __asm ("fgetexp%.x %1,%0"
	 : "=f" (float_exponent)	/* integer-valued float */
	 : "f" (value));
  int_exponent = (int) float_exponent + 1;
  *expptr = int_exponent;
  __asm ("fscale%.l %2, %0"
	 : "=f" (mantissa)
	 : "0" (value), "dmi" (-int_exponent));
  return mantissa;
}

__MATH_INLINE long double
__ieee754_frexpl (long double value, int *expptr)
{
  long double float_exponent, mantissa;
  int int_exponent;
  unsigned long fpsr;

  __m81_test(value, fpsr);
  if (fpsr & (__M81_COND_NAN|__M81_COND_INF|__M81_COND_ZERO))
    {
      /* Not finite or zero.  */
      *expptr = 0;
      return value;
    }
  __asm ("fgetexp%.x %1,%0"
	 : "=f" (float_exponent)	/* integer-valued float */
	 : "f" (value));
  int_exponent = (int) float_exponent + 1;
  *expptr = int_exponent;
  /* Unnormalized numbers must be handled specially, otherwise fscale
     results in overflow.  */
  if (int_exponent <= -16384)
    {
      value *= 0x1p16383L;
      int_exponent += 16383;
    }
  else if (int_exponent >= 16384)
    {
      value *= 0x1p-16383L;
      int_exponent -= 16383;
    }

  __asm ("fscale%.l %2, %0"
	 : "=f" (mantissa)
	 : "0" (value), "dmi" (-int_exponent));
  return mantissa;
}
#define __have_fpu_frexp

__MATH_INLINE double
__ieee754_significand(double x)
{
  double mantissa;

  __asm ("fgetman%.x %1,%0"
	 : "=f" (mantissa)		/* 1.0 <= mantissa < 2.0 */
	 : "f" (x));
  return mantissa;
}

__MATH_INLINE float
__ieee754_significandf(float x)
{
  float mantissa;

  __asm ("fgetman%.x %1,%0"
	 : "=f" (mantissa)		/* 1.0 <= mantissa < 2.0 */
	 : "f" (x));
  return mantissa;
}

__MATH_INLINE long double
__ieee754_significandl(long double x)
{
  long double mantissa;

  __asm ("fgetman%.x %1,%0"
	 : "=f" (mantissa)		/* 1.0 <= mantissa < 2.0 */
	 : "f" (x));
  return mantissa;
}
#define __have_fpu_significand

__MATH_INLINE double
__ieee754_modf (double x, double *ip)
{
  double temp, result;
  unsigned long x_cond;

  temp = __ieee754_trunc(x);
  if (ip)
    *ip = temp;
  __m81_test (x, x_cond);
  if (x_cond & __M81_COND_INF)
    {
      result = 0;
      if (x_cond & __M81_COND_NEG)
	result = -result;
    }
  else if (x_cond & __M81_COND_ZERO)
    result = x;
  else
    result = x - temp;
  return result;
}

__MATH_INLINE float
__ieee754_modff (float x, float *ip)
{
  float temp, result;
  unsigned long x_cond;

  temp = __ieee754_truncf(x);
  if (ip)
    *ip = temp;
  __m81_test (x, x_cond);
  if (x_cond & __M81_COND_INF)
    {
      result = 0;
      if (x_cond & __M81_COND_NEG)
	result = -result;
    }
  else if (x_cond & __M81_COND_ZERO)
    result = x;
  else
    result = x - temp;
  return result;
}

__MATH_INLINE long double
__ieee754_modfl (long double x, long double *ip)
{
  long double temp, result;
  unsigned long x_cond;

  temp = __ieee754_truncl(x);
  if (ip)
    *ip = temp;
  __m81_test (x, x_cond);
  if (x_cond & __M81_COND_INF)
    {
      result = 0;
      if (x_cond & __M81_COND_NEG)
	result = -result;
    }
  else if (x_cond & __M81_COND_ZERO)
    result = x;
  else
    result = x - temp;
  return result;
}
#define __have_fpu_modf

/* ISO C99 defines some macros to perform unordered comparisons.  The
   m68k FPU supports this with special opcodes and we should use them.
   These must not be inline functions since we have to be able to handle
   all floating-point types.  */
#  undef isgreater
#  undef isgreaterequal
#  undef isless
#  undef islessequal
#  undef islessgreater
#  undef isunordered
#  define isgreater(x, y)					\
   __extension__					\
   ({ signed char __result;					\
      __asm__ ("fcmp%.x %2,%1; fsogt %0"		\
	       : "=dm" (__result) : "f" (x), "f" (y));	\
      -__result; })

#  define isgreaterequal(x, y)				\
   __extension__					\
   ({ signed char __result;					\
      __asm__ ("fcmp%.x %2,%1; fsoge %0"		\
	       : "=dm" (__result) : "f" (x), "f" (y));	\
      -__result; })

#  define isless(x, y)					\
   __extension__					\
   ({ signed char __result;					\
      __asm__ ("fcmp%.x %2,%1; fsolt %0"		\
	       : "=dm" (__result) : "f" (x), "f" (y));	\
      -__result; })

#  define islessequal(x, y)				\
   __extension__					\
   ({ signed char __result;					\
      __asm__ ("fcmp%.x %2,%1; fsole %0"		\
	       : "=dm" (__result) : "f" (x), "f" (y));	\
      -__result; })

#  define islessgreater(x, y)				\
   __extension__					\
   ({ signed char __result;					\
      __asm__ ("fcmp%.x %2,%1; fsogl %0"		\
	       : "=dm" (__result) : "f" (x), "f" (y));	\
      -__result; })

#  define isunordered(x, y)				\
   __extension__					\
   ({ signed char __result;					\
      __asm__ ("fcmp%.x %2,%1; fsun %0"			\
	       : "=dm" (__result) : "f" (x), "f" (y));	\
      -__result; })

#endif /* __NO_MATH_INLINES */

#endif
