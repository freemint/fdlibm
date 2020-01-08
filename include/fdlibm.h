/* @(#)fdlibm.h 1.5 04/04/22 */
/*
 * ====================================================
 * Copyright (C) 2004 by Sun Microsystems, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

#ifndef __FDLIBM_H__
#define __FDLIBM_H__

#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif

#ifdef _LIBC
#include "lib.h"
#endif
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <errno.h>
#include <stdint.h>
#include <limits.h>

#include <fenv.h>

#ifndef __BYTE_ORDER__
#  error "endianness not defined"
#endif

#ifndef __FLOAT_WORD_ORDER__
#define __FLOAT_WORD_ORDER__ __BYTE_ORDER__
#endif

#define IEEE754_FLOAT_MAXEXP	0xff
#define IEEE754_FLOAT_BIAS	0x7f /* Added to exponent.  */
#define IEEE754_FLOAT_SHIFT 23

#define IEEE754_DOUBLE_MAXEXP	0x7ff
#define IEEE754_DOUBLE_BIAS	0x3ff /* Added to exponent.  */
#define IEEE754_DOUBLE_SHIFT 20

#define IEEE854_LONG_DOUBLE_MAXEXP	0x7fff
#define IEEE854_LONG_DOUBLE_BIAS 0x3fff
#define IEEE854_LONG_DOUBLE_SHIFT 0

/* A union which permits us to convert between a double and two 32 bit
   ints.  */

#if __FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__

typedef union
{
  double value;
  struct
  {
    uint32_t msw;
    uint32_t lsw;
  } parts;
} ieee_double_shape_type;

typedef union
{
  long double value;
  struct
  {
    int16_t sign_exponent;
#if !defined(__PUREC__) || defined(__MATH_68881__)
    uint16_t empty;
#define SET_LDOUBLE_EMPTY(u) (u).parts.empty = 0
#endif
    uint32_t msw;
    uint32_t lsw;
  } parts;
} ieee_long_double_shape_type;

#endif

#if __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__

typedef union
{
  double value;
  struct
  {
    uint32_t lsw;
    uint32_t msw;
  } parts;
} ieee_double_shape_type;

typedef union
{
  long double value;
  struct
  {
    uint32_t lsw;
    uint32_t msw;
    int sign_exponent:16;
    unsigned int empty:16;
#define SET_LDOUBLE_EMPTY(u) (u).parts.empty = 0
  } parts;
} ieee_long_double_shape_type;

#endif

#ifndef SET_LDOUBLE_EMPTY
#define SET_LDOUBLE_EMPTY(u)
#endif

/* A union which permits us to convert between a float and a 32 bit
   int.  */

typedef union
{
  float value;
  uint32_t word;
} ieee_float_shape_type;


/* Get two 32 bit ints from a double.  */

#define GET_DOUBLE_WORDS(ix0,ix1,d)				\
{								\
  const ieee_double_shape_type *ew_u = (const ieee_double_shape_type *)&(d);					\
  (ix0) = ew_u->parts.msw;					\
  (ix1) = ew_u->parts.lsw;					\
}

/* Get the more significant 32 bit int from a double.  */

#define GET_HIGH_WORD(i,d)					\
{								\
  const ieee_double_shape_type *gh_u = (const ieee_double_shape_type *)&(d);					\
  (i) = gh_u->parts.msw;						\
}

/* Get the less significant 32 bit int from a double.  */

#define GET_LOW_WORD(i,d)					\
{								\
  const ieee_double_shape_type *gl_u = (const ieee_double_shape_type *)&(d);					\
  (i) = gl_u->parts.lsw;						\
}

/* Set a double from two 32 bit ints.  */

#define INSERT_WORDS(d,ix0,ix1)                                 \
do {                                                            \
  ieee_double_shape_type *iw_u = (ieee_double_shape_type *)&(d);\
  iw_u->parts.msw = (ix0);                                      \
  iw_u->parts.lsw = (ix1);                                      \
} while (0)

/* Set the more significant 32 bits of a double from an int.  */

#define SET_HIGH_WORD(d,v)                                      \
{                                                               \
  ieee_double_shape_type *sh_u = (ieee_double_shape_type *)&(d);\
  sh_u->parts.msw = (v);                                        \
}

/* Set the less significant 32 bits of a double from an int.  */

#define SET_LOW_WORD(d,v)                                       \
{                                                               \
  ieee_double_shape_type *sl_u = (ieee_double_shape_type *)&(d);\
  sl_u->parts.lsw = (v);                                        \
}

/* Get a 32 bit int from a float.  */

#define GET_FLOAT_WORD(i,d)					\
{								\
  const ieee_float_shape_type *gf_u = (const ieee_float_shape_type *)&(d);					\
  (i) = gf_u->word;						\
}

/* Set a float from a 32 bit int.  */

#define SET_FLOAT_WORD(d,i)					\
{								\
  ieee_float_shape_type *sf_u = (ieee_float_shape_type *)&(d);					\
  sf_u->word = (i);						\
}

#if __GNUC__ <= 4 /* work around an internal compiler error */

/* Get three 32 bit ints from a long double.  */

#define GET_LDOUBLE_WORDS(exp,ix0,ix1,d)			\
do {								\
  ieee_long_double_shape_type ew_u;				\
  ew_u.value = (d);						\
  (exp) = ew_u.parts.sign_exponent;				\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)

/* Set a long double from two 32 bit ints.  */

#define SET_LDOUBLE_WORDS(d,exp,ix0,ix1)			\
do {								\
  ieee_long_double_shape_type iw_u;				\
  iw_u.parts.sign_exponent = (exp);				\
  SET_LDOUBLE_EMPTY(iw_u); \
  iw_u.parts.msw = (ix0);					\
  iw_u.parts.lsw = (ix1);					\
  (d) = iw_u.value;						\
} while (0)

/* Get the more significant 32 bits of a long double mantissa.  */

#define GET_LDOUBLE_MSW(v,d)					\
do {								\
  ieee_long_double_shape_type sh_u;				\
  sh_u.value = (d);						\
  (v) = sh_u.parts.msw;						\
} while (0)

/* Get the less significant 32 bits of a long double mantissa.  */

#define GET_LDOUBLE_LSW(v,d)					\
do {								\
  ieee_long_double_shape_type sh_u;				\
  sh_u.value = (d);						\
  (v) = sh_u.parts.lsw;						\
} while (0)

/* Set the more significant 32 bits of a long double mantissa from an int.  */

#define SET_LDOUBLE_MSW(d,v)					\
do {								\
  ieee_long_double_shape_type sh_u;				\
  sh_u.value = (d);						\
  sh_u.parts.msw = (v);						\
  (d) = sh_u.value;						\
} while (0)

/* Set the less significant 32 bits of a long double mantissa from an int.  */

#define SET_LDOUBLE_LSW(d,v)					\
do {								\
  ieee_long_double_shape_type sh_u;				\
  sh_u.value = (d);						\
  sh_u.parts.lsw = (v);						\
  (d) = sh_u.value;						\
} while (0)

/* Get int from the exponent of a long double.  */

#define GET_LDOUBLE_EXP(exp,d)					\
do {								\
  ieee_long_double_shape_type ge_u;				\
  ge_u.value = (d);						\
  (exp) = ge_u.parts.sign_exponent;				\
} while (0)

/* Set exponent of a long double from an int.  */

#define SET_LDOUBLE_EXP(d,exp)					\
do {								\
  ieee_long_double_shape_type se_u;				\
  se_u.value = (d);						\
  se_u.parts.sign_exponent = (exp);				\
  (d) = se_u.value;						\
} while (0)

#else

/* Get three 32 bit ints from a long double.  */

#define GET_LDOUBLE_WORDS(exp,ix0,ix1,d)			\
do {								\
  const ieee_long_double_shape_type *ew_u = (const ieee_long_double_shape_type *)&(d);				\
  (exp) = ew_u->parts.sign_exponent;				\
  (ix0) = ew_u->parts.msw;					\
  (ix1) = ew_u->parts.lsw;					\
} while (0)

/* Set a long double from two 32 bit ints.  */

#define SET_LDOUBLE_WORDS(d,exp,ix0,ix1)			\
do {								\
  ieee_long_double_shape_type *iw_u = (ieee_long_double_shape_type *)&(d);				\
  iw_u->parts.sign_exponent = (exp);				\
  SET_LDOUBLE_EMPTY(*iw_u); \
  iw_u->parts.msw = (ix0);					\
  iw_u->parts.lsw = (ix1);					\
} while (0)

/* Get the more significant 32 bits of a long double mantissa.  */

#define GET_LDOUBLE_MSW(v,d)					\
do {								\
  const ieee_long_double_shape_type *sh_u = (const ieee_long_double_shape_type *)&(d);				\
  (v) = sh_u->parts.msw;						\
} while (0)

/* Get the less significant 32 bits of a long double mantissa.  */

#define GET_LDOUBLE_LSW(v,d)					\
do {								\
  const ieee_long_double_shape_type *sh_u = (const ieee_long_double_shape_type *)&(d);				\
  (v) = sh_u->parts.lsw;						\
} while (0)

/* Set the more significant 32 bits of a long double mantissa from an int.  */

#define SET_LDOUBLE_MSW(d,v)					\
do {								\
  ieee_long_double_shape_type *sh_u = (ieee_long_double_shape_type *)&(d);				\
  sh_u->parts.msw = (v);						\
} while (0)

/* Set the less significant 32 bits of a long double mantissa from an int.  */

#define SET_LDOUBLE_LSW(d,v)					\
do {								\
  ieee_long_double_shape_type *sh_u = (ieee_long_double_shape_type *)&(d);				\
  sh_u->parts.lsw = (v);						\
} while (0)

/* Get int from the exponent of a long double.  */

#define GET_LDOUBLE_EXP(exp,d)					\
do {								\
  const ieee_long_double_shape_type *ge_u = (const ieee_long_double_shape_type *)&(d);				\
  (exp) = ge_u->parts.sign_exponent;				\
} while (0)

/* Set exponent of a long double from an int.  */

#define SET_LDOUBLE_EXP(d,exp)					\
do {								\
  ieee_long_double_shape_type *se_u = (ieee_long_double_shape_type *)&(d);				\
  se_u->parts.sign_exponent = (exp);				\
} while (0)

#endif

#if INT_MAX > 32767
#  define IC(x) ((int32_t) x)
#  define UC(x) ((uint32_t) x)
#else
#  define IC(x) ((int32_t) x##L)
#  define UC(x) ((uint32_t) x##UL)
#endif

/*
 * ANSI/POSIX
 */

extern int *__signgam (void);
#undef signgam
extern int signgam;

#undef MAXFLOAT
#define	MAXFLOAT	((float)3.40282346638528860e+38)

#ifndef _LIB_VERSION
/*
 * define and initialize _LIB_VERSION
 */
#if defined(_POSIX_MODE)
#define _LIB_VERSION _POSIX_
#elif defined(_XOPEN_MODE)
#define _LIB_VERSION _XOPEN_
#elif defined(_SVID3_MODE)
#define _LIB_VERSION _SVID_
#else
#define _LIB_VERSION _IEEE_
#endif

#endif

#if !defined(_MATH_H) && !defined(_MATH_H_)
#ifdef __cplusplus
struct __exception
#else
struct exception
#endif
{
	int			type;	/* exception type */
	const char	*name;	/* function in which it occured */
	double		arg1;	/* an arg */
	double		arg2;	/* another arg */
	double		retval; /* val to return */
};
#endif

#undef HUGE
#define	HUGE		MAXFLOAT

#include "k_stand.h"

/* All floating-point numbers can be put in one of these categories.  */
#ifndef FP_NAN
enum
{
  FP_NAN,
# define FP_NAN FP_NAN
  FP_INFINITE,
# define FP_INFINITE FP_INFINITE
  FP_ZERO,
# define FP_ZERO FP_ZERO
  FP_SUBNORMAL,
# define FP_SUBNORMAL FP_SUBNORMAL
  FP_NORMAL
# define FP_NORMAL FP_NORMAL
};
#endif

/*
 * set X_TLOSS = pi*2**52, which is possibly defined in <values.h>
 * (one may replace the following line by "#include <values.h>")
 */
#undef X_TLOSS
#define X_TLOSS	1.41484755040568800000e+16


/* Most routines need to check whether a float is finite, infinite, or not a
   number, and many need to know whether the result of an operation will
   overflow.  These conditions depend on whether the largest exponent is
   used for NaNs & infinities, or whether it's used for finite numbers.  The
   macros below wrap up that kind of information:

   FLT_UWORD_IS_FINITE(X)
	True if a positive float with bitmask X is finite.

   FLT_UWORD_IS_NAN(X)
	True if a positive float with bitmask X is not a number.

   FLT_UWORD_IS_INFINITE(X)
	True if a positive float with bitmask X is +infinity.

   FLT_UWORD_MAX
	The bitmask of FLT_MAX.

   FLT_UWORD_HALF_MAX
	The bitmask of FLT_MAX/2.

   FLT_UWORD_EXP_MAX
	The bitmask of the largest finite exponent (129 if the largest
	exponent is used for finite numbers, 128 otherwise).

   FLT_UWORD_LOG_MAX
	The bitmask of log(FLT_MAX), rounded down.  This value is the largest
	input that can be passed to exp() without producing overflow.

   FLT_UWORD_LOG_2MAX
	The bitmask of log(2*FLT_MAX), rounded down.  This value is the
	largest input than can be passed to cosh() without producing
	overflow.

   FLT_LARGEST_EXP
	The largest biased exponent that can be used for finite numbers
	(255 if the largest exponent is used for finite numbers, 254
	otherwise) */

#ifdef _FLT_LARGEST_EXPONENT_IS_NORMAL
#define FLT_UWORD_IS_FINITE(x) 1
#define FLT_UWORD_IS_NAN(x) 0
#define FLT_UWORD_IS_INFINITE(x) 0
#define FLT_UWORD_MAX IC(0x7fffffff)
#define FLT_UWORD_HALF_MAX IC(0x7f7fffff)
#define FLT_UWORD_EXP_MAX IC(0x43010000)
#define FLT_UWORD_LOG_MAX IC(0x42b2d4fc)
#define FLT_UWORD_LOG_2MAX IC(0x42b437e0)
/* #define HUGE ((float)0X1.FFFFFEP128) */
#else
#define FLT_UWORD_IS_FINITE(x) ((x) < IC(0x7f800000))
#define FLT_UWORD_IS_NAN(x) ((x) > IC(0x7f800000))
#define FLT_UWORD_IS_INFINITE(x) ((x) == IC(0x7f800000))
#define FLT_UWORD_MAX IC(0x7f7fffff)
#define FLT_UWORD_HALF_MAX IC(0x7effffff)
#define FLT_UWORD_EXP_MAX IC(0x43000000)
#define FLT_UWORD_LOG_MAX IC(0x42b17217)
#define FLT_UWORD_LOG_2MAX IC(0x42b2d4fc)
/* #define HUGE ((float)3.40282346638528860e+38) */
#endif
#define FLT_LARGEST_EXP (FLT_UWORD_MAX>>23)

/* Many routines check for zero and subnormal numbers.  Such things depend
   on whether the target supports denormals or not:

   FLT_UWORD_IS_ZERO(X)
	True if a positive float with bitmask X is +0.	Without denormals,
	any float with a zero exponent is a +0 representation.	With
	denormals, the only +0 representation is a 0 bitmask.

   FLT_UWORD_IS_SUBNORMAL(X)
	True if a non-zero positive float with bitmask X is subnormal.
	(Routines should check for zeros first.)

   FLT_UWORD_MIN
	The bitmask of the smallest float above +0.  Call this number
	REAL_FLT_MIN...

   FLT_UWORD_EXP_MIN
	The bitmask of the float representation of REAL_FLT_MIN's exponent.

   FLT_UWORD_LOG_MIN
	The bitmask of |log(REAL_FLT_MIN)|, rounding down.

   FLT_SMALLEST_EXP
	REAL_FLT_MIN's exponent - EXP_BIAS (1 if denormals are not supported,
	-22 if they are).
*/

#ifdef _FLT_NO_DENORMALS
#define FLT_UWORD_IS_ZERO(x) ((x) < IC(0x00800000))
#define FLT_UWORD_IS_SUBNORMAL(x) 0
#define FLT_UWORD_MIN IC(0x00800000)
#define FLT_UWORD_EXP_MIN IC(0x42fc0000)
#define FLT_UWORD_LOG_MIN IC(0x42aeac50)
#define FLT_SMALLEST_EXP 1
#else
#define FLT_UWORD_IS_ZERO(x) ((x) == 0)
#define FLT_UWORD_IS_SUBNORMAL(x) ((x) < IC(0x00800000))
#define FLT_UWORD_MIN IC(0x00000001)
#define FLT_UWORD_EXP_MIN IC(0x43160000)
#define FLT_UWORD_LOG_MIN IC(0x42cff1b5)
#define FLT_SMALLEST_EXP -22
#endif


#ifndef DOMAIN
# define	DOMAIN		1
# define	SING		2
# define	OVERFLOW	3
# define	UNDERFLOW	4
# define	TLOSS		5
# define	PLOSS		6
#endif

#if defined(__GNUC__) && defined (__mc68020__) && !defined (__mcpu32__)
static __inline__ int32_t count_leading_zeros(uint32_t x)
{
	int32_t count;
	__asm__ ("bfffo %1{%b2:%b2},%0"
		: "=d" (count)
		: "od" (x), "n" (0));
	if (count == 32) count = 0;
	return count;
}
#elif defined(__GNUC__)
static __inline__ int32_t count_leading_zeros(uint32_t x)
{
	return __builtin_clz(x);
}
#else
static int32_t count_leading_zeros(uint32_t x)
{
	uint32_t __xr = (x);
    uint32_t __a, count;

static unsigned char const __clz_tab[256] =
{
  0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
  6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
};

	__a = __xr < ((uint32_t)1<<16)
	  ? (__xr < ((uint32_t)8) ? 0 : 8)
	  : (__xr < ((uint32_t)1<<24) ? 16 : 24);
	count = 32 - (__clz_tab[__xr >> __a] + __a);
	if (count == 32) count = 0;
	return count;
}
#endif

#undef isinf
#undef isnan
#undef signbit
#undef isfinite
#undef issignaling
#undef isnormal

/*
 * ANSI/POSIX
 */
extern double acos (double);
extern double asin (double);
extern double atan (double);
extern double atan2 (double, double);
extern double cos (double);
extern double sin (double);
extern double tan (double);

extern double cosh (double);
extern double sinh (double);
extern double tanh (double);

extern double exp (double);
extern double frexp (double, int *);
extern double ldexp (double, int);
extern double log (double);
extern double log10 (double);
extern double modf (double, double *);

extern double pow (double, double);
extern double sqrt (double);

extern double ceil (double);
extern double fabs (double);
extern double floor (double);
extern double fmod (double, double);

extern double erf (double);
extern double erfc (double);
extern double gamma (double);
extern double hypot (double, double);
extern int isnan (double);
extern int finite (double);
extern double j0 (double);
extern double j1 (double);
extern double jn (int, double);
extern double lgamma (double);
extern double y0 (double);
extern double y1 (double);
extern double yn (int, double);

extern double acosh (double);
extern double asinh (double);
extern double atanh (double);
extern double cbrt (double);
extern double logb (double);
extern double nextafter (double, double);
extern double remainder (double, double);
#ifdef __mc68000__
#undef _SCALB_INT
#else
#undef _SCALB_INT
#endif
#ifdef _SCALB_INT
extern double scalb (double, int);
#else
extern double scalb (double, double);
#endif

extern int matherr (struct exception *);

# ifdef __NO_LONG_DOUBLE_MATH
#  define isnan(x) \
     (sizeof (x) == sizeof (float) ? __isnanf (x) : __isnan (x))
# else
#  define isnan(x) \
     (sizeof (x) == sizeof (float)					      \
      ? __isnanf (x)							      \
      : sizeof (x) == sizeof (double)					      \
      ? __isnan (x) : __isnanl (x))
# endif

/* Return nonzero value if X is positive or negative infinity.  */
# ifdef __NO_LONG_DOUBLE_MATH
#  define isinf(x) \
     (sizeof (x) == sizeof (float) ? __isinff (x) : __isinf (x))
# else
#  define isinf(x) \
     (sizeof (x) == sizeof (float)					      \
      ? __isinff (x)							      \
      : sizeof (x) == sizeof (double)					      \
      ? __isinf (x) : __isinfl (x))
# endif

/* Return nonzero value if sign of X is negative.  */
# ifdef __NO_LONG_DOUBLE_MATH
#  define signbit(x) \
     (sizeof (x) == sizeof (float) ? __signbitf (x) : __signbit (x))
# else
#  define signbit(x) \
     (sizeof (x) == sizeof (float)					      \
      ? __signbitf (x)							      \
      : sizeof (x) == sizeof (double)					      \
      ? __signbit (x) : __signbitl (x))
# endif

/* Return nonzero value if X is not +-Inf or NaN.  */
# ifdef __NO_LONG_DOUBLE_MATH
#  define isfinite(x) \
     (sizeof (x) == sizeof (float) ? __finitef (x) : __finite (x))
# else
#  define isfinite(x) \
     (sizeof (x) == sizeof (float)					      \
      ? __finitef (x)							      \
      : sizeof (x) == sizeof (double)					      \
      ? __finite (x) : __finitel (x))
# endif

/* Return nonzero value if X is neither zero, subnormal, Inf, nor NaN.  */
# define isnormal(x) (fpclassify (x) == FP_NORMAL)

# ifdef __NO_LONG_DOUBLE_MATH
#  define issignaling(x) \
     (sizeof (x) == sizeof (float) ? __issignalingf (x) : __issignaling (x))
# else
#  define issignaling(x) \
     (sizeof (x) == sizeof (float)					      \
      ? __issignalingf (x)						      \
      : sizeof (x) == sizeof (double)					      \
      ? __issignaling (x) : __issignalingl (x))
# endif

/*
 * IEEE Test Vector
 */
extern double significand (double);

/*
 * Functions callable from C, intended to support IEEE arithmetic.
 */
extern double copysign (double, double);
extern int ilogb (double);
extern double rint (double);
extern double scalbn (double, int);

/*
 * BSD math library entry points
 */
extern double expm1 (double);
extern double log1p (double);

/*
 * Reentrant version of gamma & lgamma; passes signgam back by reference
 * as the second argument; user must allocate space for signgam.
 */
#ifdef _REENTRANT
extern double gamma_r (double, int *);
extern double lgamma_r (double, int *);
#endif	/* _REENTRANT */

#undef __MATH_INLINE
#define __MATH_INLINE static __inline

#if defined(__GNUC__) && defined(__mcoldfire__)
# include "asm/ieee754/m68k_cf.h"
#elif defined(__GNUC__) && defined(__mc68000__)
# include "asm/ieee754/m68k.h"
#elif defined(__AHCC__)
# include "asm/ieee754/m68k_ahcc.h"
#elif defined(__PUREC__)
# include "asm/ieee754/m68k_pc.h"
#elif defined(__GNUC__) && defined(__i386__)
# include "asm/ieee754/i386.h"
#elif defined(__GNUC__) && defined(__x86_64__)
# include "asm/ieee754/x86_64.h"
#endif

/* ieee style elementary functions */
#ifndef __have_fpu_sin
double      __ieee754_sin (double x);
float       __ieee754_sinf (float x);
long double __ieee754_sinl (long double x);
#endif
#ifndef __have_fpu_cos
double      __ieee754_cos (double x);
float       __ieee754_cosf (float x);
long double __ieee754_cosl (long double x);
#endif
#ifndef __have_fpu_sincos
void        __ieee754_sincos (double __x, double *__sin, double *__cos);
void        __ieee754_sincosf (float __x, float *__sin, float *__cos);
void        __ieee754_sincosl (long double __x, long double *__sin, long double *__cos);
#endif
#ifndef __have_fpu_tan
double      __ieee754_tan (double x);
float       __ieee754_tanf (float x);
long double __ieee754_tanl (long double x);
#endif
#ifndef __have_fpu_asin
double      __ieee754_asin (double x);
float       __ieee754_asinf (float x);
long double __ieee754_asinl (long double x);
#endif
#ifndef __have_fpu_acos
double      __ieee754_acos (double x);
float       __ieee754_acosf (float x);
long double __ieee754_acosl (long double x);
#endif
#ifndef __have_fpu_atan
double      __ieee754_atan (double x);
float       __ieee754_atanf (float x);
long double __ieee754_atanl (long double x);
#endif
#ifndef __have_fpu_atan2
double      __ieee754_atan2 (double y, double x);
float       __ieee754_atan2f (float y, float x);
long double __ieee754_atan2l (long double y, long double x);
#endif
#ifndef __have_fpu_sinh
double      __ieee754_sinh (double x);
float       __ieee754_sinhf (float x);
long double __ieee754_sinhl (long double x);
#endif
#ifndef __have_fpu_cosh
double      __ieee754_cosh (double x);
float       __ieee754_coshf (float x);
long double __ieee754_coshl (long double x);
#endif
#ifndef __have_fpu_tanh
double      __ieee754_tanh (double x);
float       __ieee754_tanhf (float x);
long double __ieee754_tanhl (long double x);
#endif
#ifndef __have_fpu_acosh
double      __ieee754_acosh(double x);
float       __ieee754_acoshf(float x);
long double __ieee754_acoshl(long double x);
#endif
#ifndef __have_fpu_asinh
double      __ieee754_asinh(double x);
float       __ieee754_asinhf(float x);
long double __ieee754_asinhl(long double x);
#endif
#ifndef __have_fpu_atanh
double      __ieee754_atanh (double x);
float       __ieee754_atanhf (float x);
long double __ieee754_atanhl (long double x);
#endif
#ifndef __have_fpu_exp
double      __ieee754_exp (double x);
float       __ieee754_expf (float x);
long double __ieee754_expl (long double x);
#endif
#ifndef __have_fpu_expm1
double      __ieee754_expm1 (double x);
float       __ieee754_expm1f (float x);
long double __ieee754_expm1l (long double x);
#endif
#ifndef __have_fpu_exp2
double      __ieee754_exp2 (double x);
float       __ieee754_exp2f (float x);
long double __ieee754_exp2l (long double x);
#endif
#ifndef __have_fpu_pow2
double      __ieee754_pow2 (double x);
float       __ieee754_pow2f (float x);
long double __ieee754_pow2l (long double x);
#endif
#ifndef __have_fpu_exp10
double      __ieee754_exp10 (double x);
float       __ieee754_exp10f (float x);
long double __ieee754_exp10l (long double x);
#endif
#ifndef __have_fpu_pow10
double      __ieee754_pow10 (double x);
float       __ieee754_pow10f (float x);
long double __ieee754_pow10l (long double x);
#endif
#ifndef __have_fpu_log
double      __ieee754_log (double x);
float       __ieee754_logf (float x);
long double __ieee754_logl (long double x);
#endif
#ifndef __have_fpu_log1p
double      __ieee754_log1p (double x);
float       __ieee754_log1pf (float x);
long double __ieee754_log1pl (long double x);
#endif
#ifndef __have_fpu_log10
double      __ieee754_log10 (double x);
float       __ieee754_log10f (float x);
long double __ieee754_log10l (long double x);
#endif
#ifndef __have_fpu_log2
double      __ieee754_log2 (double x);
float       __ieee754_log2f (float x);
long double __ieee754_log2l (long double x);
#endif
#ifndef __have_fpu_sqrt
double      __ieee754_sqrt (double x);
float       __ieee754_sqrtf (float x);
long double __ieee754_sqrtl (long double x);
#endif
#ifndef __have_fpu_hypot
double      __ieee754_hypot (double x, double y);
float       __ieee754_hypotf (float x, float y);
long double __ieee754_hypotl (long double x, long double y);
#endif
#ifndef __have_fpu_pow
double      __ieee754_pow (double x, double y);
float       __ieee754_powf (float x, float y);
long double __ieee754_powl (long double x, long double y);
#endif
double      __ieee754_powi (double x, int y);
float       __ieee754_powif (float x, int y);
long double __ieee754_powil (long double x, int y);
#ifndef __have_fpu_fabs
double      __ieee754_fabs (double x);
float       __ieee754_fabsf (float x);
long double __ieee754_fabsl (long double x);
#endif
#ifndef __have_fpu_ceil
double      __ieee754_ceil (double x);
float       __ieee754_ceilf (float x);
long double __ieee754_ceill (long double x);
#endif
#ifndef __have_fpu_floor
double      __ieee754_floor (double x);
float       __ieee754_floorf (float x);
long double __ieee754_floorl (long double x);
#endif
#ifndef __have_fpu_trunc
double      __ieee754_trunc (double x);
float       __ieee754_truncf (float x);
long double __ieee754_truncl (long double x);
#endif
#ifndef __have_fpu_rint
double      __ieee754_rint (double x);
float       __ieee754_rintf (float x);
long double __ieee754_rintl (long double x);
#endif
#ifndef __have_fpu_lrint
long int    __ieee754_lrint(double x);
long int    __ieee754_lrintf(float x);
long int    __ieee754_lrintl(long double x);
#endif
#ifndef __have_fpu_llrint
#ifndef __NO_LONGLONG
long long int    __ieee754_llrint(double x);
long long int    __ieee754_llrintf(float x);
long long int    __ieee754_llrintl(long double x);
#endif
#endif
#ifndef __have_fpu_round
double      __ieee754_round (double x);
float       __ieee754_roundf (float x);
long double __ieee754_roundl (long double x);
#endif
#ifndef __have_fpu_lround
long int    __ieee754_lround (double x);
long int    __ieee754_lroundf (float x);
long int    __ieee754_lroundl (long double x);
#endif
#ifndef __have_fpu_llround
#ifndef __NO_LONGLONG
long long int    __ieee754_llround (double x);
long long int    __ieee754_llroundf (float x);
long long int    __ieee754_llroundl (long double x);
#endif
#endif
#ifndef __have_fpu_fmod
double      __ieee754_fmod (double x, double y);
float       __ieee754_fmodf (float x, float y);
long double __ieee754_fmodl (long double x, long double y);
#endif
#ifndef __have_fpu_drem
double      __ieee754_drem (double x, double y);
float       __ieee754_dremf (float x, float y);
long double __ieee754_dreml (long double x, long double y);
#endif
#ifndef __have_fpu_remainder
double      __ieee754_remainder (double x, double y);
float       __ieee754_remainderf (float x, float y);
long double __ieee754_remainderl (long double x, long double y);
#endif
#ifndef __have_fpu_remquo
double      __ieee754_remquo (double x, double y, int *quo);
float       __ieee754_remquof (float x, float y, int *quo);
long double __ieee754_remquol (long double x, long double y, int *quo);
#endif
#ifndef __have_fpu_scalb
#ifdef _SCALB_INT
double      __ieee754_scalb (double x, int n);
float       __ieee754_scalbf (float x, int n);
long double __ieee754_scalbl (long double x, int n);
#else
double      __ieee754_scalb (double x, double n);
float       __ieee754_scalbf (float x, float n);
long double __ieee754_scalbl (long double x, long double n);
#endif
#endif
#ifndef __have_fpu_scalbn
double      __ieee754_scalbn (double x, int n);
float       __ieee754_scalbnf (float x, int n);
long double __ieee754_scalbnl (long double x, int n);
#endif
#ifndef __have_fpu_scalbln
double      __ieee754_scalbln (double x, long n);
float       __ieee754_scalblnf (float x, long n);
long double __ieee754_scalblnl (long double x, long n);
#endif
#ifndef __have_fpu_logb
double      __ieee754_logb (double x);
float       __ieee754_logbf (float x);
long double __ieee754_logbl (long double x);
#endif
#ifndef __have_fpu_ilogb
int __ieee754_ilogb (double x);
int __ieee754_ilogbf (float x);
int __ieee754_ilogbl (long double x);
#endif
#ifndef __have_fpu_ldexp
double      __ieee754_ldexp (double x, int n);
float       __ieee754_ldexpf (float x, int n);
long double __ieee754_ldexpl (long double x, int n);
#endif
#ifndef __have_fpu_frexp
double      __ieee754_frexp (double x, int *exp);
float       __ieee754_frexpf (float x, int *exp);
long double __ieee754_frexpl (long double x, int *exp);
#endif
#ifndef __have_fpu_significand
double      __ieee754_significand(double x);
float       __ieee754_significandf(float x);
long double __ieee754_significandl(long double x);
#endif
#ifndef __have_fpu_modf
double      __ieee754_modf (double x, double *ip);
float       __ieee754_modff (float x, float *ip);
long double __ieee754_modfl (long double x, long double *ip);
#endif

double      __ieee754_lgamma_r(double x, int *sign);
float       __ieee754_lgammaf_r(float x, int *sign);
long double __ieee754_lgammal_r(long double x, int *sign);
double      __ieee754_tgamma_r(double x, int *sign);
float       __ieee754_tgammaf_r(float x, int *sign);
long double __ieee754_tgammal_r(long double x, int *sign);

double __ieee754_j0(double x);
float __ieee754_j0f(float x);
long double __ieee754_j0l(long double x);
double __ieee754_y0(double x);
float __ieee754_y0f(float x);
long double __ieee754_y0l(long double x);

double __ieee754_j1(double x);
float __ieee754_j1f(float x);
long double __ieee754_j1l(long double x);
double __ieee754_y1(double x);
float __ieee754_y1f(float x);
long double __ieee754_y1l(long double x);

double __ieee754_jn(int n, double x);
float __ieee754_jnf(int n, float x);
long double __ieee754_jnl(int n, long double x);
double __ieee754_yn(int n, double x);
float __ieee754_ynf(int n, float x);
long double __ieee754_ynl(int n, long double x);

#ifndef math_opt_barrier
# define math_opt_barrier(x) \
  ({ __typeof (x) __x = (x); __asm ("" : "+m" (__x)); __x; })
# define math_force_eval(x) \
  ({ __typeof (x) __x = (x); __asm __volatile__ ("" : : "m" (__x)); })
#endif

#ifndef __have_fpu_fabs
#define __ieee754_fabs(x) fabs(x)
#define __ieee754_fabsf(x) fabsf(x)
#define __ieee754_fabsl(x) fabsl(x)
#endif

#ifndef FP_ILOGB0
/* The values returned by `ilogb' for 0 and NaN respectively.  */
# define FP_ILOGB0	(-INT_MAX - 1)
# define FP_ILOGBNAN	(INT_MAX)
#endif

#define SAVE_AND_SET_ROUND(RM) \
  int __round, __oldround; \
  __round = (RM); \
  __oldround = fegetround(); \
  if (__round != __oldround) \
  { \
  	 fesetround(__round); \
  }
#define RESTORE_ROUND() \
  if (__round != __oldround) fesetround(__oldround)

/* fdlibm kernel function */
double __kernel_sin(double x, double y, int iy);
float __kernel_sinf(float x, float y, int iy);
long double __kernel_sinl(long double x, long double y, int iy);

double __kernel_cos(double x, double y);
float __kernel_cosf(float x, float y);
long double __kernel_cosl(long double x, long double y);

double __kernel_tan(double x, double y, int iy);
float __kernel_tanf(float x, float y, int iy);
long double __kernel_tanl(long double x, long double y, int iy);

int32_t __ieee754_rem_pio2(double x, double *y);
int32_t __ieee754_rem_pio2f(float x, float *y);
int32_t __ieee754_rem_pio2l(long double x, long double *y);

int32_t __kernel_rem_pio2(double *x, double *y, int32_t e0, int32_t nx, int prec);
int32_t __kernel_rem_pio2f(float *x, float *y, int32_t e0, int32_t nx, int prec);
int32_t __kernel_rem_pio2l(double *x, double *y, int32_t e0, int32_t nx, int prec);

double __j0_y0_pzero(double);
double __j0_y0_qzero(double);
float __j0_y0_pzerof(float);
float __j0_y0_qzerof(float);
long double __j0_y0_pzerol(long double);
long double __j0_y0_qzerol(long double);

double __j1_y1_pone(double);
double __j1_y1_qone(double);
float __j1_y1_ponef(float);
float __j1_y1_qonef(float);
long double __j1_y1_ponel(long double);
long double __j1_y1_qonel(long double);

double __x2y2m1(double x, double y);
float __x2y2m1f(float x, float y);
long double __x2y2m1l(long double x, long double y);

extern const long double __sincosl_table[];

/* NYI */
#define feraiseexcept(x)

#endif /* __FDLIBM_H__ */
