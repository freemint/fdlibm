#if defined(__GNUC__) && defined(__mcffpu__)

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

__MATH_INLINE double
__ieee754_atan2 (double y, double x)
{
  double pi, pi_over_2;

#ifdef M_PI
  pi = M_PI;
  pi_over_2 = M_PI_2;
#else
  pi = 3.14159265358979323846;
  pi_over_2 = 1.57079632679489661923;
#endif
  if (x > 0)
    {
      if (y > 0)
	{
	  if (x > y)
	    return atan (y / x);
	  else
	    return pi_over_2 - atan (x / y);
	}
      else
	{
	  if (x > -y)
	    return atan (y / x);
	  else
	    return - pi_over_2 - atan (x / y);
	}
    }
  else
    {
      if (y < 0)
	{
	  if (-x > -y)
	    return - pi + atan (y / x);
	  else
	    return - pi_over_2 - atan (x / y);
	}
      else
	{
	  if (-x > y)
	    return pi + atan (y / x);
	  else if (y > 0)
	    return pi_over_2 - atan (x / y);
	  else
	    {
	      double value;

	      __set_errno(EDOM);
	      value = __builtin_nan("");
	      return value;
	    }
	}
    }
}

__MATH_INLINE float
__ieee754_atan2f (float y, float x)
{
  float pi, pi_over_2;

#ifdef M_PI
  pi = M_PI;
  pi_over_2 = M_PI_2;
#else
  pi = 3.14159265358979323846;
  pi_over_2 = 1.57079632679489661923;
#endif
  if (x > 0)
    {
      if (y > 0)
	{
	  if (x > y)
	    return atanf (y / x);
	  else
	    return pi_over_2 - atanf (x / y);
	}
      else
	{
	  if (x > -y)
	    return atanf (y / x);
	  else
	    return - pi_over_2 - atanf (x / y);
	}
    }
  else
    {
      if (y < 0)
	{
	  if (-x > -y)
	    return - pi + atanf (y / x);
	  else
	    return - pi_over_2 - atanf (x / y);
	}
      else
	{
	  if (-x > y)
	    return pi + atanf (y / x);
	  else if (y > 0)
	    return pi_over_2 - atanf (x / y);
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
	return __ieee754_atan2(y, x);
}
#define __have_fpu_atan2

__MATH_INLINE double
__ieee754_sqrt (double x)
{
  double value;

  __asm ("fsqrt%.d %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_sqrtf (float x)
{
  float value;

  __asm ("fsqrt%.d %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_sqrtl (long double x)
{
	return __ieee754_sqrt(x);
}
#define __have_fpu_sqrt

__MATH_INLINE double
__ieee754_fabs (double x)
{
  double value;

  __asm ("fabs%.d %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_fabsf (float x)
{
  float value;

  __asm ("fabs%.d %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_fabsl (long double x)
{
	return __ieee754_fabs(x);
}
#define __have_fpu_fabs

__MATH_INLINE double
__ieee754_ceil (double x)
{
  long rounding_mode, round_up;
  double value;

  __asm volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_up = rounding_mode | 0x30;
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_up));
  __asm volatile ("fint%.d %1,%0"
		  : "=f" (value)
		  : "f" (x));
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}

__MATH_INLINE float
__ieee754_ceilf (float x)
{
  long rounding_mode, round_up;
  float value;

  __asm volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_up = rounding_mode | 0x30;
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_up));
  __asm volatile ("fint%.d %1,%0"
		  : "=f" (value)
		  : "f" (x));
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}

__MATH_INLINE long double
__ieee754_ceill (long double x)
{
	return __ieee754_ceil(x);
}
#define __have_fpu_ceil

__MATH_INLINE double
__ieee754_floor (double x)
{
  long rounding_mode, round_down;
  double value;

  __asm volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_down = (rounding_mode & ~0x10)
		| 0x20;
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_down));
  __asm volatile ("fint%.d %1,%0"
		  : "=f" (value)
		  : "f" (x));
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}

__MATH_INLINE float
__ieee754_floorf (float x)
{
  long rounding_mode, round_down;
  float value;

  __asm volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_down = (rounding_mode & ~0x10)
		| 0x20;
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_down));
  __asm volatile ("fint%.d %1,%0"
		  : "=f" (value)
		  : "f" (x));
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}

__MATH_INLINE long double
__ieee754_floorl (long double x)
{
	return __ieee754_floor(x);
}
#define __have_fpu_floor

__MATH_INLINE double
__ieee754_trunc (double x)
{
  double value;

  __asm volatile ("fintrz%.d %1,%0"
		  : "=f" (value)
		  : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_truncf (float x)
{
  float value;

  __asm volatile ("fintrz%.d %1,%0"
		  : "=f" (value)
		  : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_truncl (long double x)
{
	return __ieee754_trunc(x);
}
#define __have_fpu_trunc

__MATH_INLINE double
__ieee754_rint (double x)
{
  double value;
  __asm volatile ("fint%.d %1,%0"
		  : "=f" (value)
		  : "f" (x));
  return value;
}

__MATH_INLINE float
__ieee754_rintf (float x)
{
  float value;
  __asm volatile ("fint%.d %1,%0"
		  : "=f" (value)
		  : "f" (x));
  return value;
}

__MATH_INLINE long double
__ieee754_rintl (long double x)
{
	return __ieee754_rint(x);
}
#define __have_fpu_rint

__MATH_INLINE double
__ieee754_round (double x)
{
  long rounding_mode, round_nearest;
  double value;

  __asm volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_nearest = rounding_mode & ~0x30;
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_nearest));
  __asm volatile ("fint%.d %1,%0"
		  : "=f" (value)
		  : "f" (x));
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}

__MATH_INLINE float
__ieee754_roundf (float x)
{
  long rounding_mode, round_nearest;
  float value;

  __asm volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_nearest = rounding_mode & ~0x30;
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_nearest));
  __asm volatile ("fint%.d %1,%0"
		  : "=f" (value)
		  : "f" (x));
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}

__MATH_INLINE long double
__ieee754_roundl (long double x)
{
	return __ieee754_round(x);
}
#define __have_fpu_round

__MATH_INLINE double
__ieee754_modf (double x, double *ip)
{
  double temp;

  __asm ("fintrz%.d %1,%0"
	 : "=f" (temp)			/* integer-valued float */
	 : "f" (x));
  *ip = temp;
  return x - temp;
}

__MATH_INLINE float
__ieee754_modff (float x, float *ip)
{
  float temp;

  __asm ("fintrz%.d %1,%0"
	 : "=f" (temp)			/* integer-valued float */
	 : "f" (x));
  *ip = temp;
  return x - temp;
}

__MATH_INLINE long double
__ieee754_modfl (long double x, long double *ip)
{
  long double temp;

  __asm ("fintrz%.d %1,%0"
	 : "=f" (temp)			/* integer-valued float */
	 : "f" (x));
  *ip = temp;
  return x - temp;
}
#define __have_fpu_modf

#endif /* __NO_MATH_INLINES */

#endif
