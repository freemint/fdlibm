/* @(#)k_standard.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#include "fdlibm.h"
#include <errno.h>

#ifndef _USE_WRITE
#include <stdio.h>						/* fputs(), stderr */
#define	WRITE2(u,v)	fputs(u, stderr)
#else /* !defined(_USE_WRITE) */
#include <unistd.h>						/* write */
#define	WRITE2(u,v)	write(2, u, v)
#undef fflush
#endif /* !defined(_USE_WRITE) */

/* if struct exception has err field...*/
#if defined(__CYGWIN__) || defined(__MSYS__)
#define reset_matherr_errno(exc) exc.err = 0
#define set_matherr_errno(exc) if (exc.err != 0) __set_errno(exc.err)
#else
#define reset_matherr_errno(exc) 
#define set_matherr_errno(exc) 
#endif

#ifndef HUGE
#  define HUGE HUGE_VAL
#endif

/*
 * Standard conformance (non-IEEE) on exception cases.
 * Mapping:
 *	1 -- acos(|x|>1)
 *	2 -- asin(|x|>1)
 *	3 -- atan2(+-0,+-0)
 *	4 -- hypot overflow
 *	5 -- cosh overflow
 *	6 -- exp overflow
 *	7 -- exp underflow
 *	8 -- y0(0)
 *	9 -- y0(x<0)
 *	10-- y1(0)
 *	11-- y1(x<0)
 *	12-- yn(0)
 *	13-- yn(x<0)
 *	14-- lgamma(finite) overflow
 *	15-- lgamma(-integer)
 *	16-- log(0)
 *	17-- log(x<0)
 *	18-- log10(0)
 *	19-- log10(x<0)
 *	20-- pow(0.0,0.0)
 *	21-- pow(x,y) overflow
 *	22-- pow(x,y) underflow
 *	23-- pow(0,negative)
 *	24-- pow(neg,non-integral)
 *	25-- sinh(finite) overflow
 *	26-- sqrt(negative)
 *  27-- fmod(x,0)
 *  28-- remainder(x,0)
 *	29-- acosh(x<1)
 *	30-- atanh(|x|>1)
 *	31-- atanh(|x|=1)
 *	32-- scalb overflow
 *	33-- scalb underflow
 *	34-- j0(|x|>X_TLOSS)
 *	35-- y0(x>X_TLOSS)
 *	36-- j1(|x|>X_TLOSS)
 *	37-- y1(x>X_TLOSS)
 *	38-- jn(|x|>X_TLOSS, n)
 *	39-- yn(x>X_TLOSS, n)
 *	40-- tgamma(finite) overflow
 *	41-- tgamma(-integer)
 *	42-- pow(NaN,0.0)
 *	43-- +0**neg
 *	44-- exp2 overflow
 *	45-- exp2 underflow
 *	46-- exp10 overflow
 *	47-- exp10 underflow
 *	48-- log2(0)
 *	49-- log2(x<0)
 *	50-- tgamma(+-0)
 */



double __kernel_standard(double x, double y, double retval, enum matherr type)
{
	struct exception exc;

/* XXX gcc versions until now don't delay the 0.0/0.0 division until
   runtime but produce NaN at compile time.  This is wrong since the
   exceptions are not set correctly.  */
#if 0
	static const double zero = 0.0;			/* used as const */
#else
	static double zero = 0.0;				/* used as const */
#endif

#ifdef _USE_WRITE
	(void) fflush(stdout);
#endif

#define funcname(name) (type >= KMATHERRL_FIRST ? name "l" : (type >= KMATHERRF_FIRST ? name "f" : name))

	exc.arg1 = x;
	exc.arg2 = y;
	exc.retval = retval;
	reset_matherr_errno(exc);
	switch (type)
	{
	case KMATHERR_ACOS:
	case KMATHERRF_ACOS:
	case KMATHERRL_ACOS:
		/* acos(|x|>1) */
		exc.type = DOMAIN;
		exc.name = funcname("acos");
		if (_LIB_VERSION == _SVID_)
			exc.retval = HUGE;
		else
			exc.retval = __builtin_nan("");
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("acos: DOMAIN error\n", 19);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_ASIN:
	case KMATHERRF_ASIN:
	case KMATHERRL_ASIN:
		/* asin(|x|>1) */
		exc.type = DOMAIN;
		exc.name = funcname("asin");
		if (_LIB_VERSION == _SVID_)
			exc.retval = HUGE;
		else
			exc.retval = __builtin_nan("");
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("asin: DOMAIN error\n", 19);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_ATAN2:
	case KMATHERRF_ATAN2:
	case KMATHERRL_ATAN2:
		/* atan2(+-0,+-0) */
		exc.arg1 = y;
		exc.arg2 = x;
		exc.type = DOMAIN;
		exc.name = funcname("atan2");
		exc.retval = HUGE;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("atan2: DOMAIN error\n", 20);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_HYPOT:
	case KMATHERRF_HYPOT:
	case KMATHERRL_HYPOT:
		/* hypot(finite,finite) overflow */
		exc.type = OVERFLOW;
		exc.name = funcname("hypot");
		if (_LIB_VERSION == _SVID_)
			exc.retval = HUGE;
		else
			exc.retval = HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_COSH:
	case KMATHERRF_COSH:
	case KMATHERRL_COSH:
		/* cosh(finite) overflow */
		exc.type = OVERFLOW;
		exc.name = funcname("cosh");
		if (_LIB_VERSION == _SVID_)
			exc.retval = HUGE;
		else
			exc.retval = HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_EXP_OVERFLOW:
	case KMATHERRF_EXP_OVERFLOW:
	case KMATHERRL_EXP_OVERFLOW:
		/* exp(finite) overflow */
		exc.type = OVERFLOW;
		exc.name = funcname("exp");
		if (_LIB_VERSION == _SVID_)
			exc.retval = HUGE;
		else
			exc.retval = HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_EXP_UNDERFLOW:
	case KMATHERRF_EXP_UNDERFLOW:
	case KMATHERRL_EXP_UNDERFLOW:
		/* exp(finite) underflow */
		exc.type = UNDERFLOW;
		exc.name = funcname("exp");
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_EXPM1_OVERFLOW:
	case KMATHERRF_EXPM1_OVERFLOW:
	case KMATHERRL_EXPM1_OVERFLOW:
		/* exp(finite) overflow */
		exc.type = OVERFLOW;
		exc.name = funcname("expm1");
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_EXPM1_UNDERFLOW:
	case KMATHERRF_EXPM1_UNDERFLOW:
	case KMATHERRL_EXPM1_UNDERFLOW:
		/* exp(finite) underflow */
		exc.type = UNDERFLOW;
		exc.name = funcname("exp");
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_Y0_ZERO:
	case KMATHERRF_Y0_ZERO:
	case KMATHERRL_Y0_ZERO:
		/* y0(0) = -inf */
		exc.type = DOMAIN;				/* should be SING for IEEE */
		exc.name = funcname("y0");
		if (_LIB_VERSION == _SVID_)
			exc.retval = -HUGE;
		else
			exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("y0: DOMAIN error\n", 17);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_Y0_MINUS:
	case KMATHERRF_Y0_MINUS:
	case KMATHERRL_Y0_MINUS:
		/* y0(x<0) = NaN */
		exc.type = DOMAIN;
		exc.name = funcname("y0");
		if (_LIB_VERSION == _SVID_)
			exc.retval = -HUGE;
		else
			exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("y0: DOMAIN error\n", 17);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_Y1_ZERO:
	case KMATHERRF_Y1_ZERO:
	case KMATHERRL_Y1_ZERO:
		/* y1(0) = -inf */
		exc.type = DOMAIN;				/* should be SING for IEEE */
		exc.name = funcname("y1");
		if (_LIB_VERSION == _SVID_)
			exc.retval = -HUGE;
		else
			exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("y1: DOMAIN error\n", 17);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_Y1_MINUS:
	case KMATHERRF_Y1_MINUS:
	case KMATHERRL_Y1_MINUS:
		/* y1(x<0) = NaN */
		exc.type = DOMAIN;
		exc.name = funcname("y1");
		if (_LIB_VERSION == _SVID_)
			exc.retval = -HUGE;
		else
			exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("y1: DOMAIN error\n", 17);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_YN_ZERO:
	case KMATHERRF_YN_ZERO:
	case KMATHERRL_YN_ZERO:
		/* yn(n,0) = -inf */
		exc.type = DOMAIN;				/* should be SING for IEEE */
		exc.name = funcname("yn");
		if (_LIB_VERSION == _SVID_)
			exc.retval = -HUGE;
		else
			exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("yn: DOMAIN error\n", 17);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_YN_MINUS:
	case KMATHERRF_YN_MINUS:
	case KMATHERRL_YN_MINUS:
		/* yn(x<0) = NaN */
		exc.type = DOMAIN;
		exc.name = funcname("yn");
		if (_LIB_VERSION == _SVID_)
			exc.retval = -HUGE;
		else
			exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("yn: DOMAIN error\n", 17);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_LGAMMA_OVERFLOW:
	case KMATHERRF_LGAMMA_OVERFLOW:
	case KMATHERRL_LGAMMA_OVERFLOW:
		/* lgamma(finite) overflow */
		exc.type = OVERFLOW;
		exc.name = funcname("lgamma");
		if (_LIB_VERSION == _SVID_)
			exc.retval = HUGE;
		else
			exc.retval = HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_LGAMMA_MINUS:
	case KMATHERRF_LGAMMA_MINUS:
	case KMATHERRL_LGAMMA_MINUS:
		/* lgamma(-integer) or lgamma(0) */
		exc.type = SING;
		exc.name = funcname("lgamma");
		if (_LIB_VERSION == _SVID_)
			exc.retval = HUGE;
		else
			exc.retval = HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("lgamma: SING error\n", 19);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_LOG_ZERO:
	case KMATHERRF_LOG_ZERO:
	case KMATHERRL_LOG_ZERO:
		/* log(0) */
		exc.type = SING;
		exc.name = funcname("log");
		if (_LIB_VERSION == _SVID_)
			exc.retval = -HUGE;
		else
			exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("log: SING error\n", 16);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_LOG_MINUS:
	case KMATHERRF_LOG_MINUS:
	case KMATHERRL_LOG_MINUS:
		/* log(x<0) */
		exc.type = DOMAIN;
		exc.name = funcname("log");
		if (_LIB_VERSION == _SVID_)
			exc.retval = -HUGE;
		else
			exc.retval = __builtin_nan("");
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("log: DOMAIN error\n", 18);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_LOG10_ZERO:
	case KMATHERRF_LOG10_ZERO:
	case KMATHERRL_LOG10_ZERO:
		/* log10(0) */
		exc.type = SING;
		exc.name = funcname("log10");
		if (_LIB_VERSION == _SVID_)
			exc.retval = -HUGE;
		else
			exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("log10: SING error\n", 18);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_LOG10_MINUS:
	case KMATHERRF_LOG10_MINUS:
	case KMATHERRL_LOG10_MINUS:
		/* log10(x<0) */
		exc.type = DOMAIN;
		exc.name = funcname("log10");
		if (_LIB_VERSION == _SVID_)
			exc.retval = -HUGE;
		else
			exc.retval = __builtin_nan("");
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("log10: DOMAIN error\n", 20);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_POW_ZERO:
	case KMATHERRF_POW_ZERO:
	case KMATHERRL_POW_ZERO:
		/* pow(0.0,0.0) */
		/* error only if _LIB_VERSION == _SVID_ */
		exc.type = DOMAIN;
		exc.name = funcname("pow");
		exc.retval = zero;
		if (_LIB_VERSION != _SVID_)
			exc.retval = 1.0;
		else if (!matherr(&exc))
		{
			(void) WRITE2("pow(0,0): DOMAIN error\n", 23);
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_POW_OVERFLOW:
	case KMATHERRF_POW_OVERFLOW:
	case KMATHERRL_POW_OVERFLOW:
		/* pow(x,y) overflow */
		exc.type = OVERFLOW;
		exc.name = funcname("pow");
		if (_LIB_VERSION == _SVID_)
		{
			exc.retval = HUGE;
			y *= 0.5;
			if (x < zero && rint(y) != y)
				exc.retval = -HUGE;
		} else
		{
			exc.retval = HUGE_VAL;
			y *= 0.5;
			if (x < zero && rint(y) != y)
				exc.retval = -HUGE_VAL;
		}
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_POW_UNDERFLOW:
	case KMATHERRF_POW_UNDERFLOW:
	case KMATHERRL_POW_UNDERFLOW:
		/* pow(x,y) underflow */
		exc.type = UNDERFLOW;
		exc.name = funcname("pow");
		exc.retval = zero;
		y *= 0.5;
		if (x < zero && rint(y) != y)
			exc.retval = -zero;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_POW_MINUS:
	case KMATHERRF_POW_MINUS:
	case KMATHERRL_POW_MINUS:
		/* -0**neg */
		exc.type = DOMAIN;
		exc.name = funcname("pow");
		if (_LIB_VERSION == _SVID_)
			exc.retval = zero;
		else
			exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("pow(0,neg): DOMAIN error\n", 25);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_POW_NONINT:
	case KMATHERRF_POW_NONINT:
	case KMATHERRL_POW_NONINT:
		/* neg**non-integral */
		exc.type = DOMAIN;
		exc.name = funcname("pow");
		if (_LIB_VERSION == _SVID_)
			exc.retval = zero;
		else
			exc.retval = zero / zero;	/* X/Open allow NaN */
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("neg**non-integral: DOMAIN error\n", 32);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_SINH:
	case KMATHERRF_SINH:
	case KMATHERRL_SINH:
		/* sinh(finite) overflow */
		exc.type = OVERFLOW;
		exc.name = funcname("sinh");
		if (_LIB_VERSION == _SVID_)
			exc.retval = ((x > zero) ? HUGE : -HUGE);
		else
			exc.retval = ((x > zero) ? HUGE_VAL : -HUGE_VAL);
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_SQRT:
	case KMATHERRF_SQRT:
	case KMATHERRL_SQRT:
		/* sqrt(x<0) */
		exc.type = DOMAIN;
		exc.name = funcname("sqrt");
		if (_LIB_VERSION == _SVID_)
			exc.retval = zero;
		else
			exc.retval = zero / zero;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("sqrt: DOMAIN error\n", 19);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_FMOD:
	case KMATHERRF_FMOD:
	case KMATHERRL_FMOD:
		/* fmod(x,0) */
		exc.type = DOMAIN;
		exc.name = funcname("fmod");
		if (_LIB_VERSION == _SVID_)
			exc.retval = x;
		else
			exc.retval = zero / zero;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("fmod:  DOMAIN error\n", 20);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_REMAINDER:
	case KMATHERRF_REMAINDER:
	case KMATHERRL_REMAINDER:
		/* remainder(x,0) */
		exc.type = DOMAIN;
		exc.name = funcname("remainder");
		exc.retval = zero / zero;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("remainder: DOMAIN error\n", 24);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_ACOSH:
	case KMATHERRF_ACOSH:
	case KMATHERRL_ACOSH:
		/* acosh(x<1) */
		exc.type = DOMAIN;
		exc.name = funcname("acosh");
		exc.retval = zero / zero;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("acosh: DOMAIN error\n", 20);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_ATANH_PLUSONE:
	case KMATHERRF_ATANH_PLUSONE:
	case KMATHERRL_ATANH_PLUSONE:
		/* atanh(|x|>1) */
		exc.type = DOMAIN;
		exc.name = funcname("atanh");
		exc.retval = zero / zero;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("atanh: DOMAIN error\n", 20);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_ATANH_ONE:
	case KMATHERRF_ATANH_ONE:
	case KMATHERRL_ATANH_ONE:
		/* atanh(|x|=1) */
		exc.type = SING;
		exc.name = funcname("atanh");
		exc.retval = x / zero;			/* sign(x)*inf */
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("atanh: SING error\n", 18);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_SCALB_OVERFLOW:
	case KMATHERRF_SCALB_OVERFLOW:
	case KMATHERRL_SCALB_OVERFLOW:
		/* scalb overflow; SVID also returns +-HUGE_VAL */
		exc.type = OVERFLOW;
		exc.name = funcname("scalb");
		exc.retval = x > zero ? HUGE_VAL : -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_SCALB_UNDERFLOW:
	case KMATHERRF_SCALB_UNDERFLOW:
	case KMATHERRL_SCALB_UNDERFLOW:
		/* scalb underflow */
		exc.type = UNDERFLOW;
		exc.name = funcname("scalb");
		exc.retval = copysign(zero, x);
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_J0_TLOSS:
	case KMATHERRF_J0_TLOSS:
	case KMATHERRL_J0_TLOSS:
		/* j0(|x|>X_TLOSS) */
		exc.type = TLOSS;
		exc.name = funcname("j0");
		exc.retval = zero;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2(exc.name, 2);
				(void) WRITE2(": TLOSS error\n", 14);
			}
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_Y0_TLOSS:
	case KMATHERRF_Y0_TLOSS:
	case KMATHERRL_Y0_TLOSS:
		/* y0(x>X_TLOSS) */
		exc.type = TLOSS;
		exc.name = funcname("y0");
		exc.retval = zero;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2(exc.name, 2);
				(void) WRITE2(": TLOSS error\n", 14);
			}
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_J1_TLOSS:
	case KMATHERRF_J1_TLOSS:
	case KMATHERRL_J1_TLOSS:
		/* j1(|x|>X_TLOSS) */
		exc.type = TLOSS;
		exc.name = funcname("j1");
		exc.retval = zero;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2(exc.name, 2);
				(void) WRITE2(": TLOSS error\n", 14);
			}
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_Y1_TLOSS:
	case KMATHERRF_Y1_TLOSS:
	case KMATHERRL_Y1_TLOSS:
		/* y1(x>X_TLOSS) */
		exc.type = TLOSS;
		exc.name = funcname("y1");
		exc.retval = zero;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2(exc.name, 2);
				(void) WRITE2(": TLOSS error\n", 14);
			}
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_JN_TLOSS:
	case KMATHERRF_JN_TLOSS:
	case KMATHERRL_JN_TLOSS:
		/* jn(|x|>X_TLOSS) */
		exc.type = TLOSS;
		exc.name = funcname("jn");
		exc.retval = zero;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2(exc.name, 2);
				(void) WRITE2(": TLOSS error\n", 14);
			}
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_YN_TLOSS:
	case KMATHERRF_YN_TLOSS:
	case KMATHERRL_YN_TLOSS:
		/* yn(x>X_TLOSS) */
		exc.type = TLOSS;
		exc.name = funcname("yn");
		exc.retval = zero;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2(exc.name, 2);
				(void) WRITE2(": TLOSS error\n", 14);
			}
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_TGAMMA_OVERFLOW:
	case KMATHERRF_TGAMMA_OVERFLOW:
	case KMATHERRL_TGAMMA_OVERFLOW:
		/* tgamma(finite) overflow */
		exc.type = OVERFLOW;
		exc.name = funcname("tgamma");
		exc.retval = copysign(HUGE_VAL, x);
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_TGAMMA_MINUS:
	case KMATHERRF_TGAMMA_MINUS:
	case KMATHERRL_TGAMMA_MINUS:
		/* tgamma(-integer) */
		exc.type = SING;
		exc.name = funcname("tgamma");
		if (_LIB_VERSION == _SVID_)
			exc.retval = HUGE_VAL;
		else
			exc.retval = __builtin_nan("");
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
		{
			if (_LIB_VERSION == _SVID_)
			{
				(void) WRITE2("gamma: SING error\n", 18);
			}
			__set_errno(EDOM);
		}
		break;
	case KMATHERR_TGAMMA_UNDERFLOW:
	case KMATHERRF_TGAMMA_UNDERFLOW:
	case KMATHERRL_TGAMMA_UNDERFLOW:
		/* tgamma(finite) underflow */
		exc.type = UNDERFLOW;
		exc.name = funcname("tgamma");
		exc.retval = 0;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
		{
			__set_errno(ERANGE);
		}
		break;
	case KMATHERR_POW_NAN:
	case KMATHERRF_POW_NAN:
	case KMATHERRL_POW_NAN:
		/* pow(NaN,0.0) */
		/* error only if _LIB_VERSION == _SVID_ & _XOPEN_ */
		exc.type = DOMAIN;
		exc.name = funcname("pow");
		exc.retval = x;
		if (_LIB_VERSION != _SVID_ && _LIB_VERSION != _XOPEN_)
			exc.retval = 1.0;
		else if (!matherr(&exc))
		{
			__set_errno(EDOM);
		}
		break;

	case KMATHERR_POW_ZEROMINUS:
	case KMATHERRF_POW_ZEROMINUS:
	case KMATHERRL_POW_ZEROMINUS:
		/* +0**neg */
		exc.type = DOMAIN;
		exc.name = funcname("pow");
		if (_LIB_VERSION == _SVID_)
			exc.retval = zero;
		else
			exc.retval = HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
			__set_errno(EDOM);
		break;
	case KMATHERR_EXP2_OVERFLOW:
	case KMATHERRF_EXP2_OVERFLOW:
	case KMATHERRL_EXP2_OVERFLOW:
		/* exp(finite) overflow */
		exc.type = OVERFLOW;
		exc.name = funcname("exp2");
		if (_LIB_VERSION == _SVID_)
			exc.retval = HUGE;
		else
			exc.retval = HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
			__set_errno(ERANGE);
		break;
	case KMATHERR_EXP2_UNDERFLOW:
	case KMATHERRF_EXP2_UNDERFLOW:
	case KMATHERRL_EXP2_UNDERFLOW:
		/* exp(finite) underflow */
		exc.type = UNDERFLOW;
		exc.name = funcname("exp2");
		exc.retval = zero;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
			__set_errno(ERANGE);
		break;

	case KMATHERR_EXP10_OVERFLOW:
	case KMATHERRF_EXP10_OVERFLOW:
	case KMATHERRL_EXP10_OVERFLOW:
		/* exp(finite) overflow */
		exc.type = OVERFLOW;
		exc.name = funcname("exp10");
		if (_LIB_VERSION == _SVID_)
			exc.retval = HUGE;
		else
			exc.retval = HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
			__set_errno(ERANGE);
		break;
	case KMATHERR_EXP10_UNDERFLOW:
	case KMATHERRF_EXP10_UNDERFLOW:
	case KMATHERRL_EXP10_UNDERFLOW:
		/* exp(finite) underflow */
		exc.type = UNDERFLOW;
		exc.name = funcname("exp10");
		exc.retval = zero;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
			__set_errno(ERANGE);
		break;
	case KMATHERR_LOG2_ZERO:
	case KMATHERRF_LOG2_ZERO:
	case KMATHERRL_LOG2_ZERO:
		/* log2(0) */
		exc.type = SING;
		exc.name = funcname("log2");
		if (_LIB_VERSION == _SVID_)
			exc.retval = -HUGE;
		else
			exc.retval = -HUGE_VAL;
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
			__set_errno(EDOM);
		break;
	case KMATHERR_LOG2_MINUS:
	case KMATHERRF_LOG2_MINUS:
	case KMATHERRL_LOG2_MINUS:
		/* log2(x<0) */
		exc.type = DOMAIN;
		exc.name = funcname("log2");
		if (_LIB_VERSION == _SVID_)
			exc.retval = -HUGE;
		else
			exc.retval = __builtin_nan("");
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
			__set_errno(EDOM);
		break;
	case KMATHERR_TGAMMA_ZERO:
	case KMATHERRF_TGAMMA_ZERO:
	case KMATHERRL_TGAMMA_ZERO:
		/* tgamma(+-0) */
		exc.type = SING;
		exc.name = funcname("tgamma");
		exc.retval = copysign(HUGE_VAL, x);
		if (_LIB_VERSION == _POSIX_)
			__set_errno(ERANGE);
		else if (!matherr(&exc))
			__set_errno(EDOM);
		break;
	case KMATHERR_COS_INF:
	case KMATHERRF_COS_INF:
	case KMATHERRL_COS_INF:
		/* cos(inf) */
		exc.type = DOMAIN;
		exc.name = funcname("cos");
		exc.retval = __builtin_nan("");
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
			__set_errno(EDOM);
		break;
	case KMATHERR_SIN_INF:
	case KMATHERRF_SIN_INF:
	case KMATHERRL_SIN_INF:
		/* sin(inf) */
		exc.type = DOMAIN;
		exc.name = funcname("sin");
		exc.retval = __builtin_nan("");
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
			__set_errno(EDOM);
		break;
	case KMATHERR_TAN_INF:
	case KMATHERRF_TAN_INF:
	case KMATHERRL_TAN_INF:
		/* tan(inf) */
		exc.type = DOMAIN;
		exc.name = funcname("tan");
		exc.retval = __builtin_nan("");
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
			__set_errno(EDOM);
		break;
	
	case KMATHERR_SCALB_INVALID:
	case KMATHERRF_SCALB_INVALID:
	case KMATHERRL_SCALB_INVALID:
		/* scalb(x, non-integer) */
		exc.type = DOMAIN;
		exc.name = funcname("scalb");
		exc.retval = __builtin_nan("");
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
			__set_errno(EDOM);
		break;
	
	case KMATHERR_ILOGB:
	case KMATHERRF_ILOGB:
	case KMATHERRL_ILOGB:
		/* ilogb(0 or NaN) */
		exc.type = DOMAIN;
		exc.name = funcname("ilogb");
		exc.retval = x == 0 ? FP_ILOGB0 : FP_ILOGBNAN;
		feraiseexcept(FE_INVALID);
		if (_LIB_VERSION == _POSIX_)
			__set_errno(EDOM);
		else if (!matherr(&exc))
			__set_errno(EDOM);
		break;
	
	case KMATHERRF_FIRST:
	case KMATHERRL_FIRST:
		break;
	}
	return exc.retval;
}
