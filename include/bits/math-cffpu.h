/******************************************************************\
*                                                                  *
*  <math-cffpu.h>               last modified: 6 Apr 2014.         *
*                                                                  *
*  Copyright (C) 2014 by Thorsten Otto.                            *
*  Modified version of <math-68881.h>, adjusted for ColdFire FPUs  *
*                                                                  *
*  You may freely distribute verbatim copies of this software      *
*  provided that this copyright notice is retained in all copies.  *
*  You may distribute modifications to this software under the     *
*  conditions above if you also clearly note such modifications    *
*  with their author and date.                                     *
*                                                                  *
*  Note:  errno is not set to EDOM when domain errors occur for    *
*  most of these functions.  Rather, it is assumed that the        *
*  68881's OPERR exception will be enabled and handled             *
*  appropriately by the operating system.  Similarly, overflow     *
*  and underflow do not set errno to ERANGE.                       *
*                                                                  *
\******************************************************************/

#ifndef __math_cffpu
#define __math_cffpu

#include <errno.h>

/* GCC 4.3 and above with -std=c99 or -std=gnu99 implements ISO C99
   inline semantics, unless -fgnu89-inline is used.  */
#ifdef __cplusplus
# define __MATH_68881_INLINE inline
#elif defined __GNUC_STDC_INLINE__
# define __MATH_68881_INLINE extern __inline __attribute__ ((__gnu_inline__))
#else
# define __MATH_68881_INLINE extern __inline
#endif

__MATH_68881_INLINE double
atan2 (double y, double x)
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

	      errno = EDOM;
	      value = __builtin_nan("");
	      return value;
	    }
	}
    }
}

#if defined __USE_MISC || defined __USE_ISOC99
__MATH_68881_INLINE float
atan2f (float y, float x)
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

	      errno = EDOM;
	      value = __builtin_nanf("");
	      return value;
	    }
	}
    }
}
#define _GLIBCXX_HAVE_ATAN2F 1
#endif

__MATH_68881_INLINE long double
atan2l (long double y, long double x)
{
	return atan2(y, x);
}
#define _GLIBCXX_HAVE_ATAN2L 1

__MATH_68881_INLINE double
sqrt (double x)
{
  double value;

  __asm ("fsqrt%.d %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
sqrtf (float x)
{
  float value;

  __asm ("fsqrt%.d %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_SQRTF 1

__MATH_68881_INLINE long double
sqrtl (long double x)
{
	return sqrt(x);
}
#define _GLIBCXX_HAVE_SQRTL 1

__MATH_68881_INLINE double
hypot (double x, double y)
{
  return __builtin_isinf(x) || __builtin_isinf(y) ? __builtin_inf() : sqrt (x*x + y*y);
}

__MATH_68881_INLINE float
hypotf (float x, float y)
{
  return __builtin_isinff(x) || __builtin_isinff(y) ? __builtin_inff() : sqrtf (x*x + y*y);
}
#define _GLIBCXX_HAVE_HYPOTF 1

__MATH_68881_INLINE long double
hypotl (long double x, long double y)
{
  return __builtin_isinfl(x) || __builtin_isinfl(y) ? __builtin_infl() : sqrtl (x*x + y*y);
}
#define _GLIBCXX_HAVE_HYPOTL 1

__MATH_68881_INLINE double
pow (double x, double y)
{
  if (x > 0)
    return exp (y * log (x));
  else if (x == 0)
    {
      if (y > 0)
	return 0.0;
      else
	{
	  double value;

	  errno = EDOM;
	  value = __builtin_nan("");		/* quiet NaN */
	  return value;
	}
    }
  else
    {
      double temp;

      __asm ("fintrz%.d %1,%0"
	     : "=f" (temp)			/* integer-valued float */
	     : "f" (y));
      if (y == temp)
        {
	  int i = (int) y;

	  if ((i & 1) == 0)			/* even */
	    return exp (y * log (-x));
	  else
	    return - exp (y * log (-x));
        }
      else
        {
	  double value;

	  errno = EDOM;
	  value = __builtin_nan("");		/* quiet NaN */
	  return value;
        }
    }
}

#if defined __USE_MISC || defined __USE_ISOC99
__MATH_68881_INLINE float
powf (float x, float y)
{
  if (x > 0)
    return expf (y * logf (x));
  else if (x == 0)
    {
      if (y > 0)
	return 0.0;
      else
	{
	  float value;

	  errno = EDOM;
	  value = __builtin_nanf("");
	  return value;
	}
    }
  else
    {
      float temp;

      __asm ("fintrz%.d %1,%0"
	     : "=f" (temp)			/* integer-valued float */
	     : "f" (y));
      if (y == temp)
        {
	  int i = (int) y;

	  if ((i & 1) == 0)			/* even */
	    return expf (y * logf (-x));
	  else
	    return - expf (y * logf (-x));
        }
      else
        {
	  float value;

	  errno = EDOM;
	  value = __builtin_nanf("");
	  return value;
        }
    }
}
#define _GLIBCXX_HAVE_POWF 1
#endif

__MATH_68881_INLINE long double
powl (long double x, long double y)
{
	return pow(x, y);
}
#define _GLIBCXX_HAVE_POWL 1

__MATH_68881_INLINE double
fabs (double x)
{
  double value;

  __asm ("fabs%.d %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
fabsf (float x)
{
  float value;

  __asm ("fabs%.d %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_FABSF 1

__MATH_68881_INLINE long double
fabsl (long double x)
{
	return fabs(x);
}
#define _GLIBCXX_HAVE_FABSL 1

__MATH_68881_INLINE double
ceil (double x)
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

__MATH_68881_INLINE float
ceilf (float x)
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
#define _GLIBCXX_HAVE_CEILF 1

__MATH_68881_INLINE long double
ceill (long double x)
{
	return ceil(x);
}
#define _GLIBCXX_HAVE_CEILL 1

__MATH_68881_INLINE double
floor (double x)
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

__MATH_68881_INLINE float
floorf (float x)
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
#define _GLIBCXX_HAVE_FLOORF 1

__MATH_68881_INLINE long double
floorl (long double x)
{
	return floor(x);
}
#define _GLIBCXX_HAVE_FLOORL 1

__MATH_68881_INLINE double
trunc (double x)
{
  double value;

  __asm volatile ("fintrz%.d %1,%0"
		  : "=f" (value)
		  : "f" (x));
  return value;
}

__MATH_68881_INLINE float
truncf (float x)
{
  float value;

  __asm volatile ("fintrz%.d %1,%0"
		  : "=f" (value)
		  : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_TRUNCF 1

__MATH_68881_INLINE long double
truncl (long double x)
{
	return trunc(x);
}
#define _GLIBCXX_HAVE_TRUNCL 1

__MATH_68881_INLINE double
rint (double x)
{
  double value;
  __asm volatile ("fint%.d %1,%0"
		  : "=f" (value)
		  : "f" (x));
  return value;
}

__MATH_68881_INLINE float
rintf (float x)
{
  float value;
  __asm volatile ("fint%.d %1,%0"
		  : "=f" (value)
		  : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_RINTF 1

__MATH_68881_INLINE long double
rintl (long double x)
{
	return rint(x);
}
#define _GLIBCXX_HAVE_RINTL 1

__MATH_68881_INLINE double
round (double x)
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

__MATH_68881_INLINE float
roundf (float x)
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
#define _GLIBCXX_HAVE_ROUNDF 1

__MATH_68881_INLINE long double
roundl (long double x)
{
	return round(x);
}
#define _GLIBCXX_HAVE_ROUNDL 1

__MATH_68881_INLINE double
modf (double x, double *ip)
{
  double temp;

  __asm ("fintrz%.d %1,%0"
	 : "=f" (temp)			/* integer-valued float */
	 : "f" (x));
  *ip = temp;
  return x - temp;
}

__MATH_68881_INLINE float
modff (float x, float *ip)
{
  float temp;

  __asm ("fintrz%.d %1,%0"
	 : "=f" (temp)			/* integer-valued float */
	 : "f" (x));
  *ip = temp;
  return x - temp;
}
#define _GLIBCXX_HAVE_MODFF 1

__MATH_68881_INLINE long double
modfl (long double x, long double *ip)
{
  long double temp;

  __asm ("fintrz%.d %1,%0"
	 : "=f" (temp)			/* integer-valued float */
	 : "f" (x));
  *ip = temp;
  return x - temp;
}
#define _GLIBCXX_HAVE_MODFL 1

#ifdef __USE_ISOC99

#ifndef isunordered
__MATH_68881_INLINE int
__isunordered (double x, double y)
{
  signed char res;

  __asm ("fcmp%.d %1,%2\n"
  	 "\tfsun %0"
	 : "=d" (res)
	 : "f" (x), "f" (y));
  return -res;
}

__MATH_68881_INLINE int
__isunorderedf (float x, float y)
{
  signed char res;

  __asm ("fcmp%.d %1,%2\n"
  	 "\tfsun %0"
	 : "=d" (res)
	 : "f" (x), "f" (y));
  return -res;
}

__MATH_68881_INLINE int
__isunorderedl (long double x, long double y)
{
  signed char res;

  __asm ("fcmp%.d %1,%2\n"
  	 "\tfsun %0"
	 : "=d" (res)
	 : "f" (x), "f" (y));
  return -res;
}

# ifdef __NO_LONG_DOUBLE_MATH
#  define isunordered(x, y) \
     (sizeof (x) == sizeof (float) && sizeof (y) == sizeof (float) ? __isunorderedf (x, y) : __isunordered (x, y))
# else
#  define isunordered(x, y) \
     (sizeof (x) == sizeof (float) && sizeof (y) == sizeof (float)					      \
      ? __isunorderedf (x, y)						      \
      : sizeof (x) == sizeof (double) && sizeof (y) == sizeof (double)					      \
      ? __isunordered (x, y) : __isunorderedl (x, y))
# endif
#endif

#endif /* __USE_ISOC99 */

#endif /* math-cffpu.h */
