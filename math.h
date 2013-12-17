/*
 * Copyright (c) 1985, 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)math.h	5.8 (Berkeley) 4/2/91
 */

#ifndef	_MATH_H_
#define	_MATH_H_

#ifdef __cplusplus
#define exception __math_exception
#endif

#include <float.h>

#ifndef BITS
#define BITS(type)	(8 * (int)sizeof(type))
#endif

#define	M_E		2.7182818284590452354	/* e */
#define	M_LOG2E		1.4426950408889634074	/* log 2e */
#define	M_LOG10E	0.43429448190325182765	/* log 10e */
#define	M_LN2		0.69314718055994530942	/* log e2 */
#define	M_LN10		2.30258509299404568402	/* log e10 */
#define	M_PI		3.14159265358979323846	/* pi */
#define	M_PI_2		1.57079632679489661923	/* pi/2 */
#define	M_PI_4		0.78539816339744830962	/* pi/4 */
#define	M_1_PI		0.31830988618379067154	/* 1/pi */
#define	M_2_PI		0.63661977236758134308	/* 2/pi */
#define	M_2_SQRTPI	1.12837916709551257390	/* 2/sqrt(pi) */
#define	M_SQRT2		1.41421356237309504880	/* sqrt(2) */
#define	M_SQRT1_2	0.70710678118654752440	/* 1/sqrt(2) */

#include <sys/cdefs.h>

enum fdversion {fdlibm_ieee = -1, fdlibm_svid, fdlibm_xopen, fdlibm_posix};

#define _LIB_VERSION_TYPE enum fdversion
#define _LIB_VERSION _fdlib_version  

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

#ifdef __USE_SVID
/* In SVID error handling, `matherr' is called with this description
   of the exceptional condition.

   We have a problem when using C++ since `exception' is a reserved
   name in C++.  */
#ifdef __cplusplus
struct __exception
#else
struct exception
#endif
{
	int type;
	char *name;
	double arg1;
	double arg2;
	double retval;
};

__BEGIN_DECLS
#ifdef __cplusplus
extern int matherr __P((struct __exception *)) throw ();
#else
extern int matherr __P((struct exception *));
#endif
__END_DECLS

/* 
 * set X_TLOSS = pi*2**52, which is possibly defined in <values.h>
 * (one may replace the following line by "#include <values.h>")
 */

#define X_TLOSS		1.41484755040568800000e+16 

#define	DOMAIN		1
#define	SING		2
#define	OVERFLOW	3
#define	UNDERFLOW	4
#define	TLOSS		5
#define	PLOSS		6

/* SVID mode specifies returning this large value instead of infinity.  */
# define HUGE		3.40282347e+38F

#else	/* !SVID */

# ifdef __USE_XOPEN
/* X/Open wants another strange constant.  */
#  define MAXFLOAT	3.40282347e+38F
# endif

#endif	/* SVID */

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

__BEGIN_DECLS
extern int fpclassify __P((double));

#ifndef isfinite
  #define isfinite(__y) \
          (__extension__ ({int __cy = fpclassify(__y); \
                           __cy != FP_INFINITE && __cy != FP_NAN;}))
#endif

extern int signgam;

extern double erf __P((double));
extern double erfc __P((double));
extern double gamma __P((double));
extern int isnan __P((double));
extern int finite __P((double));
extern double j0 __P((double));
extern double j1 __P((double));
extern double jn __P((int, double));
extern double lgamma __P((double));
extern double y0 __P((double));
extern double y1 __P((double));
extern double yn __P((int, double));
extern double acosh __P((double));
extern double asinh __P((double));
extern double cbrt __P((double));
extern double nextafter __P((double, double));
extern double remainder __P((double, double));
extern double significand __P((double));
extern double copysign __P((double, double));
extern int ilogb __P((double));
extern double scalbn __P((double, int));
extern double round __P((double));
extern float roundf __P((float));
extern long int lround __P((double x));
extern long int lroundf __P((float));

/*
 * Reentrant version of gamma & lgamma; passes signgam back by reference
 * as the second argument; user must allocate space for signgam.
 */
#ifdef _REENTRANT
extern double gamma_r __P((double, int *));
extern double lgamma_r __P((double, int *));
#endif	/* _REENTRANT */

/* ieee style elementary functions */
extern double __ieee754_sqrt __P((double));			
extern double __ieee754_acos __P((double));			
extern double __ieee754_acosh __P((double));			
extern double __ieee754_log __P((double));			
extern double __ieee754_atanh __P((double));			
extern double __ieee754_asin __P((double));			
extern double __ieee754_atan2 __P((double,double));			
extern double __ieee754_exp __P((double));
extern double __ieee754_cosh __P((double));
extern double __ieee754_fmod __P((double,double));
extern double __ieee754_pow __P((double,double));
extern double __ieee754_lgamma_r __P((double,int *));
extern double __ieee754_gamma_r __P((double,int *));
extern double __ieee754_lgamma __P((double));
extern double __ieee754_gamma __P((double));
extern double __ieee754_log10 __P((double));
extern double __ieee754_sinh __P((double));
extern double __ieee754_hypot __P((double,double));
extern double __ieee754_j0 __P((double));
extern double __ieee754_j1 __P((double));
extern double __ieee754_y0 __P((double));
extern double __ieee754_y1 __P((double));
extern double __ieee754_jn __P((int,double));
extern double __ieee754_yn __P((int,double));
extern double __ieee754_remainder __P((double,double));
extern int    __ieee754_rem_pio2 __P((double,double*));
#define _SCALB_INT
#ifdef _SCALB_INT
extern double __ieee754_scalb __P((double,int));
#else
extern double __ieee754_scalb __P((double,double));
#endif

/* fdlibm kernel function */
extern double __kernel_standard __P((double,double,int));	
extern double __kernel_sin __P((double,double,int));
extern double __kernel_cos __P((double,double));
extern double __kernel_tan __P((double,double,int));
extern int    __kernel_rem_pio2 __P((double*,double*,int,int,int,const int*));
__END_DECLS

#if (defined(__GNUC__) || defined(__cplusplus)) && defined(__HAVE_68881__)
#include <math-68881.h>
#else

#define	HUGE_VAL	1e500			/* IEEE: positive infinity */

__BEGIN_DECLS
extern double acos __P((double));
extern double asin __P((double));
extern double atan __P((double));
extern double atan2 __P((double, double));
extern double cos __P((double));
extern double sin __P((double));
extern double tan __P((double));
extern double cosh __P((double));
extern double sinh __P((double));
extern double tanh __P((double));
extern double exp __P((double));
extern double frexp __P((double, int *));
extern double ldexp __P((double, int));
extern double log __P((double));
extern double log10 __P((double));
extern double modf __P((double, double *));
extern double pow __P((double, double));
extern double sqrt __P((double));
extern double ceil __P((double));
extern float ceilf __P((float));
extern double fabs __P((double));
extern float fabsf __P((float));
extern double floor __P((double));
extern double fmod __P((double, double));
extern double hypot __P((double, double));
extern double atanh __P((double));
extern double logb __P((double));
#ifdef _SCALB_INT
extern double scalb __P((double, int));
#else
extern double scalb __P((double, double));
#endif
extern double rint __P((double));
extern double expm1 __P((double));
extern double log1p __P((double));
extern double fmin __P((double, double));
extern double fmax __P((double, double));
__END_DECLS

#endif /* __HAVE_68881__ */

__BEGIN_DECLS
/* These functions are defined in ixemul */
extern int     isinf __P((double));
extern int     isnan __P((double));
/* This function had a different name in the old libm.a */
#define drem remainder
__END_DECLS

#ifdef __cplusplus
#undef exception
#endif

#endif /* _MATH_H_ */
