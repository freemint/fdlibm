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

/* Sometimes it's necessary to define __LITTLE_ENDIAN explicitly
   but these catch some common cases. */

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define __HI(x) *(1+(int*)&x)
#define __LO(x) *(int*)&x
#define __HIp(x) *(1+(int*)x)
#define __LOp(x) *(int*)x
#else
#define __HI(x) *(int*)&x
#define __LO(x) *(1+(int*)&x)
#define __HIp(x) *(int*)x
#define __LOp(x) *(1+(int*)x)
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
  } parts;
} ieee_long_double_shape_type;

#endif

/* A union which permits us to convert between a float and a 32 bit
   int.  */

typedef union
{
  float value;
  uint32_t word;
} ieee_float_shape_type;


/* Get two 32 bit ints from a double.  */

#define EXTRACT_WORDS(ix0,ix1,d)				\
do {								\
  ieee_double_shape_type ew_u;					\
  ew_u.value = (d);						\
  (ix0) = ew_u.parts.msw;					\
  (ix1) = ew_u.parts.lsw;					\
} while (0)

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
  ieee_double_shape_type iw_u;                                  \
  iw_u.parts.msw = (ix0);                                       \
  iw_u.parts.lsw = (ix1);                                       \
  (d) = iw_u.value;                                             \
} while (0)

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

/* Macros to avoid undefined behaviour that can arise if the amount
   of a shift is exactly equal to the size of the shifted operand.  */

#define SAFE_LEFT_SHIFT(op,amt)                                 \
  (((amt) < 8 * sizeof(op)) ? ((op) << (amt)) : 0)

#define SAFE_RIGHT_SHIFT(op,amt)                                \
  (((amt) < 8 * sizeof(op)) ? ((op) >> (amt)) : 0)

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
enum fdversion {fdlibm_ieee = -1, fdlibm_svid, fdlibm_xopen, fdlibm_posix};

#define _LIB_VERSION_TYPE enum fdversion
#define _LIB_VERSION _fdlib_version  
#endif

/* if global variable _LIB_VERSION is not desirable, one may 
 * change the following to be a constant by: 
 *	#define _LIB_VERSION_TYPE const enum version
 * In that case, after one initializes the value _LIB_VERSION (see
 * s_lib_version.c) during compile time, it cannot be modified
 * in the middle of a program
 */ 
extern  _LIB_VERSION_TYPE  _LIB_VERSION;

#define _IEEE_  fdlibm_ieee
#define _SVID_  fdlibm_svid
#define _XOPEN_ fdlibm_xopen
#define _POSIX_ fdlibm_posix

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

#ifndef DOMAIN
# define	DOMAIN		1
# define	SING		2
# define	OVERFLOW	3
# define	UNDERFLOW	4
# define	TLOSS		5
# define	PLOSS		6
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
extern double __ieee754_sqrt (double);			
extern double __ieee754_acos (double);			
extern double __ieee754_acosh (double);			
extern double __ieee754_log (double);			
extern double __ieee754_atanh (double);			
extern double __ieee754_asin (double);			
extern double __ieee754_atan2 (double,double);			
extern double __ieee754_exp (double);
extern double __ieee754_cosh (double);
extern double __ieee754_fmod (double,double);
extern double __ieee754_pow (double,double);
extern double __ieee754_lgamma_r (double,int *);
extern double __ieee754_gamma_r (double,int *);
extern double __ieee754_lgamma (double);
extern double __ieee754_gamma (double);
extern double __ieee754_log10 (double);
extern double __ieee754_sinh (double);
extern double __ieee754_hypot (double,double);
extern double __ieee754_j0 (double);
extern double __ieee754_j1 (double);
extern double __ieee754_y0 (double);
extern double __ieee754_y1 (double);
extern double __ieee754_jn (int,double);
extern double __ieee754_yn (int,double);
extern double __ieee754_remainder (double,double);
extern int    __ieee754_rem_pio2 (double,double*);
#ifdef _SCALB_INT
extern double __ieee754_scalb (double,int);
#else
extern double __ieee754_scalb (double,double);
#endif

/* fdlibm kernel function */
extern double __kernel_standard (double,double,int);	
extern double __kernel_sin (double,double,int);
extern double __kernel_cos (double,double);
extern double __kernel_tan (double,double,int);
extern int    __kernel_rem_pio2 (double*,double*,int,int,int,const int*);

#endif /* __FDLIBM_H__ */
