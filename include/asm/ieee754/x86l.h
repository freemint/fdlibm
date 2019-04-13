int __x86_have_sse(void);

#if defined(__x86_64__) || defined(__SSE__)
#  define HAVE_SSE 1
#else
#  define HAVE_SSE __x86_have_sse()
#endif

/* Bit values returned by __x87_test.  */

#define __X87_COND_NAN		 0x0100
#define __X87_COND_NEG		 0x0200
#define __X87_COND_NORMAL	 0x0400
#define __X87_COND_ZERO		 0x4000
#define __X87_COND_INF		 (__X87_COND_NAN|__X87_COND_NORMAL)
#define __X87_COND_SUBNORMAL (__X87_COND_ZERO|__X87_COND_NORMAL)

#define __X87_COND_INVALID	 0x0400

#ifndef FE_ROUNDING_MASK
#define FE_ROUNDING_MASK (FE_TONEAREST|FE_DOWNWARD|FE_UPWARD|FE_TOWARDZERO)
#endif


#if defined __SSE2__
# define math_opt_barrier(x) \
  ({ __typeof(x) __x;					\
     if (sizeof (x) <= sizeof (double))			\
       __asm ("" : "=x" (__x) : "0" (x));		\
     else						\
       __asm ("" : "=t" (__x) : "0" (x));		\
     __x; })
# define math_force_eval(x) \
  do {							\
    if (sizeof (x) <= sizeof (double))			\
      __asm __volatile ("" : : "x" (x));		\
    else						\
      __asm __volatile ("" : : "f" (x));		\
  } while (0)
#elif defined(__i386__) || defined(__x86_64__)
# define math_opt_barrier(x) \
  ({ __typeof (x) __x;					\
     __asm ("" : "=t" (__x) : "0" (x));			\
     __x; })
# define math_force_eval(x) \
  do {							\
    __typeof (x) __x = (x);				\
    if (sizeof (x) <= sizeof (double))			\
      __asm __volatile ("" : : "m" (__x));		\
    else						\
      __asm __volatile ("" : : "f" (__x));		\
  } while (0)
#endif


#ifndef __NO_MATH_INLINES


#define __x87_test(__val, __fpsr) \
  __asm__ __volatile__("fxam; fstsw	%0" : "=am" (__fpsr) : "t" (__val))

#define __x87_getsw(__fpsr) \
  __asm__ __volatile__("fnstsw	%0" : "=am" (__fpsr))

#define __x87_getcw(cw) \
  __asm__ __volatile__("fnstcw	%0" : "=m" (cw))

#define __x87_setcw(cw) \
  __asm__ __volatile__("fldcw %0" : : "m" (cw))



__MATH_INLINE long double
__ieee754_fabsl (long double x)
{
	long double res;
	__asm__ ("fabs;" : "=t" (res) : "0" (x));
	return res;
}
#define __have_fpu_fabs

__MATH_INLINE long double
__ieee754_sinl(long double x)
{
	long double value;
	unsigned short x_class;
	
	/* sin(Inf or NaN) is NaN */
	__x87_test(x, x_class);
	if (x_class & __X87_COND_NAN)
		return x - x;
	__asm__ __volatile__(
		"fsin\n"
		"fnstsw	%%ax\n"
		"testl	$0x400,%%eax\n"
		"jz	2f\n"
		"fldpi\n"
		"fadd	%%st(0)\n"
		"fxch	%%st(1)\n"
	"1:	fprem1\n"
		"fnstsw	%%ax\n"
		"testl	$0x400,%%eax\n"
		"jnz	1b\n"
		"fstp	%%st(1)\n"
		"fsin\n"
	"2:\n"
		: "=t"(value)
		: "0"(x)
		: "st(1)", "eax");
	return value;
}
#define __have_fpu_sin

__MATH_INLINE long double
__ieee754_cosl(long double x)
{
	long double value;
	unsigned short x_class;
	
	/* cos(Inf or NaN) is NaN */
	__x87_test(x, x_class);
	if (x_class & __X87_COND_NAN)
		return x - x;
	__asm__ __volatile__(
		"fcos\n"
		"fnstsw	%%ax\n"
		"testl	$0x400,%%eax\n"
		"jz	2f\n"
		"fldpi\n"
		"fadd	%%st(0)\n"
		"fxch	%%st(1)\n"
	"1:	fprem1\n"
		"fnstsw	%%ax\n"
		"testl	$0x400,%%eax\n"
		"jnz	1b\n"
		"fstp	%%st(1)\n"
		"fcos\n"
	"2:\n"
		: "=t"(value)
		: "0"(x)
		: "st(1)", "eax");
	return value;
}
#define __have_fpu_cos

__MATH_INLINE void
__ieee754_sincosl(long double x, long double *sinx, long double *cosx)
{
	unsigned short x_class;
	
	__x87_test(x, x_class);
	if (x_class & __X87_COND_NAN)
	{
		*sinx = *cosx = x - x;
		return;
	}	
	__asm__ __volatile__(
		"fsincos\n"
		"fnstsw	%%ax\n"
		"testl	$0x400,%%eax\n"
		"jz	2f\n"
		"fldpi\n"
		"fadd	%%st(0)\n"
		"fxch	%%st(1)\n"
	"1:	fprem1\n"
		"fnstsw	%%ax\n"
		"testl	$0x400,%%eax\n"
		"jnz	1b\n"
		"fstp	%%st(1)\n"
		"fsincos\n"
	"2:\n"
		: "=t"(*cosx), "=u"(*sinx)
		: "0"(x)
		: "st(2)", "eax");
}
#define __have_fpu_sincos

__MATH_INLINE long double
__ieee754_tanl(long double x)
{
	long double value;
	unsigned short x_class;
	
	/* tan(Inf or NaN) is NaN */
	__x87_test(x, x_class);
	if (x_class & __X87_COND_NAN)
		return x - x;
	__asm__ __volatile__(
		"fptan\n"
		"fstp	%%st(0)\n"
		: "=t"(value)
		: "0"(x));
	__x87_getsw(x_class);
	if (x_class & __X87_COND_INVALID)
	{
		__asm__("fldpi\n"
			"fadd	%%st(0)\n"
			"fxch	%%st(1)\n"
		"1:	fprem1\n"
			"fnstsw	%%ax\n"
			"testl	$0x400,%%eax\n"
			"jnz	1b\n"
			"fstp	%%st(1)\n"
			"fptan\n"
			"fstp	%%st(0)\n"
			: "=t"(value)
			: "0"(x)
			: "st(1)", "eax");
	}
	return value;
}
#define __have_fpu_tan

__MATH_INLINE long double
__ieee754_acosl (long double x)
{
	long double res;

	/* acosl = atanl (sqrtl((1-x) (1+x)) / x) */
	__asm__("fld	%%st\n"
		"fmul %%st(0)\n"		/* x^2 */
		"fld1\n"
		"fsubp\n"		/* 1 - x^2 */
		"fsqrt\n"		/* sqrtl (1 - x^2) */
	   "fabs\n"
	   "fxch	%%st(1)\n"
	   "fpatan"
	: "=t"(res)
	: "0"(x)
	: "st(1)", "st(2)");
	return res;
}
#define __have_fpu_acos

__MATH_INLINE long double
__ieee754_asinl (long double x)
{
	long double res;

	/* asinl = atanl (x / sqrtl(1 - x^2)) */
	__asm__("fld	%%st\n"
		"fmul %%st(0)\n"		/* x^2 */
		"fld1\n"
		"fsubp\n"		/* 1 - x^2 */
		"fsqrt\n"		/* sqrtl (1 - x^2) */
	   "fpatan"
	: "=t"(res)
	: "0"(x)
	: "st(1)", "st(2)");
	return res;
}
#define __have_fpu_asin

__MATH_INLINE long double
__ieee754_sqrtl (long double x)
{
  long double res;

  __asm__("fsqrt" : "=t" (res) : "0" (x));

  return res;
}
#define __have_fpu_sqrt

__MATH_INLINE long double
__ieee754_atanl (long double x)
{
  long double res;

  __asm__("fld1\n"
       "fpatan"
       : "=t" (res)
       : "0" (x)
	   : "st(1)");
  return res;
}
#define __have_fpu_atan


__MATH_INLINE long double
__ieee754_atan2l (long double y, long double x)
{
  long double res;

  __asm__("fpatan" : "=t" (res) : "u" (y), "0" (x) : "st(1)");

  return res;
}
#define __have_fpu_atan2


__MATH_INLINE long double
__ieee754_fmodl (long double x, long double y)
{
  long double res;

  __asm__("1:\tfprem\n"
       "fstsw   %%ax\n"
       "sahf\n"
       "jp      1b\n"
       "fstp    %%st(1)"
       : "=t" (res) : "0" (x), "u" (y) : "ax", "st(1)");
  return res;
}
#define __have_fpu_fmod

__MATH_INLINE long double
__ieee754_remainderl (long double x, long double y)
{
  long double res;

  __asm__("1:\tfprem1\n"
       "fstsw   %%ax\n"
       "sahf\n"
       "jp      1b\n"
       "fstp    %%st(1)"
       : "=t" (res) : "0" (x), "u" (y) : "ax", "st(1)");
  return res;
}
#define __have_fpu_remainder

#if 0

__MATH_INLINE long double
__ieee754_remquol(long double x, long double y, int *quo)
{
	int cquo;
	long double value;
	
	__asm__ __volatile__(
	"1:	fprem1\n"
		"fstsw	%%ax\n"
		"sahf\n"
		"jp	1b\n"
		"fstp	%%st(1)\n"
		/* Compute the congruent of the quotient.  */
		"movl	%%eax, %%ecx\n"
		"shrl	$8, %%eax\n"
		"shrl	$12, %%ecx\n"
		"andl	$4, %%ecx\n"
		"andl	$3, %%eax\n"
		"orl	%%eax, %%ecx\n"
		"movl	$0xef2a60, %%eax\n"
		"leal	(%%ecx,%%ecx,2),%%ecx\n"
		"shrl	%%cl, %%eax\n"
		"andl	$7, %%eax\n"
        "movl   8(%4), %%ecx\n"
        "xorl   8(%5), %%ecx\n"
        "testl  $0x8000, %%ecx\n"
        "jz 1f\n"
        "negl %%eax\n"
	"1:\n"
	: "=t"(value), "=&a"(cquo)
	: "0"(x), "u"(y), "r"(&x), "r"(&y)
	: "st(1)", "ecx");
	*quo = cquo;
	return value;
}
#define __have_fpu_remquo

#endif

__MATH_INLINE long double
__ieee754_ceill (long double x)
{
  unsigned short rounding_mode, round_up;
  long double value;

  __x87_getcw(rounding_mode);
  round_up = (unsigned short)((rounding_mode & ~FE_ROUNDING_MASK) | FE_UPWARD);
  __x87_setcw(round_up);
  __asm __volatile ("frndint"
		  : "=t" (value)
		  : "0" (x));
  __x87_setcw(rounding_mode);
  return value;
}
#define __have_fpu_ceil

__MATH_INLINE long double
__ieee754_floorl (long double x)
{
  unsigned short rounding_mode, round_down;
  long double value;

  __x87_getcw(rounding_mode);
  round_down = (unsigned short)((rounding_mode & ~FE_ROUNDING_MASK) | FE_DOWNWARD);
  __x87_setcw(round_down);
  __asm __volatile ("frndint"
		  : "=t" (value)
		  : "0" (x));
  __x87_setcw(rounding_mode);
  return value;
}
#define __have_fpu_floor

__MATH_INLINE long double
__ieee754_truncl (long double x)
{
  unsigned short rounding_mode, round_down;
  long double value;

  __x87_getcw(rounding_mode);
  round_down = (rounding_mode & ~FE_ROUNDING_MASK) | FE_TOWARDZERO;
  __x87_setcw(round_down);
  __asm __volatile ("frndint"
		  : "=t" (value)
		  : "0" (x));
  __x87_setcw(rounding_mode);
  return value;
}
#define __have_fpu_trunc

__MATH_INLINE long double
__ieee754_rintl (long double x)
{
  long double value;

  __asm __volatile ("frndint"
		  : "=t" (value)
		  : "0" (x));
  return value;
}
#define __have_fpu_rint

__MATH_INLINE long
__ieee754_lrintl (long double x)
{
#if LONG_MAX > 2147483647L
  int64_t value;

  __asm __volatile ("fistpll %0"
		  : "=m" (value)
		  : "t" (x)
		  : "st");
  return value;
#else
  int32_t value;

  __asm __volatile ("fistpl %0"
		  : "=m" (value)
		  : "t" (x)
		  : "st");
  return value;
#endif
}
#define __have_fpu_lrint

__extension__ __MATH_INLINE long long
__ieee754_llrintl (long double x)
{
  long long value;

  __asm __volatile ("fistpll %0"
		  : "=m" (value)
		  : "t" (x)
		  : "st");
  return value;
}
#define __have_fpu_llrint

#if 0 /* frndint rounds in wrong direction */

__MATH_INLINE long double
__ieee754_roundl (long double x)
{
  unsigned short rounding_mode, round_nearest;
  long double value;

  __x87_getcw(rounding_mode);
  round_nearest = (unsigned short)((rounding_mode & ~FE_ROUNDING_MASK) | FE_TONEAREST);
  __x87_setcw(round_nearest);
  __asm __volatile ("frndint"
		  : "=t" (value)
		  : "0" (x));
  __x87_setcw(rounding_mode);
  return value;
}
#define __have_fpu_round

#endif

__MATH_INLINE long double
__ieee754_scalbnl (long double x, int n)
{
  long double value;
  long double exp = (long double)(n);

  __asm __volatile (
  		  "fscale\n"
  		  "fstp	%%st(1)\n"
		  : "=t" (value)
		  : "0" (x), "u"(exp)
		  : "st(1)");
  return value;
}
#define __have_fpu_scalbn

__MATH_INLINE long double
__ieee754_scalblnl (long double x, long n)
{
  long double value;
  long double exp = (long double)(n);

  __asm __volatile (
  		  "fscale\n"
  		  "fstp	%%st(1)\n"
		  : "=t" (value)
		  : "0" (x), "u"(exp)
		  : "st(1)");
  return value;
}
#define __have_fpu_scalbln


__MATH_INLINE long double
__ieee754_logbl (long double x)
{
  long double exponent;

  __asm __volatile (
       "fxtract\n"
       "fstp	%%st"
	 : "=t" (exponent)
	 : "0" (x));
  return exponent;
}
#define __have_fpu_logb


__MATH_INLINE long double
__ieee754_expm1l(long double x)
{
	int x_class = fpclassify(x);
	long double value;
	
	if (x_class == FP_NAN)
		return x;
	if (x_class == FP_INFINITE)
		return signbit(x) ? -1.0L : x;
	if (x_class == FP_ZERO)
		return x;
	if (x > 11356.523406294143949491931077970765L)
		return LDBL_MAX * LDBL_MAX;
	if (x < -7.9018778583833765273564461846232128760607E1L)
		return (4.0 / 1e4932L - 1.0L);
  __asm __volatile (
	"fldl2e\n"					/* log2(e) : x */
	"fmulp\n"					/* log2(e)*x */
	"fld	%%st\n"				/* log2(e)*x : log2(e)*x */
	"frndint\n"					/* int(log2(e)*x) : log2(e)*x */
	"fsubr	%%st, %%st(1)\n"	/* int(log2(e)*x) : fract(log2(e)*x) */
	"fxch\n"					/* fract(log2(e)*x) : int(log2(e)*x) */
	"f2xm1\n"					/* 2^fract(log2(e)*x)-1 : int(log2(e)*x) */
	"fscale\n"					/* 2^(log2(e)*x)-2^int(log2(e)*x) : int(log2(e)*x) */
	"fxch\n"					/* int(log2(e)*x) : 2^(log2(e)*x)-2^int(log2(e)*x) */
	"fld1\n"					/* 1 : int(log2(e)*x) : 2^(log2(e)*x)-2^int(log2(e)*x) */
	"fscale\n"					/* 2^int(log2(e)*x) : int(log2(e)*x) : 2^(log2(e)*x)-2^int(log2(e)*x) */
	"fstp	%%st(1)\n"			/* 2^int(log2(e)*x) : 2^(log2(e)*x)-2^int(log2(e)*x) */
	"fld1\n"					/* 1 : 2^int(log2(e)*x) : 2^(log2(e)*x)-2^int(log2(e)*x) */
	"fsubp	%%st, %%st(1)\n"	/* 1-2^int(log2(e)*x) : 2^(log2(e)*x)-2^int(log2(e)*x) */
	"fsubrp	%%st, %%st(1)\n"	/* 2^(log2(e)*x)-1 */
	 : "=t" (value)
	 : "0" (x)
	 : "st(1)", "st(2)");
	return value;
}
#define __have_fpu_expm1


__MATH_INLINE long double
__ieee754_exp2l (long double x)
{
	unsigned short fpsr;
	
	__x87_test(x, fpsr);
	if ((fpsr & (__X87_COND_ZERO|__X87_COND_INF)) == __X87_COND_INF)
	{
		if (fpsr & __X87_COND_NEG)
			x = 0;
	} else
	{
		double y;
		__asm (
			"\tfld	%%st\n"				/* x : x */
			"\tfrndint\n"				/* int(x) : x */
			"\tfsubr %%st,%%st(1)\n"	/* int(x) : fract(x) */
			"\tfxch\n"					/* fract(x) : int(x) */
			"\tf2xm1\n"					/* 2^(fract(x)) - 1 : int(x) */
			"\tfld1\n"					/* 1 : 2^(fract(x)) - 1 : int(x) */
			"\tfaddp\n" 				/* 2^(fract(x)) : int(x) */
			"\tfscale\n"				/* 2^x : int(x)  */
			"\tfstp	%%st(1)\n"
		: "=t"(y)
		: "0"(x)
		: "st(1)", "st(2)");
		x = y;
	}
	return x;
}
#define __have_fpu_exp2


__MATH_INLINE long double
__ieee754_logl (long double x)
{
	int x_class = fpclassify(x);
	long double value;
	
	if (x_class == FP_ZERO)
		return -HUGE_VALL;
	if (signbit(x))
		return __builtin_nanl("");
	if (x_class == FP_INFINITE || x_class == FP_NAN)
		return x + x;
	__asm (
		"fldln2\n"			/* log(2) */
		"fxch\n"			/* x : log(2) */
		"fyl2x\n"			/* log(x) */
	: "=t"(value)
	: "0"(x)
	: "st(1)", "st(2)");
	return value;
}
#define __have_fpu_log

__MATH_INLINE long double
__ieee754_log1pl (long double x)
{
	int x_class = fpclassify(x);
	long double value;

	/* It is not important that this constant is precise.  It is only
	   a value which is known to be on the safe side for using the
	   fyl2xp1 instruction.  */
	const double limit = 0.29;
	
	if (x_class == FP_INFINITE || x_class == FP_NAN)
		return x + x;
	if (x_class == FP_ZERO)
		return x;

	if (__ieee754_fabsl(x) < limit)
	{
		__asm (
			"fldln2\n"					/* log(2) */
			"fxch\n"					/* x : log(2) */
			"fyl2xp1\n"					/* log(x) */
		: "=t"(value)
		: "0"(x)
		: "st(1)");
	} else
	{
		__asm (
			"fldln2\n"					/* log(2) */
			"fxch\n"					/* x : log(2) */
			"fld1\n"					/* 1 : x : log(2) */
			"faddp\n"					/* x + 1 : log(2) */
			"fyl2x\n"					/* log(x) */
		: "=t"(value)
		: "0"(x)
		: "st(1)", "st(2)");
	}
	return value;
}
#define __have_fpu_log1p

__MATH_INLINE long double
__ieee754_log2l (long double x)
{
	int x_class = fpclassify(x);
	long double value;

	if (x_class == FP_INFINITE || x_class == FP_NAN)
		return x + x;

	__asm (
		"fld1\n"					/* 1 */
		"fxch\n"					/* x : 1 */
		"fyl2x\n"					/* log2(x) */
	: "=t"(value)
	: "0"(x)
	: "st(1)");
	return value;
}
#define __have_fpu_log2
	
__MATH_INLINE long double
__ieee754_log10l (long double x)
{
	int x_class = fpclassify(x);
	long double value;

	if (x_class == FP_INFINITE || x_class == FP_NAN)
		return x + x;

	__asm (
		"fldlg2\n"					/* log10(2) */
		"fxch\n"					/* x : log10(2) */
		"fyl2x\n"					/* log10(x) */
	: "=t"(value)
	: "0"(x)
	: "st(1)");
	return value;
}
#define __have_fpu_log10
	
__MATH_INLINE long double
__ieee754_expl(long double x)
{
	int x_class = fpclassify(x);

	if (x_class == FP_NAN)
		return x;
	if (x_class == FP_INFINITE)
		return signbit(x) ? 0.0 : x;
	if (x_class == FP_ZERO)
		return 1.0L;
	if (x > 11356.523406294143949491931077970765L)
	{
		feraiseexcept(FE_OVERFLOW);
		return HUGE_VALL;
	}
	if (x < -11355.137111933024058885L)
	{
		feraiseexcept(FE_UNDERFLOW);
		return 0;
	}
	return __ieee754_exp2l(1.44269504088896340735992468100189213743L * x);
}
#define __have_fpu_exp

__MATH_INLINE long double
__ieee754_modfl (long double x, long double *ip)
{
  long double temp, result;
  unsigned short x_cond;

  temp = __ieee754_truncl(x);
  if (ip)
    *ip = temp;
  __x87_test (x, x_cond);
  if ((x_cond & (__X87_COND_ZERO|__X87_COND_INF)) == __X87_COND_INF)
    {
      result = 0;
      if (x_cond & __X87_COND_NEG)
	result = -result;
    }
  else if ((x_cond & __X87_COND_ZERO) == __X87_COND_ZERO)
    result = x;
  else
    result = x - temp;
  return result;
}
#define __have_fpu_modf

#endif /* __NO_MATH_INLINES */
