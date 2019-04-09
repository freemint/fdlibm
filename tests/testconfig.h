#ifndef __TESTCONFIG_H__
#define __TESTCONFIG_H__ 1

#define _GNU_SOURCE

#include "config.h"
#include <float.h>

#ifdef LIBCMINI
#define CR "\r"
#else
#define CR
#endif

/*
 * set to 1 to allow for small differences in the last few
 * bits of floating point values,
 * since at least the JIT compiler cannot guarantee bit exactness
 * in all cases.
 * Theoretically, when running the tests on real hardware,
 * this should be set to 0. However, it turns out that different types
 * of FPUs return slightly different values, so getting this exactly
 * right for all cases is almost impossible.
 */
#define ALLOW_INEXACT 1

/*
 * set to 1 to allow for testing m68k minimum subnormal value.
 * Problem with this is that this value cannot be represented
 * by x87.
 * When using a softfloat emulation, or running the tests on real hardware,
 * this should be set to 1.
 * Note: if this is enabled, it will be disabled when a runtime check
 * can be performed to detect the cpu type, and it is '040 or '060
 * (currently only done for the amiga version).
 */
#define ALLOW_SUBNORM_MIN 0


/*
 * For the same reason as above, m68k minval cannot be represent
 * in x87.
 * When using a softfloat emulation, or running the tests on real hardware,
 * this should be set to 1
 */
#define ALLOW_MINVAL 0


/*
 * number of iterations each single test is run.
 * mainly intended to get the code compiled by JIT
 */
#ifndef JIT_LOOPS
#  define JIT_LOOPS 128
#endif




/*
 * remaining stuff should normally not needed to be changed
 */

#ifndef __ORDER_BIG_ENDIAN__
#  define __ORDER_BIG_ENDIAN__ 4321
#endif

#ifdef __PUREC__
#  define __mc68000__ 1
#  define __BYTE_ORDER__ __ORDER_BIG_ENDIAN__
#endif

#ifndef __BYTE_ORDER__
  #error "__BYTE_ORDER__ not defined"
#endif

#ifndef __mc68000__
#define __HAVE_68881__
#endif

#if (defined(__68881__) || defined(_M68881) || defined(__M68881__) || defined(__FPU__)) && !defined(__HAVE_68881__)
#define __HAVE_68881__
#endif

#ifndef __USER_LABEL_PREFIX__
#  if defined(__PUREC__) || defined(__ELF__)
#    define __USER_LABEL_PREFIX__
#  else
#    define __USER_LABEL_PREFIX__ _
#  endif
#endif

#ifndef __SYMBOL_PREFIX
# define __SYMBOL_PREFIX __STRINGIFY(__USER_LABEL_PREFIX__)
#endif
#ifndef __ASM_SYMBOL_PREFIX
# define __ASM_SYMBOL_PREFIX __USER_LABEL_PREFIX__
#endif

#ifndef C_SYMBOL_NAME
# define C_SYMBOL_NAME(name) __SYMBOL_PREFIX #name
#endif

#ifndef __STRINGIFY
#define __STRING(x)	#x
#define __STRINGIFY(x)	__STRING(x)
#endif

#include <math.h>
#include <sys/types.h>
#include <stdint.h>

#ifdef TEST_DOUBLE
#define REAL double
#define __MATH_PRECNAME(name) name
#endif
#ifdef TEST_FLOAT
#define REAL float
#define __MATH_PRECNAME(name) name##f
#endif
#ifdef TEST_LDOUBLE
#define REAL long double
#define __MATH_PRECNAME(name) name##l
#endif
#ifndef REAL
#error "don't know what to test"
#endif

/*
 * ColdFire has no long double format
 */
#if DBL_MANT_DIG == LDBL_MANT_DIG && !defined(__NO_LONG_DOUBLE_MATH)
#  define __NO_LONG_DOUBLE_MATH 1
#endif

#endif /* __TESTCONFIG_H__ */
