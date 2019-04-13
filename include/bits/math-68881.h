/******************************************************************\
*								   *
*  <math-68881.h>		last modified: 23 May 1992.	   *
*								   *
*  Copyright (C) 1989 by Matthew Self.				   *
*  You may freely distribute verbatim copies of this software	   *
*  provided that this copyright notice is retained in all copies.  *
*  You may distribute modifications to this software under the     *
*  conditions above if you also clearly note such modifications    *
*  with their author and date.			   	     	   *
*								   *
*  Note:  errno is not set to EDOM when domain errors occur for    *
*  most of these functions.  Rather, it is assumed that the	   *
*  68881's OPERR exception will be enabled and handled		   *
*  appropriately by the	operating system.  Similarly, overflow	   *
*  and underflow do not set errno to ERANGE.			   *
*								   *
*  Send bugs to Matthew Self (self@bayes.arc.nasa.gov).		   *
*								   *
\******************************************************************/

/* This file is NOT a part of GCC, just distributed with it.  */

/* If you find this in GCC,
   please send bug reports to bug-gcc@prep.ai.mit.edu.  */

/* Changed by Richard Stallman:
   May 1993, add conditional to prevent multiple inclusion.
   % inserted before a #.
   New function `hypot' added.
   Nans written in hex to avoid 0rnan.
   May 1992, use %! for fpcr register.  Break lines before function names.
   December 1989, add parens around `&' in pow.
   November 1990, added alternate definition of HUGE_VAL for Sun.  */

/* Changed by Jim Wilson:
   September 1993, Use #undef before HUGE_VAL instead of #ifdef/#endif.  */

/* Changed by Ian Lance Taylor:
   September 1994, use extern inline instead of static inline.  */

#ifndef __math_68881
#define __math_68881

#include <errno.h>

/* GCC 4.3 and above with -std=c99 or -std=gnu99 implements ISO C99
   inline semantics, unless -fgnu89-inline is used.  */
#if defined __cplusplus
# define __MATH_68881_INLINE inline
#elif defined __GNUC_STDC_INLINE__
# define __MATH_68881_INLINE extern __inline __attribute__ ((__gnu_inline__))
#else
# define __MATH_68881_INLINE extern __inline
#endif

#undef HUGE_VAL
#ifdef __sun__
/* The Sun assembler fails to handle the hex constant in the usual defn.  */
#define HUGE_VAL							\
({									\
  static union { long i[2]; double d; } u = { {0x7ff00000, 0} };		\
  u.d;									\
})
#else
#define HUGE_VAL							\
({									\
  double huge_val;							\
									\
  __asm ("fmove%.d #0x7ff0000000000000,%0"	/* Infinity */		\
	 : "=f" (huge_val)						\
	 : /* no inputs */);						\
  huge_val;								\
})
#endif

__MATH_68881_INLINE double
sin (double x)
{
  double value;

  __asm ("fsin%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
sinf (float x)
{
  float value;

  __asm ("fsin%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_SINF 1

__MATH_68881_INLINE long double
sinl (long double x)
{
  long double value;

  __asm ("fsin%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_SINL 1

__MATH_68881_INLINE double
cos (double x)
{
  double value;

  __asm ("fcos%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
cosf (float x)
{
  float value;

  __asm ("fcos%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_COSF 1

__MATH_68881_INLINE long double
cosl (long double x)
{
  long double value;

  __asm ("fcos%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_COSL 1

__MATH_68881_INLINE void
sincos (double __x, double *__sin, double *__cos)
{
  __asm ("fsincos%.x %2,%0,%1"
	 : "=f" (*__cos), "=f" (*__sin)
	 : "f" (__x));
}

__MATH_68881_INLINE void
sincosf (float __x, float *__sin, float *__cos)
{
  __asm ("fsincos%.x %2,%0,%1"
	 : "=f" (*__cos), "=f" (*__sin)
	 : "f" (__x));
}
#define _GLIBCXX_HAVE_SINCOSF 1

__MATH_68881_INLINE void
sincosl (long double __x, long double *__sin, long double *__cos)
{
  __asm ("fsincos%.x %2,%0,%1"
	 : "=f" (*__cos), "=f" (*__sin)
	 : "f" (__x));
}
#define _GLIBCXX_HAVE_SINCOSL 1

__MATH_68881_INLINE double
tan (double x)
{
  double value;

  __asm ("ftan%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
tanf (float x)
{
  float value;

  __asm ("ftan%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_TANF 1

__MATH_68881_INLINE long double
tanl (long double x)
{
  long double value;

  __asm ("ftan%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_TANL 1

__MATH_68881_INLINE double
asin (double x)
{
  double value;

  __asm ("fasin%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
asinf (float x)
{
  float value;

  __asm ("fasin%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_ASINF 1

__MATH_68881_INLINE long double
asinl (long double x)
{
  long double value;

  __asm ("fasin%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_ASINL 1

__MATH_68881_INLINE double
acos (double x)
{
  double value;

  __asm ("facos%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
acosf (float x)
{
  float value;

  __asm ("facos%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_ACOSF 1

__MATH_68881_INLINE long double
acosl (long double x)
{
  long double value;

  __asm ("facos%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_ACOSL 1

__MATH_68881_INLINE double
atan (double x)
{
  double value;

  __asm ("fatan%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
atanf (float x)
{
  float value;

  __asm ("fatan%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_ATANF 1

__MATH_68881_INLINE long double
atanl (long double x)
{
  long double value;

  __asm ("fatan%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_ATANL 1

__MATH_68881_INLINE double
atan2 (double y, double x)
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
	      __asm ("fmove%.d #0x7fffffffffffffff,%0"	/* quiet NaN */
		     : "=f" (value)
		     : /* no inputs */);
	      return value;
	    }
	}
    }
}

__MATH_68881_INLINE float
atan2f (float y, float x)
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

__MATH_68881_INLINE long double
atan2l (long double y, long double x)
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
	    return atanl (y / x);
	  else
	    return pi_over_2 - atanl (x / y);
	}
      else
	{
	  if (x > -y)
	    return atanl (y / x);
	  else
	    return - pi_over_2 - atanl (x / y);
	}
    }
  else
    {
      if (y < 0)
	{
	  if (-x > -y)
	    return - pi + atanl (y / x);
	  else
	    return - pi_over_2 - atanl (x / y);
	}
      else
	{
	  if (-x > y)
	    return pi + atanl (y / x);
	  else if (y > 0)
	    return pi_over_2 - atanl (x / y);
	  else
	    {
	      long double value;

	      errno = EDOM;
	      value = __builtin_nanl("");
	      return value;
	    }
	}
    }
}
#define _GLIBCXX_HAVE_ATAN2L 1

__MATH_68881_INLINE double
sinh (double x)
{
  double value;

  __asm ("fsinh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
sinhf (float x)
{
  float value;

  __asm ("fsinh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_SINHF 1

__MATH_68881_INLINE long double
sinhl (long double x)
{
  long double value;

  __asm ("fsinh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_SINHL 1

__MATH_68881_INLINE double
cosh (double x)
{
  double value;

  __asm ("fcosh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
coshf (float x)
{
  float value;

  __asm ("fcosh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_COSHF 1

__MATH_68881_INLINE long double
coshl (long double x)
{
  long double value;

  __asm ("fcosh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_COSHL 1

__MATH_68881_INLINE double
tanh (double x)
{
  double value;

  __asm ("ftanh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
tanhf (float x)
{
  float value;

  __asm ("ftanh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_TANHF 1

__MATH_68881_INLINE long double
tanhl (long double x)
{
  long double value;

  __asm ("ftanh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_TANHL 1

__MATH_68881_INLINE double
atanh (double x)
{
  double value;

  __asm ("fatanh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
atanhf (float x)
{
  float value;

  __asm ("fatanh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_ATANHF 1

__MATH_68881_INLINE long double
atanhl (long double x)
{
  long double value;

  __asm ("fatanh%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_ATANHL 1

__MATH_68881_INLINE double
exp (double x)
{
  double value;

  __asm ("fetox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
expf (float x)
{
  float value;

  __asm ("fetox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_EXPF 1

__MATH_68881_INLINE long double
expl (long double x)
{
  long double value;

  __asm ("fetox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_EXPL 1

__MATH_68881_INLINE double
expm1 (double x)
{
  double value;

  __asm ("fetoxm1%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
expm1f (float x)
{
  float value;

  __asm ("fetoxm1%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_EXPM1F 1

__MATH_68881_INLINE long double
expm1l (long double x)
{
  long double value;

  __asm ("fetoxm1%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_EXPM1L 1

__MATH_68881_INLINE double
exp2 (double x)
{
  double value;

  __asm ("ftwotox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
exp2f (float x)
{
  float value;

  __asm ("ftwotox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_EXP2F 1

__MATH_68881_INLINE long double
exp2l (long double x)
{
  long double value;

  __asm ("ftwotox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_EXP2L 1

__MATH_68881_INLINE double
pow2 (double x)
{
  double value;

  __asm ("ftwotox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
pow2f (float x)
{
  float value;

  __asm ("ftwotox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_POW2F 1

__MATH_68881_INLINE long double
pow2l (long double x)
{
  long double value;

  __asm ("ftwotox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_POW2L 1

__MATH_68881_INLINE double
exp10 (double x)
{
  double value;

  __asm ("ftentox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
exp10f (float x)
{
  float value;

  __asm ("ftentox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_EXP10F 1

__MATH_68881_INLINE long double
exp10l (long double x)
{
  long double value;

  __asm ("ftentox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_EXP10L 1

__MATH_68881_INLINE double
pow10 (double x)
{
  double value;

  __asm ("ftentox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
pow10f (float x)
{
  float value;

  __asm ("ftentox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_POW10F 1

__MATH_68881_INLINE long double
pow10l (long double x)
{
  long double value;

  __asm ("ftentox%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_POW10L 1

__MATH_68881_INLINE double
log (double x)
{
  double value;

  __asm ("flogn%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
logf (float x)
{
  float value;

  __asm ("flogn%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_LOGF 1

__MATH_68881_INLINE long double
logl (long double x)
{
  long double value;

  __asm ("flogn%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_LOGL 1

__MATH_68881_INLINE double
log1p (double x)
{
  double value;

  __asm ("flognp1%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
log1pf (float x)
{
  float value;

  __asm ("flognp1%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_LOG1PF 1

__MATH_68881_INLINE long double
log1pl (long double x)
{
  long double value;

  __asm ("flognp1%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_LOG1PL 1

__MATH_68881_INLINE double
log10 (double x)
{
  double value;

  __asm ("flog10%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
log10f (float x)
{
  float value;

  __asm ("flog10%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_LOG10F 1

__MATH_68881_INLINE long double
log10l (long double x)
{
  long double value;

  __asm ("flog10%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_LOG10L 1

__MATH_68881_INLINE double
log2 (double x)
{
  double value;

  __asm ("flog2%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
log2f (float x)
{
  float value;

  __asm ("flog2%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_LOG2F 1

__MATH_68881_INLINE long double
log2l (long double x)
{
  long double value;

  __asm ("flog2%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_LOG2L 1

__MATH_68881_INLINE double
sqrt (double x)
{
  double value;

  __asm ("fsqrt%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
sqrtf (float x)
{
  float value;

  __asm ("fsqrt%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_SQRTF 1

__MATH_68881_INLINE long double
sqrtl (long double x)
{
  long double value;

  __asm ("fsqrt%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
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
	  __asm ("fmove%.d #0x7fffffffffffffff,%0"		/* quiet NaN */
		 : "=f" (value)
		 : /* no inputs */);
	  return value;
	}
    }
  else
    {
      double temp;

      __asm ("fintrz%.x %1,%0"
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
	  __asm ("fmove%.d #0x7fffffffffffffff,%0"		/* quiet NaN */
		 : "=f" (value)
		 : /* no inputs */);
	  return value;
        }
    }
}

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

      __asm ("fintrz%.x %1,%0"
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

__MATH_68881_INLINE long double
powl (long double x, long double y)
{
  if (x > 0)
    return expl (y * logl (x));
  else if (x == 0)
    {
      if (y > 0)
	return 0.0;
      else
	{
	  long double value;

	  errno = EDOM;
	  value = __builtin_nanl("");
	  return value;
	}
    }
  else
    {
      long double temp;

      __asm ("fintrz%.x %1,%0"
	     : "=f" (temp)			/* integer-valued long double */
	     : "f" (y));
      if (y == temp)
        {
	  int i = (int) y;

	  if ((i & 1) == 0)			/* even */
	    return expl (y * logf (-x));
	  else
	    return - expl (y * logl (-x));
        }
      else
        {
	  long double value;

	  errno = EDOM;
	  value = __builtin_nanl("");
	  return value;
        }
    }
}
#define _GLIBCXX_HAVE_POWL 1

__MATH_68881_INLINE double
fabs (double x)
{
  double value;

  __asm ("fabs%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}

__MATH_68881_INLINE float
fabsf (float x)
{
  float value;

  __asm ("fabs%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_FABSF 1

__MATH_68881_INLINE long double
fabsl (long double x)
{
  long double value;

  __asm ("fabs%.x %1,%0"
	 : "=f" (value)
	 : "f" (x));
  return value;
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
  __asm volatile ("fint%.x %1,%0"
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
  __asm volatile ("fint%.x %1,%0"
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
  long rounding_mode, round_up;
  long double value;

  __asm volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_up = rounding_mode | 0x30;
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_up));
  __asm volatile ("fint%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
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
  __asm volatile ("fint%.x %1,%0"
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
  __asm volatile ("fint%.x %1,%0"
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
  long rounding_mode, round_down;
  long double value;

  __asm volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_down = (rounding_mode & ~0x10)
		| 0x20;
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_down));
  __asm volatile ("fint%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}
#define _GLIBCXX_HAVE_FLOORL 1

__MATH_68881_INLINE double
trunc (double x)
{
  double value;

  __asm volatile ("fintrz%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  return value;
}

__MATH_68881_INLINE float
truncf (float x)
{
  float value;

  __asm volatile ("fintrz%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_TRUNCF 1

__MATH_68881_INLINE long double
truncl (long double x)
{
  long double value;

  __asm volatile ("fintrz%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  return value;
}
#define _GLIBCXX_HAVE_TRUNCL 1

__MATH_68881_INLINE double
rint (double x)
{
  double value;
#if 0
  long rounding_mode, round_nearest;

  __asm volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_nearest = rounding_mode & ~0x30;
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_nearest));
#endif
  __asm volatile ("fint%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
#if 0
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
#endif
  return value;
}

__MATH_68881_INLINE float
rintf (float x)
{
  float value;
#if 0
  long rounding_mode, round_nearest;

  __asm volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_nearest = rounding_mode & ~0x30;
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_nearest));
#endif
  __asm volatile ("fint%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
#if 0
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
#endif
  return value;
}
#define _GLIBCXX_HAVE_RINTF 1

__MATH_68881_INLINE long double
rintl (long double x)
{
  long double value;
#if 0
  long rounding_mode, round_nearest;

  __asm volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_nearest = rounding_mode & ~0x30;
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_nearest));
#endif
  __asm volatile ("fint%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
#if 0
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
#endif
  return value;
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
  __asm volatile ("fint%.x %1,%0"
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
  __asm volatile ("fint%.x %1,%0"
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
  long rounding_mode, round_nearest;
  long double value;

  __asm volatile ("fmove%.l %!,%0"
		  : "=dm" (rounding_mode)
		  : /* no inputs */ );
  round_nearest = rounding_mode & ~0x30;
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (round_nearest));
  __asm volatile ("fint%.x %1,%0"
		  : "=f" (value)
		  : "f" (x));
  __asm volatile ("fmove%.l %0,%!"
		  : /* no outputs */
		  : "dmi" (rounding_mode));
  return value;
}
#define _GLIBCXX_HAVE_ROUNDL 1

__MATH_68881_INLINE double
fmod (double x, double y)
{
  double value;

  __asm ("fmod%.x %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "f" (y));
  return value;
}

__MATH_68881_INLINE float
fmodf (float x, float y)
{
  float value;

  __asm ("fmod%.x %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "f" (y));
  return value;
}
#define _GLIBCXX_HAVE_FMODF 1

__MATH_68881_INLINE long double
fmodl (long double x, long double y)
{
  long double value;

  __asm ("fmod%.x %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "f" (y));
  return value;
}
#define _GLIBCXX_HAVE_FMODL 1

__MATH_68881_INLINE double
drem (double x, double y)
{
  double value;

  __asm ("frem%.x %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "f" (y));
  return value;
}

__MATH_68881_INLINE float
dremf (float x, float y)
{
  float value;

  __asm ("frem%.x %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "f" (y));
  return value;
}
#define _GLIBCXX_HAVE_DREMF 1

__MATH_68881_INLINE long double
dreml (long double x, long double y)
{
  long double value;

  __asm ("frem%.x %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "f" (y));
  return value;
}
#define _GLIBCXX_HAVE_DREML 1

__MATH_68881_INLINE double
remainder (double x, double y)
{
  double value;

  __asm ("frem%.x %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "f" (y));
  return value;
}

__MATH_68881_INLINE float
remainderf (float x, float y)
{
  float value;

  __asm ("frem%.x %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "f" (y));
  return value;
}
#define _GLIBCXX_HAVE_REMAINDERF 1

__MATH_68881_INLINE long double
remainderl (long double x, long double y)
{
  long double value;

  __asm ("frem%.x %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "f" (y));
  return value;
}
#define _GLIBCXX_HAVE_REMAINDERL 1

__MATH_68881_INLINE double
scalb (double x, double n)
{
  double value;
  long exp = (long)(n);

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (exp));
  return value;
}

__MATH_68881_INLINE float
scalbf (float x, float n)
{
  float value;
  long exp = (long)(n);

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (exp));
  return value;
}
#define _GLIBCXX_HAVE_SCALBF 1

__MATH_68881_INLINE long double
scalbl (long double x, long double n)
{
  long double value;
  long exp = (long)(n);

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (exp));
  return value;
}
#define _GLIBCXX_HAVE_SCALBL 1

__MATH_68881_INLINE double
scalbn (double x, int n)
{
  double value;
  long exp = (long)(n);

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (exp));
  return value;
}

__MATH_68881_INLINE float
scalbnf (float x, int n)
{
  float value;
  long exp = (long)(n);

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (exp));
  return value;
}
#define _GLIBCXX_HAVE_SCALBNF 1

__MATH_68881_INLINE long double
scalbnl (long double x, int n)
{
  long double value;
  long exp = (long)(n);

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (exp));
  return value;
}
#define _GLIBCXX_HAVE_SCALBNL 1

__MATH_68881_INLINE double
scalbln (double x, long n)
{
  double value;
  long exp = (long)(n);

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (exp));
  return value;
}

__MATH_68881_INLINE float
scalblnf (float x, long n)
{
  float value;
  long exp = (long)(n);

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (exp));
  return value;
}
#define _GLIBCXX_HAVE_SCALBLNF 1

__MATH_68881_INLINE long double
scalblnl (long double x, long n)
{
  long double value;
  long exp = (long)(n);

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (exp));
  return value;
}
#define _GLIBCXX_HAVE_SCALBLNL 1

__MATH_68881_INLINE double
logb (double x)
{
  double exponent;

  __asm ("fgetexp%.x %1,%0"
	 : "=f" (exponent)
	 : "f" (x));
  return exponent;
}

__MATH_68881_INLINE float
logbf (float x)
{
  float exponent;

  __asm ("fgetexp%.x %1,%0"
	 : "=f" (exponent)
	 : "f" (x));
  return exponent;
}
#define _GLIBCXX_HAVE_LOGBF 1

__MATH_68881_INLINE long double
logbl (long double x)
{
  long double exponent;

  __asm ("fgetexp%.x %1,%0"
	 : "=f" (exponent)
	 : "f" (x));
  return exponent;
}
#define _GLIBCXX_HAVE_LOGBL 1

__MATH_68881_INLINE int
ilogb (double x)
{
  double exponent;

  __asm ("fgetexp%.x %1,%0"
	 : "=f" (exponent)
	 : "f" (x));
  return (int)exponent;
}

__MATH_68881_INLINE int
ilogbf (float x)
{
  float exponent;

  __asm ("fgetexp%.x %1,%0"
	 : "=f" (exponent)
	 : "f" (x));
  return (int)exponent;
}
#define _GLIBCXX_HAVE_ILOGBF 1

__MATH_68881_INLINE int
ilogbl (long double x)
{
  long double exponent;

  __asm ("fgetexp%.x %1,%0"
	 : "=f" (exponent)
	 : "f" (x));
  return (int)exponent;
}
#define _GLIBCXX_HAVE_ILOGBL 1


__MATH_68881_INLINE double
ldexp (double x, int n)
{
  double value;

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (n));
  return value;
}

__MATH_68881_INLINE float
ldexpf (float x, int n)
{
  float value;

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (n));
  return value;
}
#define _GLIBCXX_HAVE_LDEXPF 1

__MATH_68881_INLINE long double
ldexpl (long double x, int n)
{
  long double value;

  __asm ("fscale%.l %2,%0"
	 : "=f" (value)
	 : "0" (x),
	   "dmi" (n));
  return value;
}
#define _GLIBCXX_HAVE_LDEXPL 1

__MATH_68881_INLINE double
frexp (double x, int *exp)
{
  double float_exponent;
  long int_exponent;
  double mantissa;

  __asm ("fgetexp%.x %1,%0"
	 : "=f" (float_exponent)	/* integer-valued float */
	 : "f" (x));
  int_exponent = (long) float_exponent;
  __asm ("fgetman%.x %1,%0"
	 : "=f" (mantissa)		/* 1.0 <= mantissa < 2.0 */
	 : "f" (x));
  if (mantissa != 0)
    {
      __asm ("fscale%.b #-1,%0"
	     : "=f" (mantissa)		/* mantissa /= 2.0 */
	     : "0" (mantissa));
      int_exponent += 1;
    }
  *exp = int_exponent;
  return mantissa;
}

__MATH_68881_INLINE float
frexpf (float x, int *exp)
{
  float float_exponent;
  long int_exponent;
  float mantissa;

  __asm ("fgetexp%.x %1,%0"
	 : "=f" (float_exponent)	/* integer-valued float */
	 : "f" (x));
  int_exponent = (long) float_exponent;
  __asm ("fgetman%.x %1,%0"
	 : "=f" (mantissa)		/* 1.0 <= mantissa < 2.0 */
	 : "f" (x));
  if (mantissa != 0)
    {
      __asm ("fscale%.b #-1,%0"
	     : "=f" (mantissa)		/* mantissa /= 2.0 */
	     : "0" (mantissa));
      int_exponent += 1;
    }
  *exp = int_exponent;
  return mantissa;
}
#define _GLIBCXX_HAVE_FREXPF 1

__MATH_68881_INLINE long double
frexpl (long double x, int *exp)
{
  long double float_exponent;
  long int_exponent;
  long double mantissa;

  __asm ("fgetexp%.x %1,%0"
	 : "=f" (float_exponent)	/* integer-valued float */
	 : "f" (x));
  int_exponent = (long) float_exponent;
  __asm ("fgetman%.x %1,%0"
	 : "=f" (mantissa)		/* 1.0 <= mantissa < 2.0 */
	 : "f" (x));
  if (mantissa != 0)
    {
      __asm ("fscale%.b #-1,%0"
	     : "=f" (mantissa)		/* mantissa /= 2.0 */
	     : "0" (mantissa));
      int_exponent += 1;
    }
  *exp = int_exponent;
  return mantissa;
}
#define _GLIBCXX_HAVE_FREXPL 1

__MATH_68881_INLINE double
significand(double x)
{
  double mantissa;

  __asm ("fgetman%.x %1,%0"
	 : "=f" (mantissa)		/* 1.0 <= mantissa < 2.0 */
	 : "f" (x));
  return mantissa;
}

__MATH_68881_INLINE float
significandf(float x)
{
  float mantissa;

  __asm ("fgetman%.x %1,%0"
	 : "=f" (mantissa)		/* 1.0 <= mantissa < 2.0 */
	 : "f" (x));
  return mantissa;
}
#define _GLIBCXX_HAVE_SIGNIFICANDF 1

__MATH_68881_INLINE long double
significandl(long double x)
{
  long double mantissa;

  __asm ("fgetman%.x %1,%0"
	 : "=f" (mantissa)		/* 1.0 <= mantissa < 2.0 */
	 : "f" (x));
  return mantissa;
}
#define _GLIBCXX_HAVE_SIGNIFICANDL 1

__MATH_68881_INLINE double
modf (double x, double *ip)
{
  double temp;

  __asm ("fintrz%.x %1,%0"
	 : "=f" (temp)			/* integer-valued float */
	 : "f" (x));
  *ip = temp;
  return x - temp;
}

__MATH_68881_INLINE float
modff (float x, float *ip)
{
  float temp;

  __asm ("fintrz%.x %1,%0"
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

  __asm ("fintrz%.x %1,%0"
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

  __asm ("fcmp%.x %1,%2\n"
  	 "\tfsun %0"
	 : "=d" (res)
	 : "f" (x), "f" (y));
  return -res;
}

__MATH_68881_INLINE int
__isunorderedf (float x, float y)
{
  signed char res;

  __asm ("fcmp%.x %1,%2\n"
  	 "\tfsun %0"
	 : "=d" (res)
	 : "f" (x), "f" (y));
  return -res;
}

__MATH_68881_INLINE int
__isunorderedl (long double x, long double y)
{
  signed char res;

  __asm ("fcmp%.x %1,%2\n"
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

__MATH_68881_INLINE double
fma(double x, double y, double z)
{
	return (x * y) + z;
}

__MATH_68881_INLINE float
fmaf(float x, float y, float z)
{
	return (x * y) + z;
}
#define _GLIBCXX_HAVE_FMAF 1

__MATH_68881_INLINE long double
fmal(long double x, long double y, long double z)
{
	return (x * y) + z;
}
#define _GLIBCXX_HAVE_FMAL 1

#endif /* __USE_ISOC99 */

#endif /* not __math_68881 */
