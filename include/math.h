/*  math.h -- MiNTLib.
    Copyright (C) 2019 Thorsten Otto <admin@tho-otto.de>

    This file is part of the MiNTLib project, and may only be used
    modified and distributed under the terms of the MiNTLib project
    license, COPYMINT.  By continuing to use, modify, or distribute
    this file you indicate that you have read the license and
    understand and accept it fully.
*/

/*
 *	ISO C99 Standard: 7.12 Mathematics	<math.h>
 */

#ifndef _MATH_H
#define _MATH_H 1

#ifndef	_FEATURES_H
# include <features.h>
#endif

#if (defined(__HAVE_68881__) || defined(__FPU__) || (defined(__mcoldfire__) && defined(__mcffpu__))) && !defined(__HAVE_FPU__)
#  define __HAVE_FPU__ 1
#endif

/*
 * PureC uses __NFPUIN__ to inhibit math inline intrinsics
 */
#if defined(__NFPUIN__) && !defined(__NO_MATH_INLINES)
#  define __NO_MATH_INLINES
#endif

/*
 * ColdFire has no long double format
 */
#if defined(__mcoldfire__) && !defined(__NO_LONG_DOUBLE_MATH)
#  define __NO_LONG_DOUBLE_MATH 1
#endif

/*
 * GNU-C uses 64-bit double format, and 96-bit for long double.
 * PureC uses 64-bit double format for ColdFire (AHCC only),
         80-bit for soft-float and 96-bit for hard-float,
         and the same format also for long double.
 * so:
 *    - ColdFire has no long double, and the long double versions
 *      are redirected to the double versions
 *    - PureC has no real double, all arithmetic is done using long double,
 *      but the format differs between soft-float and fpu versions.
 */
#if (defined(__mcoldfire__) || defined(__GNUC__)) && !defined(__DOUBLE_64__)
#  define __DOUBLE_64__ 1 /* double == long double == ieee754-64bit */
#endif
#if defined(__PUREC__) || defined(__TURBOC__)
#  define __DOUBLE_80__ 1 /* double == long double == ieee854-96bit */
#endif

__BEGIN_DECLS

/* Get definitions of __intmax_t and __uintmax_t.  */
#include <sys/types.h>

/* Get machine-dependent HUGE_VAL value (returned on overflow).
   On all IEEE754 machines, this is +Infinity.  */
#include <bits/huge_val.h>

#ifdef	 __USE_ISOC99
/* Get machine-dependent INFINITY value.  */
# include <bits/inf.h>

/* Get machine-dependent NAN value (returned for some domain errors).  */
# include <bits/nan.h>
#endif

/* Get general and ISO C9X specific information.  */
#include <bits/mathdef.h>


/* The file <bits/mathcalls.h> contains the prototypes for all the
   actual math functions.  These macros are used for those prototypes,
   so we can easily declare each function as both `name' and `__name',
   and can declare the float versions `namef' and `__namef'.  */

#define __MATHCALL(function,suffix, args)	\
  __MATHDECL (_Mdouble_,function,suffix, args)
#define __MATHDECL(type, function,suffix, args) \
  __MATHDECL_1(type, function,suffix, args); \
  __MATHDECL_1(type, __CONCAT(__,function),suffix, args)
#define __MATHCALLX(function,suffix, args, attrib)	\
  __MATHDECLX (_Mdouble_,function,suffix, args, attrib)
#define __MATHDECLX(type, function,suffix, args, attrib) \
  __MATHDECL_1(type, function,suffix, args) __attribute__ (attrib); \
  __MATHDECL_1(type, __CONCAT(__,function),suffix, args) __attribute__ (attrib)
#define __MATHDECL_1(type, function,suffix, args) \
  extern type __MATH_PRECNAME(function,suffix) args __THROW

#define _Mdouble_		double
#define __MATH_PRECNAME(name,r)	__CONCAT(name,r)
#define _Mdouble_BEGIN_NAMESPACE __BEGIN_NAMESPACE_STD
#define _Mdouble_END_NAMESPACE   __END_NAMESPACE_STD
#define __MATH_DECLARING_DOUBLE  1
#define __MATH_DECLARING_FLOATN  0
#include <bits/mathcall.h>
#undef	_Mdouble_
#undef	__MATH_PRECNAME
#undef __MATH_DECLARING_DOUBLE
#undef __MATH_DECLARING_FLOATN
#undef _Mdouble_BEGIN_NAMESPACE
#undef _Mdouble_END_NAMESPACE

#if defined __USE_MISC || defined __USE_ISOC99


/* Include the file of declarations again, this time using `float'
   instead of `double' and appending f to each function name.  */

# ifndef _Mfloat_
#  define _Mfloat_		float
# endif
# define _Mdouble_		_Mfloat_
# define __MATH_PRECNAME(name,r) name##f##r
# define _Mdouble_BEGIN_NAMESPACE __BEGIN_NAMESPACE_C99
# define _Mdouble_END_NAMESPACE   __END_NAMESPACE_C99
# define __MATH_DECLARING_DOUBLE  0
# define __MATH_DECLARING_FLOATN  0
# include <bits/mathcall.h>
# undef	_Mdouble_
# undef	__MATH_PRECNAME
# undef __MATH_DECLARING_DOUBLE
# undef __MATH_DECLARING_FLOATN
# undef _Mdouble_BEGIN_NAMESPACE
# undef _Mdouble_END_NAMESPACE

#  ifdef __LDBL_COMPAT

#   ifdef __USE_ISOC99
extern float __nldbl_nexttowardf (float __x, long double __y)
				  __THROW __attribute__ ((__const__));
#    ifdef __REDIRECT_NTH
extern float __REDIRECT_NTH (nexttowardf, (float __x, long double __y),
			     __nldbl_nexttowardf)
     __attribute__ ((__const__));
extern double __REDIRECT_NTH (nexttoward, (double __x, long double __y),
			      nextafter) __attribute__ ((__const__));
extern long double __REDIRECT_NTH (nexttowardl,
				   (long double __x, long double __y),
				   nextafter) __attribute__ ((__const__));
#    endif
#   endif

#   undef __MATHDECL_1
#   define __MATHDECL_2(type, function,suffix, args, alias) \
  extern type __REDIRECT_NTH(__MATH_PRECNAME(function,suffix), \
			     args, alias)
#   define __MATHDECL_1(type, function,suffix, args) \
  __MATHDECL_2(type, function,suffix, args, __CONCAT(function,suffix))
#  endif

/* Include the file of declarations again, this time using `long double'
   instead of `double' and appending l to each function name.  */

#  ifndef _Mlong_double_
#   define _Mlong_double_	long double
#  endif
#  define _Mdouble_		_Mlong_double_
#  define __MATH_PRECNAME(name,r) name##l##r
#  define _Mdouble_BEGIN_NAMESPACE __BEGIN_NAMESPACE_C99
#  define _Mdouble_END_NAMESPACE   __END_NAMESPACE_C99
#  define __MATH_DECLARING_DOUBLE  0
#  define __MATH_DECLARING_FLOATN  0
#  define __MATH_DECLARE_LDOUBLE   1
#  include <bits/mathcall.h>
#  undef _Mdouble_
#  undef __MATH_PRECNAME
#  undef __MATH_DECLARING_DOUBLE
#  undef __MATH_DECLARING_FLOATN
#  undef _Mdouble_BEGIN_NAMESPACE
#  undef _Mdouble_END_NAMESPACE

#endif	/* Use misc or ISO C99.  */
#undef	__MATHDECL_1
#undef	__MATHDECL
#undef	__MATHCALL


# define X_TLOSS	1.41484755040568800000e+16

#if !defined(__STRICT_ANSI__) || defined(__USE_PUREC)
typedef enum  {
    DOMAIN      = 1,
#define DOMAIN DOMAIN
    SING        = 2,
#define SING SING
    OVERFLOW    = 3,
#define OVERFLOW OVERFLOW
    UNDERFLOW   = 4,
#define UNDERFLOW UNDERFLOW
    TLOSS       = 5,
#define TLOSS TLOSS
    PLOSS       = 6
#define PLOSS PLOSS
} exception_type;
#ifdef __USE_PUREC
typedef exception_type mexcep;
#endif

#ifdef	__USE_MISC
/* Support for various different standard error handling behaviors.  */
typedef enum
{
  _IEEE_ = -1,	/* According to IEEE 754/IEEE 854.  */
  _SVID_,	/* According to System V, release 4.  */
  _XOPEN_,	/* Nowadays also Unix98.  */
  _POSIX_,
  _ISOC_	/* Actually this is ISO C99.  */
} _LIB_VERSION_TYPE;

/* This variable can be changed at run-time to any of the values above to
   affect floating point error handling behavior (it may also be necessary
   to change the hardware FPU exception settings).  */
extern _LIB_VERSION_TYPE _LIB_VERSION;
#endif


/* In SVID error handling, `matherr' is called with this description
   of the exceptional condition.

   We have a problem when using C++ since `exception' is a reserved
   name in C++.  */
/* do not change the comment above; it is looked up by GCCs fixincludes script */
#ifdef __cplusplus
struct __exception
#else
struct exception
#endif
{
	exception_type	type;	/* exception type */
	const char	*name;	/* function in which it occured */
	double		arg1;	/* an arg */
	double		arg2;	/* another arg */
	double		retval; /* val to return */
};
#endif /* __STRICT_ANSI__ */


/* Some useful constants.  */
#if defined __USE_BSD || defined __USE_XOPEN || defined __USE_PUREC
# define M_E		2.71828182845904523536	/* e */
# define M_LOG2E	1.44269504088896340736	/* log_2 e */
# define M_LOG10E	0.434294481903251827651	/* log_10 e */
# define M_LN2		0.693147180559945309417	/* log_e 2 */
# define M_LN10		2.30258509299404568402	/* log_e 10 */
# define M_PI		3.14159265358979323846	/* pi */
# define M_PI_2		1.57079632679489661923	/* pi/2 */
# define M_PI_4		0.785398163397448309116	/* pi/4 */
# define M_1_PI		0.318309886183790671538	/* 1/pi */
# define M_2_PI		0.636619772367581343076	/* 2/pi */
# define M_1_SQRTPI	0.564189583547756286948	/* 1/sqrt(pi) */
# define M_2_SQRTPI	1.12837916709551257390	/* 2/sqrt(pi) */
# define M_SQRT2	1.41421356237309504880	/* sqrt(2) */
# define M_SQRT1_2	0.707106781186547524401	/* 1/sqrt(2) */
#endif

/* The above constants are not adequate for computation using `long double's.
   Therefore we provide as an extension constants with similar names as a
   GNU extension.  Provide enough digits for the 128-bit IEEE quad.  */
#ifdef __USE_GNU
# define M_El		2.718281828459045235360287471352662498L /* e */
# define M_LOG2El	1.442695040888963407359924681001892137L /* log_2 e */
# define M_LOG10El	0.434294481903251827651128918916605082L /* log_10 e */
# define M_LN2l		0.693147180559945309417232121458176568L /* log_e 2 */
# define M_LN10l	2.302585092994045684017991454684364208L /* log_e 10 */
# define M_PIl		3.141592653589793238462643383279502884L /* pi */
# define M_PI_2l	1.570796326794896619231321691639751442L /* pi/2 */
# define M_PI_4l	0.785398163397448309615660845819875721L /* pi/4 */
# define M_1_PIl	0.318309886183790671537767526745028724L /* 1/pi */
# define M_2_PIl	0.636619772367581343075535053490057448L /* 2/pi */
# define M_2_SQRTPIl	1.128379167095512573896158903121545172L /* 2/sqrt(pi) */
# define M_SQRT2l	1.414213562373095048801688724209698079L /* sqrt(2) */
# define M_SQRT1_2l	0.707106781186547524400844362104849039L /* 1/sqrt(2) */
#endif


#define HUGE HUGE_VAL

#ifdef __GNUC__
# ifndef __cplusplus
#  ifndef max
#   define max(x,y) ({typeof(x) _x=(x); typeof(y) _y=(y); _x > _y ? _x : _y;})
#   define min(x,y) ({typeof(x) _x=(x); typeof(y) _y=(y); _x < _y ? _x : _y;})
#  endif
# endif
#endif

#if defined __USE_MISC || defined __USE_XOPEN
/* This variable is used by `gamma' and `lgamma'.  */
#define signgam (*__signgam())
extern int *__signgam (void);
#endif


/* ISO C99 defines some generic macros which work on any data type.  */
#ifdef __USE_ISOC99

/* Get the architecture specific values describing the floating-point
   evaluation.  The following symbols will get defined:

    float_t	floating-point type at least as wide as `float' used
		to evaluate `float' expressions
    double_t	floating-point type at least as wide as `double' used
		to evaluate `double' expressions

    FLT_EVAL_METHOD
		Defined to
		  0	if `float_t' is `float' and `double_t' is `double'
		  1	if `float_t' and `double_t' are `double'
		  2	if `float_t' and `double_t' are `long double'
		  else	`float_t' and `double_t' are unspecified

    INFINITY	representation of the infinity value of type `float'

    FP_FAST_FMA
    FP_FAST_FMAF
    FP_FAST_FMAL
		If defined it indicates that the `fma' function
		generally executes about as fast as a multiply and an add.
		This macro is defined only iff the `fma' function is
		implemented directly with a hardware multiply-add instructions.

    FP_ILOGB0	Expands to a value returned by `ilogb (0.0)'.
    FP_ILOGBNAN	Expands to a value returned by `ilogb (NAN)'.

    DECIMAL_DIG	Number of decimal digits supported by conversion between
		decimal and all internal floating-point formats.

*/

/* All floating-point numbers can be put in one of these categories.  */
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

/* Return number of classification appropriate for X.  */
# ifdef __NO_LONG_DOUBLE_MATH
#  define fpclassify(x) \
     (sizeof (x) == sizeof (float) ? __fpclassifyf (x) : __fpclassify (x))
# else
#  define fpclassify(x) \
     (sizeof (x) == sizeof (float)					      \
      ? __fpclassifyf (x)						      \
      : sizeof (x) == sizeof (double)					      \
      ? __fpclassify (x) : __fpclassifyl (x))
# endif

/* Return nonzero value if sign of X is negative.  */
# if __GNUC_PREREQ (6,0)
#  define signbit(x) __builtin_signbit (x)
# elif defined __cplusplus
  /* In C++ mode, __MATH_TG cannot be used, because it relies on
     __builtin_types_compatible_p, which is a C-only builtin.
     The check for __cplusplus allows the use of the builtin instead of
     __MATH_TG. This is provided for libstdc++, only to let its configure
     test work. No further use of this definition of signbit is expected
     in C++ mode, since libstdc++ provides its own version of signbit
     in cmath (which undefines signbit). */
#  define signbit(x) __builtin_signbitl (x)
# elif defined(__NO_LONG_DOUBLE_MATH)
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
# if (__GNUC_PREREQ (4,4) && !defined __SUPPORT_SNAN__)
#  define isnormal(x) __builtin_isnormal (x)
# else
#  define isnormal(x) (fpclassify (x) == FP_NORMAL)
# endif

/* Return nonzero value if X is subnormal.  */
# define issubnormal(x) (fpclassify (x) == FP_SUBNORMAL)

/* Return nonzero value if X is a NaN.  We could use `fpclassify' but
   we already have this functions `__isnan' and it is faster.  */
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

/* Bitmasks for the math_errhandling macro.  */
# define MATH_ERRNO	1	/* errno set by math functions.  */
# define MATH_ERREXCEPT	2	/* Exceptions raised by math functions.  */

/* By default all functions support both errno and exception handling.
   In gcc's fast math mode and if inline functions are defined this
   might not be true.  */
# ifndef __FAST_MATH__
#  define math_errhandling	(MATH_ERRNO | MATH_ERREXCEPT)
# endif

#endif /* Use ISO C99.  */


#ifdef __USE_GNU
/* Return nonzero value if X is a signaling NaN.  */
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
#endif /* Use GNU.  */

extern double atof (__const char *__nptr) __THROW;

#ifdef __USE_PUREC
int ftoa(double *__x, char *__resStr, int __ndig, int __format, int *__decPnt, int *__sign) __THROW;

void    setmatherr( int (*errorfunc)( struct exception *e ) );
int     fpumode( int disable );
void xdcnv(const double *val, void *rep8bytes);
void dxcnv(const void *rep8bytes, double *val);
double x80x96cnv( void *rep10bytes );
void   x96x80cnv( double rep12bytes, void *rep10bytes );

#if defined( __HAVE_68881__ )
double  fint( double __x );
double  fintrz( double __x );
double  fgetexp( double __x );
double  fgetman( double __x );
double  frem( double __x, double __y );
double  fsgldiv( double __x, double __y );
double  fsglmul( double __x, double __y );
double  fetoxm1( double __x );
double  flognp1( double __x );
#endif

#endif

#ifndef __STRICT_ANSI__

#ifndef dabs
extern double dabs (double) __THROW;
#if defined(__HAVE_FPU__) && !defined(__NO_MATH_INLINES)
# define dabs(x) fabs(x)
#endif
#endif

extern double copysign (double, double) __THROW;
extern float copysignf (float, float) __THROW;
extern long double copysignl (long double, long double) __THROW;

#ifdef __cplusplus
extern int matherr (struct __exception *) throw();
#else
extern int matherr (struct exception *) __THROW;
#endif

#endif /* __STRICT_ANSI__ */

#if defined __USE_ISOC99 && __GNUC_PREREQ(2,97)
/* ISO C99 defines some macros to compare number while taking care for
   unordered numbers.  Many FPUs provide special instructions to support
   these operations.  Generic support in GCC for these as builtins went
   in before 3.0.0, but not all cpus added their patterns.  We define
   versions that use the builtins here, and <bits/mathinline.h> will
   undef/redefine as appropriate for the specific GCC version in use.  */
# define isgreater(x, y)	__builtin_isgreater(x, y)
# define isgreaterequal(x, y)	__builtin_isgreaterequal(x, y)
# define isless(x, y)		__builtin_isless(x, y)
# define islessequal(x, y)	__builtin_islessequal(x, y)
# define islessgreater(x, y)	__builtin_islessgreater(x, y)
# define isunordered(u, v)	__builtin_isunordered(u, v)
#endif

#if defined(__HAVE_FPU__) && !defined(__NO_MATH_INLINES)
#if defined(__GNUC__) && !defined(__mcoldfire__)
/* this uses an updated version of GCC */
# include <bits/math-68881.h>
#elif defined(__GNUC__) && defined(__mcffpu__)
# include <bits/math-cffpu.h>
#elif defined(__AHCC__)
# include <ahcc/mathinl.h>
#elif defined(__PUREC__) || defined(__TURBOC__)
# include <purec/mathinl.h>
#endif
#endif

#ifdef __USE_ISOC99
/* If we've still got undefined comparison macros, provide defaults.  */

/* Return nonzero value if arguments are unordered.  */
# ifndef isunordered
#  define isunordered(u, v) (fpclassify (u) == FP_NAN || fpclassify (v) == FP_NAN)
# endif

/* Return nonzero value if X is greater than Y.  */
# ifndef isgreater
#ifdef __GNUC__
#  define isgreater(x, y) \
  (__extension__							      \
   ({ __typeof__(x) __x = (x); __typeof__(y) __y = (y);			      \
      !isunordered (__x, __y) && __x > __y; }))
#else
static int __isgreater(long double x, long double y)
{
	return !isunordered (x, y) && x > y;
}
#define isgreater(x, y) __isgreater(x, y)
#endif
# endif

/* Return nonzero value if X is greater than or equal to Y.  */
# ifndef isgreaterequal
#ifdef __GNUC__
#  define isgreaterequal(x, y) \
  (__extension__							      \
   ({ __typeof__(x) __x = (x); __typeof__(y) __y = (y);			      \
      !isunordered (__x, __y) && __x >= __y; }))
#else
static int __isgreaterequal(long double x, long double y)
{
	return !isunordered (x, y) && x >= y;
}
#define isgreaterequal(x, y) __isgreaterequal(x, y)
#endif
# endif

/* Return nonzero value if X is less than Y.  */
# ifndef isless
#ifdef __GNUC__
#  define isless(x, y) \
  (__extension__							      \
   ({ __typeof__(x) __x = (x); __typeof__(y) __y = (y);			      \
      !isunordered (__x, __y) && __x < __y; }))
#else
static int __isless(long double x, long double y)
{
	return !isunordered (x, y) && x < y;
}
#define isless(x, y) __isless(x, y)
#endif
# endif

/* Return nonzero value if X is less than or equal to Y.  */
# ifndef islessequal
#ifdef __GNUC__
#  define islessequal(x, y) \
  (__extension__							      \
   ({ __typeof__(x) __x = (x); __typeof__(y) __y = (y);			      \
      !isunordered (__x, __y) && __x <= __y; }))
#else
static int __islessequal(long double x, long double y)
{
	return !isunordered (x, y) && x <= y;
}
#define islessequal(x, y) __islessequal(x, y)
#endif
# endif

/* Return nonzero value if either X is less than Y or Y is less than X.  */
# ifndef islessgreater
#ifdef __GNUC__
#  define islessgreater(x, y) \
  (__extension__							      \
   ({ __typeof__(x) __x = (x); __typeof__(y) __y = (y);			      \
      !isunordered (__x, __y) && (__x < __y || __y < __x); }))
#else
static int __islessgreater(long double x, long double y)
{
	return !isunordered (x, y) && (x < y || x > y);
}
#define islessgreater(x, y) __islessgreater(x, y)
#endif
# endif

#endif

/* This function had a different name in the old libm.a */
#define drem remainder

__END_DECLS

#endif /* math.h  */
