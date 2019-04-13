#include "asm/ieee754/x86l.h"

#ifndef __NO_MATH_INLINES

__MATH_INLINE double
__ieee754_fabs(double x)
{
	return (double)__ieee754_fabsl(x);
}

__MATH_INLINE float
__ieee754_fabsf(float x)
{
	return (float)__ieee754_fabsl(x);
}

__MATH_INLINE double
__ieee754_sin(double x)
{
	return (double)__ieee754_sinl(x);
}

__MATH_INLINE float
__ieee754_sinf(float x)
{
	return (float)__ieee754_sinl(x);
}

__MATH_INLINE double
__ieee754_cos(double x)
{
	return (double)__ieee754_cosl(x);
}

__MATH_INLINE float
__ieee754_cosf(float x)
{
	return (float)__ieee754_cosl(x);
}

__MATH_INLINE void
__ieee754_sincos(double x, double *sinx, double *cosx)
{
	long double lsinx, lcosx;
	__ieee754_sincosl(x, &lsinx, &lcosx);
	*sinx = lsinx;
	*cosx = lcosx;
}

__MATH_INLINE void
__ieee754_sincosf(float x, float *sinx, float *cosx)
{
	long double lsinx, lcosx;
	__ieee754_sincosl(x, &lsinx, &lcosx);
	*sinx = lsinx;
	*cosx = lcosx;
}

__MATH_INLINE double
__ieee754_tan(double x)
{
	return (double)__ieee754_tanl(x);
}

__MATH_INLINE float
__ieee754_tanf(float x)
{
	return (float)__ieee754_tanl(x);
}

__MATH_INLINE double
__ieee754_sqrt (double x)
{
  double res;

  asm ("fsqrt" : "=t" (res) : "0" (x));

  return res;
}

__MATH_INLINE float
__ieee754_sqrtf (float x)
{
  float res;

  asm ("fsqrt" : "=t" (res) : "0" (x));

  return res;
}

__MATH_INLINE double
__ieee754_acos (double x)
{
	double res;

	/* acosl = atanl (sqrtl((1-x) (1+x)) / x) */
	asm("fld	%%st\n"
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

__MATH_INLINE float
__ieee754_acosf (float x)
{
	float res;

	/* acosl = atanl (sqrtl((1-x) (1+x)) / x) */
	asm("fld	%%st\n"
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

__MATH_INLINE double
__ieee754_asin (double x)
{
	double res;

	/* asinl = atanl (x / sqrtl(1 - x^2)) */
	asm("fld	%%st\n"
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

__MATH_INLINE float
__ieee754_asinf (float x)
{
	float res;

	/* asinl = atanl (x / sqrtl(1 - x^2)) */
	asm("fld	%%st\n"
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


__MATH_INLINE double
__ieee754_atan (double x)
{
  double res;

  asm ("fld1\n"
       "fpatan"
       : "=t" (res)
       : "0" (x)
	   : "st(1)");
  return res;
}

__MATH_INLINE float
__ieee754_atanf (float x)
{
  float res;

  asm ("fld1\n"
       "fpatan"
       : "=t" (res)
       : "0" (x)
	   : "st(1)");
  return res;
}


__MATH_INLINE double
__ieee754_atan2 (double y, double x)
{
  double res;

  asm ("fpatan" : "=t" (res) : "u" (y), "0" (x) : "st(1)");

  return res;
}

__MATH_INLINE float
__ieee754_atan2f (float y, float x)
{
  float res;

  asm ("fpatan" : "=t" (res) : "u" (y), "0" (x) : "st(1)");

  return res;
}


__MATH_INLINE double
__ieee754_fmod (double x, double y)
{
	return (double) __ieee754_fmodl(x, y);
}

__MATH_INLINE float
__ieee754_fmodf (float x, float y)
{
	return (float) __ieee754_fmodl(x, y);
}

__MATH_INLINE double
__ieee754_remainder (double x, double y)
{
	return (double) __ieee754_remainderl(x, y);
}

__MATH_INLINE float
__ieee754_remainderf (float x, float y)
{
	return (float) __ieee754_remainderl(x, y);
}

#if 0

__MATH_INLINE double
__ieee754_remquo (double x, double y, int *quo)
{
	return (double) __ieee754_remquol(x, y, quo);
}

__MATH_INLINE float
__ieee754_remquof (float x, float y, int *quo)
{
	return (float) __ieee754_remquol(x, y, quo);
}

#endif

__MATH_INLINE double
__ieee754_ceil (double x)
{
	return (double) __ieee754_ceill(x);
}

__MATH_INLINE float
__ieee754_ceilf (float x)
{
	return (float) __ieee754_ceill(x);
}

__MATH_INLINE double
__ieee754_floor (double x)
{
	return (double) __ieee754_floorl(x);
}

__MATH_INLINE float
__ieee754_floorf (float x)
{
	return (float) __ieee754_floorl(x);
}

__MATH_INLINE double
__ieee754_trunc (double x)
{
	return (double) __ieee754_truncl(x);
}

__MATH_INLINE float
__ieee754_truncf (float x)
{
	return (float) __ieee754_truncl(x);
}

__MATH_INLINE double
__ieee754_rint (double x)
{
	return (double) __ieee754_rintl(x);
}

__MATH_INLINE float
__ieee754_rintf (float x)
{
	return (float) __ieee754_rintl(x);
}

__MATH_INLINE long
__ieee754_lrint (double x)
{
	return __ieee754_lrintl(x);
}

__MATH_INLINE long
__ieee754_lrintf (float x)
{
	return __ieee754_lrintl(x);
}

__extension__ __MATH_INLINE long long
__ieee754_llrint (double x)
{
	return __ieee754_llrintl(x);
}

__extension__ __MATH_INLINE long long
__ieee754_llrintf (float x)
{
	return __ieee754_llrintl(x);
}

#if 0 /* frndint rounds in wrong direction */

__MATH_INLINE double
__ieee754_round (double x)
{
	return (double) __ieee754_roundl(x);
}

__MATH_INLINE float
__ieee754_roundf (float x)
{
	return (float) __ieee754_roundl(x);
}

#endif

__MATH_INLINE double
__ieee754_scalbn (double x, int n)
{
	return (double) __ieee754_scalbnl(x, n);
}

__MATH_INLINE float
__ieee754_scalbnf (float x, int n)
{
	return (float) __ieee754_scalbnl(x, n);
}

__MATH_INLINE double
__ieee754_scalbln (double x, long n)
{
	return (double) __ieee754_scalblnl(x, n);
}

__MATH_INLINE float
__ieee754_scalblnf (float x, long n)
{
	return (float) __ieee754_scalblnl(x, n);
}

__MATH_INLINE double
__ieee754_logb (double x)
{
	return (double) __ieee754_logbl(x);
}

__MATH_INLINE float
__ieee754_logbf (float x)
{
	return (float) __ieee754_logbl(x);
}

__MATH_INLINE double
__ieee754_expm1 (double x)
{
	return (double) __ieee754_expm1l(x);
}

__MATH_INLINE float
__ieee754_expm1f (float x)
{
	return (float) __ieee754_expm1l(x);
}

__MATH_INLINE double
__ieee754_exp2 (double x)
{
	return (double) __ieee754_exp2l(x);
}

__MATH_INLINE float
__ieee754_exp2f (float x)
{
	return (float) __ieee754_exp2l(x);
}

__MATH_INLINE double
__ieee754_log (double x)
{
	return (double) __ieee754_logl(x);
}

__MATH_INLINE float
__ieee754_logf (float x)
{
	return (float) __ieee754_logl(x);
}

__MATH_INLINE double
__ieee754_log1p (double x)
{
	return (double) __ieee754_log1pl(x);
}

__MATH_INLINE float
__ieee754_log1pf (float x)
{
	return (float) __ieee754_log1pl(x);
}

__MATH_INLINE double
__ieee754_log2 (double x)
{
	return (double) __ieee754_log2l(x);
}

__MATH_INLINE float
__ieee754_log2f (float x)
{
	return (float) __ieee754_log2l(x);
}

__MATH_INLINE double
__ieee754_log10 (double x)
{
	return (double) __ieee754_log10l(x);
}

__MATH_INLINE float
__ieee754_log10f (float x)
{
	return (float) __ieee754_log10l(x);
}

__MATH_INLINE double
__ieee754_exp (double x)
{
	return (double) __ieee754_expl(x);
}

__MATH_INLINE float
__ieee754_expf (float x)
{
	return (float) __ieee754_expl(x);
}

__MATH_INLINE double
__ieee754_modf (double x, double *ip)
{
	long double temp, result;
	result = __ieee754_modfl(x, &temp);
	if (ip)
		*ip = (double) temp;
	return (double) result;
}

__MATH_INLINE float
__ieee754_modff (float x, float *ip)
{
	long double temp, result;
	result = __ieee754_modfl(x, &temp);
	if (ip)
		*ip = (float) temp;
	return (float) result;
}



#if 0

"0", "1", and "i" stand for the associated
floating point stack registers, not constant numbers!  Constant
numbers in the descriptions are given with decimal points: 0.0,
1.0, 2.0, 10.0.


    Opcode    Instruction     Description

 w  D9 F0     F2XM1           0 := (2.0 ** 0) - 1.0
 w  D9 E1     FABS            0 := |0|
 w  DE C1     FADD            1 := 1 + 0, pop
 w  D8 C0+i   FADD i          0 := i + 0
 w  DC C0+i   FADD i,0        i := i + 0
 w  D8 C0+i   FADD 0,i        0 := i + 0
 w  D8 /0     FADD mem4r      0 := 0 + mem4r
 w  DC /0     FADD mem8r      0 := 0 + mem8r
 w  DE C0+i   FADDP i,0       i := i + 0, pop
 w  DF /4     FBLD mem10d     push, 0 := mem10d
 w  DF /6     FBSTP mem10d    mem10d := 0, pop
 w  D9 E0     FCHS            0 := -0
9B  DB E2     FCLEX           clear exceptions
 w  D8 D1     FCOM            compare 0 - 1
 w  D8 D0+i   FCOM 0,i        compare 0 - i
 w  D8 D0+i   FCOM i          compare 0 - i
 w  D8 /2     FCOM mem4r      compare 0 - mem4r
 w  DC /2     FCOM mem8r      compare 0 - mem8r
 w  D8 D9     FCOMP           compare 0 - 1, pop
 w  D8 D8+i   FCOMP 0,i       compare 0 - i, pop
 w  D8 D8+i   FCOMP i         compare 0 - i, pop
 w  D8 /3     FCOMP mem4r     compare 0 - mem4r, pop
 w  DC /3     FCOMP mem8r     compare 0 - mem8r, pop
 w  DE D9     FCOMPP          compare 0 - 1, pop both
 w  D9 FF     FCOS            387 only: push, 1/0 := cosine(old 0)
 w  D9 F6     FDECSTP         decrement stack pointer
 w  DB E1     FDISI           disable interrupts (.287 ignore)
 w  DE F9     FDIV            1 := 1 / 0, pop
 w  D8 F0+i   FDIV i          0 := 0 / i
 w  DC F8+i   FDIV i,0        i := i / 0
 w  D8 F0+i   FDIV 0,i        0 := 0 / i
 w  D8 /6     FDIV mem4r      0 := 0 / mem4r
 w  DC /6     FDIV mem8r      0 := 0 / mem8r
 w  DE F8+i   FDIVP i,0       i := i / 0, pop
 w  DE F1     FDIVR           1 := 0 / 1, pop
 w  D8 F8+i   FDIVR i         0 := i / 0
 w  DC F0+i   FDIVR i,0       i := 0 / i
 w  D8 F8+i   FDIVR 0,i       0 := i / 0
 w  D8 /7     FDIVR mem4r     0 := mem4r / 0
 w  DC /7     FDIVR mem8r     0 := mem8r / 0
 w  DE F0+i   FDIVRP i,0      i := 0 / i, pop
 w  DB E0     FENI            enable interrupts (.287 ignore)
 w  DD C0+i   FFREE i         empty i
 w  DE /0     FIADD mem2i     0 := 0 + mem4i
 w  DA /0     FIADD mem4i     0 := 0 + mem2i
 w  DE /2     FICOM mem2i     compare 0 - mem2i
 w  DA /2     FICOM mem4i     compare 0 - mem4i
 w  DE /3     FICOMP mem2i    compare 0 - mem2i, pop
 w  DA /3     FICOMP mem4i    compare 0 - mem4i, pop
 w  DE /6     FIDIV mem2i     0 := 0 / mem2i
 w  DA /6     FIDIV mem4i     0 := 0 / mem4i
 w  DE /7     FIDIVR mem2i    0 := mem2i / 0
 w  DA /7     FIDIVR mem4i    0 := mem4i / 0
 w  DF /0     FILD mem2i      push, 0 := mem2i
 w  DB /0     FILD mem4i      push, 0 := mem4i
 w  DF /5     FILD mem8i      push, 0 := mem8i
 w  DE /1     FIMUL mem2i     0 := 0 * mem2i
 w  DA /1     FIMUL mem4i     0 := 0 * mem4i
 w  D9 F7     FINCSTP         increment stack pointer
9B  DB E3     FINIT           initialize 87
 w  DF /2     FIST mem2i      mem2i := 0
 w  DB /2     FIST mem4i      mem4i := 0
 w  DF /3     FISTP mem2i     mem2i := 0, pop
 w  DB /3     FISTP mem4i     mem4i := 0, pop
 w  DF /7     FISTP mem8i     mem8i := 0, pop
 w  DE /4     FISUB mem2i     0 := 0 - mem2i
 w  DA /4     FISUB mem4i     0 := 0 - mem4i
 w  DE /5     FISUBR mem2i    0 := mem2i - 0
 w  DA /5     FISUBR mem4i    0 := mem4i - 0
 w  D9 C0+i   FLD i           push, 0 := old i
 w  DB /5     FLD mem10r      push, 0 := mem10r
 w  D9 /0     FLD mem4r       push, 0 := mem4r
 w  DD /0     FLD mem8r       push, 0 := mem8r
 w  D9 E8     FLD1            push, 0 := 1.0
 w  D9 /5     FLDCW mem2i     control word := mem2i
 w  D9 /4     FLDENV mem14    environment := mem14
 w  D9 EA     FLDL2E          push, 0 := log base 2.0 of e
 w  D9 E9     FLDL2T          push, 0 := log base 2.0 of 10.0
 w  D9 EC     FLDLG2          push, 0 := log base 10.0 of 2.0
 w  D9 ED     FLDLN2          push, 0 := log base e of 2.0
 w  D9 EB     FLDPI           push, 0 := Pi
 w  D9 EE     FLDZ            push, 0 := +0.0
 w  DE C9     FMUL            1 := 1 * 0, pop
 w  D8 C8+i   FMUL i          0 := 0 * i
 w  DC C8+i   FMUL i,0        i := i * 0
 w  D8 C8+i   FMUL 0,i        0 := 0 * i
 w  D8 /1     FMUL mem4r      0 := 0 * mem4r
 w  DC /1     FMUL mem8r      0 := 0 * mem8r
 w  DE C8+i   FMULP i,0       i := i * 0, pop
    DB E2     FNCLEX          nowait clear exceptions
    DB E1     FNDISI          disable interrupts (.287 ignore)
    DB E0     FNENI           enable interrupts (.287 ignore)
    DB E3     FNINIT          nowait initialize 87
 w  D9 D0     FNOP            no operation
    DD /6     FNSAVE mem94    mem94 := 87 state
    D9 /7     FNSTCW mem2i    mem2i := control word
    D9 /6     FNSTENV mem14   mem14 := environment
    DF E0     FNSTSW AX       AX := status word
    DD /7     FNSTSW mem2i    mem2i := status word
 w  D9 F3     FPATAN          0 := arctan(1/0), pop
 w  D9 F8     FPREM           0 := REPEAT(0 - 1)
 w  D9 F5     FPREM1          387 only: 0 := REPEAT(0 - 1) IEEE compat.
 w  D9 F2     FPTAN           push, 1/0 := tan(old 0)
 w  D9 FC     FRNDINT         0 := round(0)
 w  DD /4     FRSTOR mem94    87 state := mem94
 w  DD /6     FSAVE mem94     mem94 := 87 state
 w  D9 FD     FSCALE          0 := 0 * 2.0 ** 1
9B  DB E4     FSETPM          set protection mode
 w  D9 FE     FSIN            387 only: push, 1/0 := sine(old 0)
 w  D9 FB     FSINCOS         387 only: push, 1 := sine, 0 := cos(old 0)
 w  D9 FA     FSQRT           0 := square root of 0
 w  DD D0+i   FST i           i := 0
 w  D9 /2     FST mem4r       mem4r := 0
 w  DD /2     FST mem8r       mem8r := 0
 w  D9 /7     FSTCW mem2i     mem2i := control word
 w  D9 /6     FSTENV mem14    mem14 := environment
 w  DD D8+i   FSTP i          i := 0, pop
 w  DB /7     FSTP mem10r     mem10r := 0, pop
 w  D9 /3     FSTP mem4r      mem4r := 0, pop
 w  DD /3     FSTP mem8r      mem8r := 0, pop
 w  DF E0     FSTSW AX        AX := status word
 w  DD /7     FSTSW mem2i     mem2i := status word
 w  DE E9     FSUB            1 := 1 - 0, pop
 w  D8 E0+i   FSUB i          0 := 0 - i
 w  DC E8+i   FSUB i,0        i := i - 0
 w  D8 E0+i   FSUB 0,i        0 := 0 - i
 w  D8 /4     FSUB mem4r      0 := 0 - mem4r
 w  DC /4     FSUB mem8r      0 := 0 - mem8r
 w  DE E8+i   FSUBP i,0       i := i - 0, pop
 w  DE E1     FSUBR           1 := 0 - 1, pop
 w  D8 E8+i   FSUBR i         0 := i - 0
 w  DC E8+i   FSUBR i,0       0 := i - 0
 w  D8 E0+i   FSUBR 0,i       i := i - 0
 w  D8 /5     FSUBR mem4r     0 := mem4r - 0
 w  DC /5     FSUBR mem8r     0 := mem8r - 0
 w  DE E0+i   FSUBRP i,0      i := 0 - i, pop
 w  D9 E4     FTST            compare 0 - 0.0
 w  DD E0+i   FUCOM i         387 only: unordered compare 0 - i
 w  DD E1     FUCOM           387 only: unordered compare 0 - 1
 w  DD E8+i   FUCOMP i        387 only: unordered compare 0 - i, pop
 w  DD E9     FUCOMP          387 only: unordered compare 0 - 1, pop
 w  DA E9     FUCOMPP         387 only: unordered compare 0 - 1, pop both
9B            FWAIT           wait for 87 ready
 w  D9 E5     FXAM            C3 -- C0 := type of 0
 w  D9 C9     FXCH            exchange 0 and 1
 w  D9 C8+i   FXCH 0,i        exchange 0 and i
 w  D9 C8+i   FXCH i          exchange 0 and i
 w  D9 C8+i   FXCH i,0        exchange 0 and i
 w  D9 F4     FXTRACT         push, 1 := expo, 0 := sig
 w  D9 F1     FYL2X           0 := 1 * log base 2.0 of 0, pop
 w  D9 F9     FYL2XP1         0 := 1 * log base 2.0 of (0+1.0), pop

#endif

#endif /* __NO_MATH_INLINES */
