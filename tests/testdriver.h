#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "testconfig.h"
#include "../fdlibm.h"

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
struct float96 {
	uint16_t exponent;
	uint16_t empty;
	uint32_t mantissa0;
	uint32_t mantissa1;
};
struct float64 {
	uint32_t msw;
	uint32_t lsw;
};
#define HEXCONSTE(v, exp, mant0, mant1) { { exp, 0, mant0, mant1 } }
#else
struct float96 {
	uint32_t mantissa1;
	uint32_t mantissa0;
	uint16_t exponent;
	uint16_t empty;
};
struct float64 {
	uint32_t lsw;
	uint32_t msw;
};
#define HEXCONSTE(v, exp, mant0, mant1) { { mant1, mant0, exp, 0 } }
#endif

typedef union _ld_union {
	struct float96 v;
	long double x;
	double d;
	float s;
} ld_union;

typedef unsigned int flag_t;

typedef struct { int line; ld_union x; ld_union e; flag_t flags; } test_f_f_data;
typedef struct { int line; ld_union x; int e; flag_t flags; } test_f_i_data;
typedef struct { int line; ld_union x; long e; flag_t flags; } test_f_l_data;
typedef struct { int line; ld_union x; long long e; flag_t flags; } test_f_ll_data;
typedef struct { int line; ld_union x; ld_union y; int e; flag_t flags; } test_ff_i_data;
typedef struct { int line; ld_union x; ld_union y; ld_union e; flag_t flags; } test_ff_f_data;
typedef struct { int line; ld_union a; ld_union b; ld_union c; ld_union e; flag_t flags; } test_fff_f_data;
typedef struct { int line; ld_union x; ld_union e; ld_union e2; flag_t flags; } test_f_ffp_data;
typedef struct { int line; ld_union x; ld_union r1; ld_union r2; flag_t flags; } test_f_ff_data;
typedef struct { int line; ld_union x; ld_union e; int exp; flag_t flags; } test_fip_f_data;
typedef struct { int line; ld_union x; int exp; ld_union e; flag_t flags; } test_fi_f_data;
typedef struct { int line; ld_union x; long exp; ld_union e; flag_t flags; } test_fl_f_data;
typedef struct { int line; int i; ld_union x; ld_union e; flag_t flags; } test_if_f_data;

#define FLAG_INEXACT             0x00000001 /* accept slightly inexact values */
#define FLAG_SUBNORM             0x00000002 /* test uses subnormal min values */
#define FLAG_MINVAL              0x00000004 /* test uses minimum values */
#define FLAG_FAIL_ARANYM         0x00000008 /* test currently fails on ARAnyM, but shouldn't */
#define FLAG_FAIL_HATARI         0x00000010 /* test currently fails on Hatari, but shouldn't */
#define FLAG_FAIL_STONX          0x00000020 /* test currently fails on STonX, but shouldn't */
#define FLAG_FAIL_JIT            0x00000040 /* test currently fails when using JIT */
#define FLAG_XFAIL_LINUX         0x00000080 /* test currently fails on linux */
#define FLAG_FAIL_X87            0x00000100 /* test currently fails when emulation uses x87 host FPU */
#define FLAG_INEXACT2            0x00000200 /* accept even more inexact values */
#define FLAG_INEXACT3            0x00000400 /* accept even more inexact values */
#define FLAG_INEXACT4            0x00000800 /* accept even more inexact values */
#define FLAG_INEXACT5            0x00001000 /* accept even more inexact values */
#define FLAG_INEXACT_SUBNORM     0x00002000 /* accept inexact subnorm results */
#define FLAG_FAIL_ARANYM2        0x00004000 /* test only occassionaly fails on ARAnyM */
#define FLAG_FAIL_SOFTFLOAT      0x00008000 /* test currently fails with softfloat version and has to be fixed */
#define FLAG_XFAIL               0x00010000 /* test currently fails and has to be fixed */
#define FLAG_IGNORE_ZEROSIGN     0x00020000 /* accept both +/-0 */

/*
 * exceptions that should be raised (not checked yet)
 */
#define INEXACT_EXCEPTION        0x01000000
#define NO_INEXACT_EXCEPTION     0x02000000
#define UNDERFLOW_EXCEPTION      0x04000000
#define OVERFLOW_EXCEPTION       0x08000000
#define INVALID_EXCEPTION        0x10000000
#define DIVIDE_BY_ZERO_EXCEPTION 0x20000000
#define UNDERFLOW_EXCEPTION_OK   0x00000000
#define OVERFLOW_EXCEPTION_OK    0x00000000
#define INVALID_EXCEPTION_OK     0x00000000
#define UNDERFLOW_EXCEPTION_BEFORE_ROUNDING 0

/* Values underflowing only for float.  */
#if defined(TEST_FLOAT)
# define UNDERFLOW_EXCEPTION_FLOAT	UNDERFLOW_EXCEPTION
# define UNDERFLOW_EXCEPTION_OK_FLOAT	UNDERFLOW_EXCEPTION_OK
#else
# define UNDERFLOW_EXCEPTION_FLOAT	0
# define UNDERFLOW_EXCEPTION_OK_FLOAT	0
#endif
/* Values underflowing only for double or types with a larger least
   positive normal value.  */
#if defined(TEST_FLOAT) || defined(TEST_DOUBLE) || (defined(TEST_LDOUBLE) && LDBL_MIN_EXP >= DBL_MIN_EXP)
# define UNDERFLOW_EXCEPTION_DOUBLE	UNDERFLOW_EXCEPTION
# define UNDERFLOW_EXCEPTION_OK_DOUBLE	UNDERFLOW_EXCEPTION_OK
#else
# define UNDERFLOW_EXCEPTION_DOUBLE	0
# define UNDERFLOW_EXCEPTION_OK_DOUBLE	0
#endif

/* not implemented yet */
#define ERRNO_ERANGE    0
#define ERRNO_EDOM      0
#define ERRNO_UNCHANGED 0

/* not implemented yet */
#define ROUND_TONEAREST  0
#define ROUND_TOWARDZERO 0
#define ROUND_DOWNWARD   0
#define ROUND_UPWARD     0


#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))


/*
 * Replace some conversion functions from libgcc.
 * The default implementation will generate NaNs
 * when converting INFs, but we need to preverse them
 */

union double_long {
	struct {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
		int32_t upper;
		uint32_t lower;
#else
		uint32_t lower;
		int32_t upper;
#endif
	} l;
	double d;
};

#ifdef __NO_LONG_DOUBLE_MATH

#define my_truncxfdf2(x) x
#define my_extenddfxf2(x) x

#else

union long_double_long {
	long double ld;
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	struct
	{
	    int16_t sign_exponent;
	    uint16_t empty;
		uint32_t middle;
		uint32_t lower;
	} l;
#else
	struct
	{
		uint32_t lower;
		uint32_t middle;
	    int16_t sign_exponent;
	    uint16_t empty;
	} l;
#endif
};

/* convert long double to double */
double my_truncxfdf2(long double ld);
double my_truncxfdf2(long double ld)
{
	int32_t exp;
	union double_long dl;
	union long_double_long ldl;

	ldl.ld = ld;

	dl.l.upper = ((int32_t)(ldl.l.sign_exponent & 0x8000)) << 16;
	exp = ldl.l.sign_exponent & 0x7FFF;
	if (exp == 0 && !(ldl.l.middle & 0x7FFFFFFFL) && !ldl.l.lower)
	{
		dl.l.lower = 0;
	} else
	{
		if (exp == IEEE854_LONG_DOUBLE_MAXEXP)
		{
			exp = IEEE754_DOUBLE_MAXEXP;
		} else
		{
	 		exp = exp - IEEE854_LONG_DOUBLE_BIAS + IEEE754_DOUBLE_BIAS;
			if (exp >= IEEE754_DOUBLE_MAXEXP)
				exp = IEEE754_DOUBLE_MAXEXP - 1;
	 	}
	 	dl.l.upper |= exp << (32 - (11 + 1));
		/* +1-1: add one for sign bit, but take one off for explicit-integer-bit */
		dl.l.upper |= (ldl.l.middle & 0x7FFFFFFFL) >> (11 + 1 - 1);
		dl.l.lower = (ldl.l.middle & 0x7FFFFFFFL) << (32 - (11 + 1 - 1));
		dl.l.lower |= ldl.l.lower >> (11 + 1 - 1);
	}
	
	/* printf("%04x:%08x:%08x %.22Le -> %08x:%08x %.22e\n", (uint16_t)ldl.l.sign_exponent, ldl.l.middle, ldl.l.lower, ldl.ld, dl.l.upper, dl.l.lower, dl.d); */
	return dl.d;
}


/* convert double to long double */
long double my_extenddfxf2(double d);
long double my_extenddfxf2(double d)
{
	union double_long dl;
	union long_double_long ldl;
	int32_t exp;

	dl.d = d;

	ldl.l.sign_exponent = (dl.l.upper & 0x80000000L) >> 16;
	ldl.l.empty = 0;
	if ((dl.l.upper & 0x7fffffffL) == 0 && !dl.l.lower)
	{
		ldl.l.middle = 0;
		ldl.l.lower = 0;
	} else
	{
		exp = (((dl.l.upper) >> 20) & 0x7FFL);
		if (exp == IEEE754_DOUBLE_MAXEXP)
		{
			exp = IEEE854_LONG_DOUBLE_MAXEXP;
		} else
		{
			exp = exp - IEEE754_DOUBLE_BIAS + IEEE854_LONG_DOUBLE_BIAS;
		}
		ldl.l.sign_exponent |= exp;
		ldl.l.middle = (1L << 31L);
		/* 31-20: # mantissa bits in ldl.l.middle - # mantissa bits in dl.l.upper */
		ldl.l.middle |= (dl.l.upper & 0xFFFFFL) << (31 - 20);
		/* 1+20: explicit-integer-bit + # mantissa bits in dl.l.upper */
		ldl.l.middle |= dl.l.lower >> (1 + 20);
		/* 32 - 21: # bits of dl.l.lower in ldl.l.middle */
		ldl.l.lower = dl.l.lower << (32 - 21);
		if (exp == IEEE854_LONG_DOUBLE_MAXEXP && ldl.l.lower != 0)
		{
			ldl.l.lower |= 0x7ff;
		}
	}

	/* printf("%08x:%08x %.22e -> %04x:%08x:%08x %.22Le\n", dl.l.upper, dl.l.lower, dl.d, (uint16_t)ldl.l.sign_exponent, ldl.l.middle, ldl.l.lower, ldl.ld); */
	return ldl.ld;
}


/* convert float to long double */
long double my_extendsfxf2(float f);
long double my_extendsfxf2(float f)
{
	return f; /* TODO */
}


/* convert long double to float */
float my_truncxfsf2(long double x);
float my_truncxfsf2(long double x)
{
	return x; /* TODO */
}


#endif /* __NO_LONG_DOUBLE_MATH */

/* convert float to double */
double my_extendsfxd2(float f);
double my_extendsfxd2(float f)
{
	return f; /* TODO */
}


#if defined(__AMIGA__) && ALLOW_SUBNORM_MIN
#include <exec/execbase.h>
#undef ALLOW_SUBNORM_MIN
/*
 * 040/060 does not handle denormals like 6888x
 */
#define ALLOW_SUBNORM_MIN (!is_040_or_060())
static __inline int is_040_or_060(void)
{
	struct ExecBase *AbsSysBase = *((struct ExecBase **)4);
	
	return (AbsSysBase->AttnFlags & (AFF_68040|AFF_68060)) != 0;
}
#endif


int numtests;
int testonly;
int jit_loops = JIT_LOOPS;

static enum {
	EMULATOR_NONE,
	EMULATOR_ARANYM,
	EMULATOR_HATARI,
	EMULATOR_STONX
} emulator;


#define CHECK(var) \
	this_fail = var; \
	if (var) \
	{ \
		fprintf(stderr, "%s:%d: test %d(%d): failed" CR "\n", __FILE__, __LINE__, numtests, i); \
		status |= this_fail != 0; \
	}


#ifdef TEST_DOUBLE
__attribute__((__noinline__))
static int skip_double_test(const ld_union *x, flag_t flags)
{
	/*
	 * skip tests involving sub-normals for now,
	 * they are currently only designed for long doubles
	 */
	if (flags & (FLAG_SUBNORM|FLAG_MINVAL))
		return 1;
	/*
	 * allow tests involving infs and nans
	 */
	if ((x->v.exponent & IEEE854_LONG_DOUBLE_MAXEXP) == IEEE854_LONG_DOUBLE_MAXEXP)
		return 0;
	/*
	 * ... and zeroes
	 */
	if ((x->v.exponent & IEEE854_LONG_DOUBLE_MAXEXP) == 0)
	{
		/* ... but not for min values */
		if (x->v.mantissa0 == 0x80000000UL && x->v.mantissa1 == 0x00000000)
			return 1;
		return 0;
	}
	/*
	 * skip tests whos exponent exceeds the range for doubles
	 */
	if (((x->v.exponent & IEEE854_LONG_DOUBLE_MAXEXP) - IEEE854_LONG_DOUBLE_BIAS + IEEE754_DOUBLE_BIAS) <= 0)
		return 1;
	if (((x->v.exponent & IEEE854_LONG_DOUBLE_MAXEXP) - IEEE854_LONG_DOUBLE_BIAS) >= (IEEE754_DOUBLE_MAXEXP - IEEE754_DOUBLE_BIAS))
		return 1;
	
	/* allow anything else */
	return 0;
}
#define SKIP_TEST(x, flags) skip_double_test(&(x), flags)
#endif

#ifdef TEST_FLOAT
__attribute__((__noinline__))
static int skip_float_test(const ld_union *x, flag_t flags)
{
	/*
	 * skip tests involving sub-normals for now,
	 * they are currently only designed for long doubles
	 */
	if (flags & (FLAG_SUBNORM|FLAG_MINVAL))
		return 1;
	/*
	 * allow tests involving infs and nans
	 */
	if ((x->v.exponent & IEEE854_LONG_DOUBLE_MAXEXP) == IEEE854_LONG_DOUBLE_MAXEXP)
		return 0;
	/*
	 * ... and zeroes
	 */
	if ((x->v.exponent & IEEE854_LONG_DOUBLE_MAXEXP) == 0)
	{
		/* ... but not for min values */
		if (x->v.mantissa0 == 0x80000000UL && x->v.mantissa1 == 0x00000000)
			return 1;
		return 0;
	}
	/*
	 * skip tests whos exponent exceeds the range for doubles
	 */
	if (((x->v.exponent & IEEE854_LONG_DOUBLE_MAXEXP) - IEEE854_LONG_DOUBLE_BIAS + IEEE754_FLOAT_BIAS) <= 0)
		return 1;
	if (((x->v.exponent & IEEE854_LONG_DOUBLE_MAXEXP) - IEEE854_LONG_DOUBLE_BIAS) >= (IEEE754_FLOAT_MAXEXP - IEEE754_FLOAT_BIAS))
		return 1;
	
	/* allow anything else */
	return 0;
}
#define SKIP_TEST(x, flags) skip_float_test(&(x), flags)
#endif

#ifdef TEST_LDOUBLE
#define SKIP_TEST(x, flags) 0
#endif


#if !defined(NO_PRINTF_DOUBLE)
/*
 * libcmini does not correctly print inf & NaN;
 * mintlib seems to be buggy with printing subnormal & low values
 */
static const char *print_ld(const ld_union *x)
{
	int sign = (x->v.exponent & 0x8000) != 0;
	static int nbuf = 0;
	static char buf[2][8000];		/* yes, it has to be that large; mintlib sometimes prints lots of digits */
	char *p;
	ld_union r;
	
	if ((x->v.exponent & IEEE854_LONG_DOUBLE_MAXEXP) == 0)
	{
		/*
		 * zero, or subnormal
		 */
		if (x->v.mantissa0 == 0 && x->v.mantissa1 == 0)
			return sign ? "-zero" : "+zero";
		return sign ? "-denorm" : "+denorm";
	} else if ((x->v.exponent & IEEE854_LONG_DOUBLE_MAXEXP) == IEEE854_LONG_DOUBLE_MAXEXP)
	{
		/* inf or nan */
 		if ((x->v.mantissa0 & 0x7fffffff) == 0 && x->v.mantissa1 == 0)
			return sign ? "-inf" : "+inf";
		if ((x->v.mantissa0 & 0x40000000L) == 0)
			return sign ? "-sNaN" : "+sNaN";
		return sign ? "-NaN" : "+NaN";
	} else if ((x->v.exponent & IEEE854_LONG_DOUBLE_MAXEXP) <= 0x0001 && !(x->v.mantissa0 & 0x80000000))
	{
		/*
		 * subnormal
		 */
		return sign ? "-denorm" : "+denorm";
	}
	/*
	 * hopefully some printable value now
	 */
	p = buf[nbuf];
	nbuf++;
	if (nbuf >= 2)
		nbuf = 0;
	/*
	 * avoid problems with impossible patterns
	 */
	r = *x;
	r.v.mantissa0 |= 0x80000000L;
#ifdef __AMIGA__
	sprintf(p, "%.22e", (double)r.x);
#else
	sprintf(p, "%.22Le", r.x);
#endif
	return p;
}

__attribute__((__noinline__))
static int check_fp(uint16_t exponent, uint32_t mant0, uint32_t mant1, const ld_union *f, int flags, int i, const char *file, int line)
{
	ld_union r;
	
#ifdef TEST_DOUBLE
	/* mask off the precision for double */
	if ((exponent & IEEE854_LONG_DOUBLE_MAXEXP) != IEEE854_LONG_DOUBLE_MAXEXP)
		mant1 &= ~((uint32_t)0x7ff);
#endif
	if (exponent == f->v.exponent && mant0 == f->v.mantissa0 && mant1 == f->v.mantissa1)
		return 0;
	if ((exponent & IEEE854_LONG_DOUBLE_MAXEXP) == 0 &&
		(flags & FLAG_IGNORE_ZEROSIGN) &&
		(f->v.exponent & IEEE854_LONG_DOUBLE_MAXEXP) == 0 &&
		mant0 == f->v.mantissa0 && mant1 == f->v.mantissa1)
		return 0;

	/*
	 * if we expect a nan, accept *any* value that would be interpreted as NaN
	 * (that is, any non-zero bit pattern in the mantissa)
	 */
	if ((exponent & IEEE854_LONG_DOUBLE_MAXEXP) == IEEE854_LONG_DOUBLE_MAXEXP && (f->v.exponent & IEEE854_LONG_DOUBLE_MAXEXP) == IEEE854_LONG_DOUBLE_MAXEXP)
	{
		if (((mant0 & 0x7fffffff) != 0 || mant1 != 0) && ((f->v.mantissa0 & 0x7fffffff) != 0 || f->v.mantissa1 != 0))
			return 0;
		if (((mant0 & 0x7fffffff) == 0 && mant1 == 0) && ((f->v.mantissa0 & 0x7fffffff) == 0 && f->v.mantissa1 == 0))
			return 0;
	}
	if (flags & (FLAG_INEXACT|FLAG_INEXACT2|FLAG_INEXACT3|FLAG_INEXACT4|FLAG_INEXACT5|FLAG_INEXACT_SUBNORM))
	{
#if ALLOW_INEXACT
		/*
		 * allow for inexactness in least-significant 2 bits
		 */
		int expdiff = (exponent & IEEE854_LONG_DOUBLE_MAXEXP) - (f->v.exponent & IEEE854_LONG_DOUBLE_MAXEXP);

		if ((flags & FLAG_INEXACT_SUBNORM) && (exponent & IEEE854_LONG_DOUBLE_MAXEXP) == 0 && exponent == f->v.exponent)
		{
			if ((f->v.mantissa0 & 0x7fffffff) == (mant0 & 0x7fffffff))
				return 0;
		} else if ((flags & FLAG_INEXACT_SUBNORM) && (exponent & IEEE854_LONG_DOUBLE_MAXEXP) == 1 && exponent == f->v.exponent)
		{
			if ((f->v.mantissa0 & 0x7fffffff) == (mant0 & 0x7fffffff) && (f->v.mantissa1 & 0xffff0000) == (mant1 & 0xffff0000))
				return 0;
		} else if ((flags & FLAG_INEXACT4) && expdiff == 0)
		{
			ld_union diffval, t;

			/*
			 * used for cos(x) with x near pi, which should yield values close to zero
			 */
			t = *f;
			diffval.v.mantissa0 = mant0;
			diffval.v.mantissa1 = mant1;
			diffval.v.exponent = 0x3fff;
			t.v.exponent = 0x3fff - expdiff;
			diffval.x -= t.x;
			diffval.v.exponent &= IEEE854_LONG_DOUBLE_MAXEXP;
			expdiff = 0x3fff - diffval.v.exponent;
			if (expdiff >= 2)
				return 0;
		} else if (expdiff == 1 || expdiff == -1)
		{
			ld_union diffval, t;

			t = *f;
			diffval.v.mantissa0 = mant0;
			diffval.v.mantissa1 = mant1;
			diffval.v.exponent = 0x3fff;
			t.v.exponent = 0x3fff - expdiff;
			diffval.x -= t.x;
			diffval.v.exponent &= IEEE854_LONG_DOUBLE_MAXEXP;
			expdiff = 0x3fff - diffval.v.exponent;
#if defined(TEST_LDOUBLE) && LDBL_MANT_DIG >= 64
			if (expdiff >= 60)
				return 0;
			if (flags & FLAG_INEXACT2)
				if (expdiff >= 57)
					return 0;
			if (flags & FLAG_INEXACT3)
				if (expdiff >= 54)
					return 0;
			if (flags & FLAG_INEXACT5)
				if (expdiff >= 51)
					return 0;
#endif
#if defined(TEST_DOUBLE) || (defined(TEST_LDOUBLE) && LDBL_MANT_DIG < 64)
			if (expdiff >= 49)
				return 0;
			if (flags & FLAG_INEXACT2)
				if (expdiff >= 46)
					return 0;
			if (flags & FLAG_INEXACT3)
				if (expdiff >= 43)
					return 0;
			if (flags & FLAG_INEXACT5)
				if (expdiff >= 40)
					return 0;
#endif
		} else if (expdiff == 0)
		{
			uint32_t diff;

			if (mant1 > f->v.mantissa1)
			{
				diff = mant1 - f->v.mantissa1;
			} else
			{
				diff = f->v.mantissa1 - mant1;
			}
			if (mant0 == (f->v.mantissa0 + 1))
				diff = -diff;
			else if (mant0 != f->v.mantissa0)
				diff = 0x7fffffff;
#if defined(TEST_LDOUBLE) && LDBL_MANT_DIG >= 64
			if (diff <= 4)
				return 0;
			if (flags & FLAG_INEXACT2)
				if (diff <= 32)
					return 0;
			if (flags & FLAG_INEXACT3)
				if (diff <= 128)
					return 0;
			if (flags & FLAG_INEXACT5)
				if (diff <= 1280)
					return 0;
#endif
#if defined(TEST_DOUBLE) || (defined(TEST_LDOUBLE) && LDBL_MANT_DIG < 64)
			if (diff <= (4 << 11))
				return 0;
			if (flags & FLAG_INEXACT2)
				if (diff <= (32 << 11))
					return 0;
			if (flags & FLAG_INEXACT3)
				if (diff <= (128 << 11))
					return 0;
			if (flags & FLAG_INEXACT5)
				if (diff <= (1280 << 11))
					return 0;
#endif
		}
#endif
	}
	if (flags & FLAG_SUBNORM)
	{
		if (!ALLOW_SUBNORM_MIN)
			return 0;
	}
	if (flags & FLAG_MINVAL)
	{
		if (!ALLOW_MINVAL)
			return 0;
	}
	if (flags & FLAG_XFAIL_LINUX)
	{
#ifdef __linux__
		return 0;
#endif
	}
	if (flags & FLAG_FAIL_X87)
	{
#if defined(__i386__) || defined(__x86_64__)
		return 0;
#endif
	}
	if (flags & (FLAG_FAIL_STONX|FLAG_FAIL_X87))
	{
		if (emulator == EMULATOR_STONX)
			return 0;
	}
#ifndef __HAVE_68881__
	if (flags & (FLAG_FAIL_SOFTFLOAT))
	{
		return 0;
	}
#endif
	if (flags & (FLAG_FAIL_ARANYM|FLAG_FAIL_ARANYM2|FLAG_FAIL_X87))
	{
		if (emulator == EMULATOR_ARANYM)
			return 0;
	}
	if (flags & (FLAG_XFAIL))
	{
		return 0;
	}
	if (flags & FLAG_FAIL_HATARI)
	{
		if (emulator == EMULATOR_HATARI)
			return 0;
	}
	r.v.exponent = exponent;
	r.v.mantissa0 = mant0;
	r.v.mantissa1 = mant1;
	fprintf(stderr, "%s:%d: test %d(%d):" CR "\n"
	                "  expected %04x:%08lx:%08lx (%s)" CR "\n"
	                "  got      %04x:%08lx:%08lx (%s)" CR "\n\n",
		file, line, numtests, i,
		exponent, (unsigned long)mant0, (unsigned long)mant1,
		print_ld(&r),
		f->v.exponent, (unsigned long)f->v.mantissa0, (unsigned long)f->v.mantissa1,
		print_ld(f));
	return 1;
}

#endif



#if defined(TEST_FUNC_F_I) || defined(TEST_FUNC_FF_I) || defined(TEST_FUNC_FIP_F)
__attribute__((__noinline__))
static int check_int(int e, int f, int flags, int i, const char *file, int line)
{
	if (e == f)
		return 0;

	/*
	 * if we expect a nan, accept *any* value that would be interpreted as NaN
	 * (that is, any non-zero bit pattern in the mantissa)
	 */
	if (flags & FLAG_SUBNORM)
	{
		if (!ALLOW_SUBNORM_MIN)
			return 0;
	}
	if (flags & FLAG_MINVAL)
	{
		if (!ALLOW_MINVAL)
			return 0;
	}
	if (flags & FLAG_XFAIL_LINUX)
	{
#ifdef __linux__
		return 0;
#endif
	}
	if (flags & FLAG_FAIL_X87)
	{
#if defined(__i386__) || defined(__x86_64__)
		return 0;
#endif
	}
	if (flags & (FLAG_FAIL_STONX|FLAG_FAIL_X87))
	{
		if (emulator == EMULATOR_STONX)
			return 0;
	}
#ifndef __HAVE_68881__
	if (flags & (FLAG_FAIL_SOFTFLOAT))
	{
		return 0;
	}
#endif
	if (flags & (FLAG_FAIL_ARANYM|FLAG_FAIL_ARANYM2|FLAG_FAIL_X87))
	{
		if (emulator == EMULATOR_ARANYM)
			return 0;
	}
	if (flags & (FLAG_XFAIL))
	{
		return 0;
	}
	if (flags & FLAG_FAIL_HATARI)
	{
		if (emulator == EMULATOR_HATARI)
			return 0;
	}
	fprintf(stderr, "%s:%d: test %d(%d): expected %d got %d" CR "\n",
		file, line, numtests, i,
		e, f);
	return 1;
}
#endif


#if defined(TEST_FUNC_F_L)
__attribute__((__noinline__))
static int check_long(long e, long f, int flags, int i, const char *file, int line)
{
	if (e == f)
		return 0;

	/*
	 * if we expect a nan, accept *any* value that would be interpreted as NaN
	 * (that is, any non-zero bit pattern in the mantissa)
	 */
	if (flags & FLAG_SUBNORM)
	{
		if (!ALLOW_SUBNORM_MIN)
			return 0;
	}
	if (flags & FLAG_MINVAL)
	{
		if (!ALLOW_MINVAL)
			return 0;
	}
	if (flags & FLAG_XFAIL_LINUX)
	{
#ifdef __linux__
		return 0;
#endif
	}
	if (flags & FLAG_FAIL_X87)
	{
#if defined(__i386__) || defined(__x86_64__)
		return 0;
#endif
	}
	if (flags & (FLAG_FAIL_STONX|FLAG_FAIL_X87))
	{
		if (emulator == EMULATOR_STONX)
			return 0;
	}
#ifdef __HAVE_68881__
	if (flags & (FLAG_FAIL_ARANYM|FLAG_FAIL_ARANYM2|FLAG_FAIL_X87))
	{
		if (emulator == EMULATOR_ARANYM)
			return 0;
	}
#else
	if (flags & (FLAG_FAIL_SOFTFLOAT))
	{
		return 0;
	}
#endif
	if (flags & (FLAG_XFAIL))
	{
		return 0;
	}
	if (flags & FLAG_FAIL_HATARI)
	{
		if (emulator == EMULATOR_HATARI)
			return 0;
	}
	fprintf(stderr, "%s:%d: test %d(%d): expected %ld got %ld" CR "\n",
		file, line, numtests, i,
		e, f);
	return 1;
}
#endif


#if defined(TEST_FUNC_F_LL)
__attribute__((__noinline__))
static int check_longlong(long long e, long long f, int flags, int i, const char *file, int line)
{
	if (e == f)
		return 0;

	/*
	 * if we expect a nan, accept *any* value that would be interpreted as NaN
	 * (that is, any non-zero bit pattern in the mantissa)
	 */
	if (flags & FLAG_SUBNORM)
	{
		if (!ALLOW_SUBNORM_MIN)
			return 0;
	}
	if (flags & FLAG_MINVAL)
	{
		if (!ALLOW_MINVAL)
			return 0;
	}
	if (flags & FLAG_XFAIL_LINUX)
	{
#ifdef __linux__
		return 0;
#endif
	}
	if (flags & FLAG_FAIL_X87)
	{
#if defined(__i386__) || defined(__x86_64__)
		return 0;
#endif
	}
	if (flags & (FLAG_FAIL_STONX|FLAG_FAIL_X87))
	{
		if (emulator == EMULATOR_STONX)
			return 0;
	}
#ifdef __HAVE_68881__
	if (flags & (FLAG_FAIL_ARANYM|FLAG_FAIL_ARANYM2|FLAG_FAIL_X87))
	{
		if (emulator == EMULATOR_ARANYM)
			return 0;
	}
#else
	if (flags & (FLAG_FAIL_SOFTFLOAT))
	{
		return 0;
	}
#endif
	if (flags & (FLAG_XFAIL))
	{
		return 0;
	}
	if (flags & FLAG_FAIL_HATARI)
	{
		if (emulator == EMULATOR_HATARI)
			return 0;
	}
	fprintf(stderr, "%s:%d: test %d(%d): expected %ld got %ld" CR "\n",
		file, line, numtests, i,
		e, f);
	return 1;
}
#endif


#define EXPECT_FP_FLAGS(f, exp, mant0, mant1, flags) \
	this_fail = check_fp(exp, mant0, mant1, &f, flags, i, __FILE__, __LINE__); \
	status |= this_fail != 0

#define EXPECT_FP(f, exp, mant0, mant1) EXPECT_FP_FLAGS(f, exp, mant0, mant1, 0)

#define EXPECT_FP_CONST(f, c) EXPECT_FP(f, c.v.exponent, c.v.mantissa0, c.v.mantissa1)

#define EXPECT_FP_CONST_FLAGS(f, c, flags) EXPECT_FP_FLAGS(f, c.v.exponent, c.v.mantissa0, c.v.mantissa1, flags)

#ifdef __HAVE_68881__
#define EXPECT_FAIL_68881 if (((flags) & (FLAG_FAIL_ARANYM|FLAG_FAIL_ARANYM2|FLAG_FAIL_X87)) && (emulator == EMULATOR_ARANYM)) this_fail = 0
#else
#define EXPECT_FAIL_68881
#endif

#define EXPECT_FLAGS(r, v, flags) \
	this_fail = ((r) != (v)); \
	if (((flags) & (FLAG_FAIL_STONX|FLAG_FAIL_X87)) && (emulator == EMULATOR_STONX)) this_fail = 0; \
	EXPECT_FAIL_68881; \
	if (((flags) & (FLAG_FAIL_HATARI|FLAG_FAIL_X87)) && (emulator == EMULATOR_HATARI)) this_fail = 0; \
	if (this_fail) \
	{ \
		fprintf(stderr, "%s:%d: test %d(%d): expected 0x%08lx, got 0x%08lx" CR "\n", __FILE__, __LINE__, numtests, i, v, r); \
		status |= this_fail != 0; \
	}

#define EXPECT(r, v) EXPECT_FLAGS(r, v, 0)


#ifdef TEST_FUNC_F_F

/*
 * body for monadic operations:
 * fabs, facos, fasin, fatan, fatanh, fcos, fcosh,
 * fetox, fetoxm1, fgetexp, fgetman, fint, fintrz,
 * flog10, flog2, flogn, flognp1, fneg,
 * fsinh, fsqrt, ftan, ftanh, ftentox, ftst, ftwotox
 */
#ifdef TEST_LDOUBLE
#define TEST_OP_BODY_F_F(_x) fp0.x = TEST_FUNC_F_F(_x.x)
#endif
#ifdef TEST_DOUBLE
#define TEST_OP_BODY_F_F(_x) fp0.x = my_extenddfxf2(TEST_FUNC_F_F(my_truncxfdf2(_x.x)))
#endif
#ifdef TEST_FLOAT
#define TEST_OP_BODY_F_F(_x) fp0.x = my_extendsfxf2(TEST_FUNC_F_F(my_truncxfsf2(_x.x)))
#endif

#define TEST_CONST_FLAGS(_x, e, flags) \
	++numtests; \
	if (!SKIP_TEST(_x, flags)) \
	{ \
		for (i = 0; i < jit_loops; i++) \
		{ \
			TEST_OP_BODY_F_F(_x); \
			EXPECT_FP_CONST_FLAGS(fp0, e, flags); \
			if (this_fail) break; \
		} \
	}

#define TEST_CONST(v, e) TEST_CONST_FLAGS(v, e, 0)

#endif


#ifdef TEST_FUNC_F_I
/*
 * body for monadic operations returning int:
 * ilogb
 */

#ifdef TEST_LDOUBLE
#define TEST_OP_BODY_F_I(_x) x = TEST_FUNC_F_I(_x.x)
#endif
#ifdef TEST_DOUBLE
#define TEST_OP_BODY_F_I(_x) x = TEST_FUNC_F_I(my_truncxfdf2(_x.x))
#endif
#ifdef TEST_FLOAT
#define TEST_OP_BODY_F_I(_x) x = TEST_FUNC_F_I(my_truncxfsf2(_x.x))
#endif
#endif


#ifdef TEST_FUNC_FF_I
/*
 * body for dyadic operations returning int:
 * isunordered etc.
 */

#ifdef TEST_LDOUBLE
#define TEST_OP_BODY_FF_I(_x, _y) x = TEST_FUNC_FF_I(_x.x, _y.x)
#endif
#ifdef TEST_DOUBLE
#define TEST_OP_BODY_FF_I(_x, _y) x = TEST_FUNC_FF_I(my_truncxfdf2(_x.x), my_truncxfdf2(_y.x))
#endif
#ifdef TEST_FLOAT
#define TEST_OP_BODY_FF_I(_x, _y) x = TEST_FUNC_FF_I(my_truncxfsf2(_x.x), my_truncxfsf2(_y.x))
#endif
#endif


#ifdef TEST_FUNC_F_L
/*
 * body for monadic operations returning long:
 * lround, lrint
 */

#ifdef TEST_LDOUBLE
#define TEST_OP_BODY_F_L(_x) x = TEST_FUNC_F_L(_x.x)
#endif
#ifdef TEST_DOUBLE
#define TEST_OP_BODY_F_L(_x) x = TEST_FUNC_F_L(my_truncxfdf2(_x.x))
#endif
#ifdef TEST_FLOAT
#define TEST_OP_BODY_F_L(_x) x = TEST_FUNC_F_L(my_truncxfsf2(_x.x))
#endif
#endif


#ifdef TEST_FUNC_F_LL
/*
 * body for monadic operations returning long long:
 * llrint
 */

#ifdef TEST_LDOUBLE
#define TEST_OP_BODY_F_LL(_x) x = TEST_FUNC_F_LL(_x.x)
#endif
#ifdef TEST_DOUBLE
#define TEST_OP_BODY_F_LL(_x) x = TEST_FUNC_F_LL(my_truncxfdf2(_x.x))
#endif
#ifdef TEST_FLOAT
#define TEST_OP_BODY_F_LL(_x) x = TEST_FUNC_F_LL(my_truncxfsf2(_x.x))
#endif
#endif


#ifdef TEST_FUNC_FF_F
/*
 * body for dyadic operations:
 * fadd, fcmp, fdiv, fmod, fmul, frem,
 * fscale, fsgldiv, fsglmul, fsub
 */
#ifdef TEST_LDOUBLE
#define TEST_OP_BODY_FF_F(_x, _y) fp0.x = TEST_FUNC_FF_F(_x.x, _y.x)
#endif
#ifdef TEST_DOUBLE
#define TEST_OP_BODY_FF_F(_x, _y) fp0.x = my_extenddfxf2(TEST_FUNC_FF_F(my_truncxfdf2(_x.x), my_truncxfdf2(_y.x)))
#endif
#ifdef TEST_FLOAT
#define TEST_OP_BODY_FF_F(_x, _y) fp0.x = my_extendsfxf2(TEST_FUNC_FF_F(my_truncxfsf2(_x.x), my_truncxfsf2(_y.x)))
#endif

#define TEST_CONST_FLAGS(_x, _y, e, flags) \
	++numtests; \
	if (!SKIP_TEST(_x, flags) && !SKIP_TEST(_y, flags)) \
	{ \
		for (i = 0; i < jit_loops; i++) \
		{ \
			TEST_OP_BODY_FF_F(_x, _y); \
			EXPECT_FP_CONST_FLAGS(fp0, e, flags); \
			if (this_fail) break; \
		} \
	}

#endif


#ifdef TEST_FUNC_FFF_F
/*
 * body for triadic operations:
 * fma
 */
#ifdef TEST_LDOUBLE
#define TEST_OP_BODY_FFF_F(_x, _y, _z) fp0.x = TEST_FUNC_FFF_F(_x.x, _y.x, _z.x)
#endif
#ifdef TEST_DOUBLE
#define TEST_OP_BODY_FFF_F(_x, _y, _z) fp0.x = my_extenddfxf2(TEST_FUNC_FFF_F(my_truncxfdf2(_x.x), my_truncxfdf2(_y.x), my_truncxfdf2(_z.x)))
#endif
#ifdef TEST_FLOAT
#define TEST_OP_BODY_FFF_F(_x, _y, _z) fp0.x = my_extendsfxf2(TEST_FUNC_FFF_F(my_truncxfsf2(_x.x), my_truncxfsf2(_y.x), my_truncxfsf2(_z.x)))
#endif
#endif


#ifdef TEST_FUNC_F_FFP
#ifdef TEST_LDOUBLE
#define TEST_OP_BODY_F_FFP(_x) r.x = TEST_FUNC_F_FFP(_x.x, &r2.x)
#endif
#ifdef TEST_DOUBLE
#define TEST_OP_BODY_F_FFP(_x) r.x = my_extenddfxf2(TEST_FUNC_F_FFP(my_truncxfdf2(_x.x), &r2.d)), r2.x = my_extenddfxf2(r2.d)
#endif
#ifdef TEST_FLOAT
#define TEST_OP_BODY_F_FFP(_x) r.x = my_extendsfxf2(TEST_FUNC_F_FFP(my_truncxfsf2(_x.x), &r2.s)), r2.x = my_extendsfxf2(r2.s)
#endif
#endif


#ifdef TEST_FUNC_FIP_F
#ifdef TEST_LDOUBLE
#define TEST_OP_BODY_FIP_F(_x) fp0.x = TEST_FUNC_FIP_F(_x.x, &exp)
#endif
#ifdef TEST_DOUBLE
#define TEST_OP_BODY_FIP_F(_x) fp0.x = my_extenddfxf2(TEST_FUNC_FIP_F(my_truncxfdf2(_x.x), &exp))
#endif
#ifdef TEST_FLOAT
#define TEST_OP_BODY_FIP_F(_x) fp0.x = my_extendsfxf2(TEST_FUNC_FIP_F(my_truncxfsf2(_x.x), &exp))
#endif
#endif


#ifdef TEST_FUNC_FI_F
#ifdef TEST_LDOUBLE
#define TEST_OP_BODY_FI_F(_x, exp) fp0.x = TEST_FUNC_FI_F(_x.x, exp)
#endif
#ifdef TEST_DOUBLE
#define TEST_OP_BODY_FI_F(_x, exp) fp0.x = my_extenddfxf2(TEST_FUNC_FI_F(my_truncxfdf2(_x.x), exp))
#endif
#ifdef TEST_FLOAT
#define TEST_OP_BODY_FI_F(_x, exp) fp0.x = my_extendsfxf2(TEST_FUNC_FI_F(my_truncxfsf2(_x.x), exp))
#endif
#endif


#ifdef TEST_FUNC_FL_F
#ifdef TEST_LDOUBLE
#define TEST_OP_BODY_FL_F(_x, exp) fp0.x = TEST_FUNC_FL_F(_x.x, exp)
#endif
#ifdef TEST_DOUBLE
#define TEST_OP_BODY_FL_F(_x, exp) fp0.x = my_extenddfxf2(TEST_FUNC_FL_F(my_truncxfdf2(_x.x), exp))
#endif
#ifdef TEST_FLOAT
#define TEST_OP_BODY_FL_F(_x, exp) fp0.x = my_extendsfxf2(TEST_FUNC_FL_F(my_truncxfsf2(_x.x), exp))
#endif
#endif


#ifdef TEST_FUNC_F_FF
/*
 * body for sincos which returns two values
 */
#ifdef TEST_LDOUBLE
#define TEST_OP_BODY_F_FF(_x, _r1, _r2) fp0.x = _x.x, TEST_FUNC_F_FF(fp0.x, &(_r1.x), &(_r2.x))
#endif
#ifdef TEST_DOUBLE
#define TEST_OP_BODY_F_FF(_x, _r1, _r2) fp0.d = my_truncxfdf2(_x.x), TEST_FUNC_F_FF(fp0.d, &(_r1.d), &(_r2.d)), _r1.x = my_extenddfxf2(_r1.d), _r2.x = my_extenddfxf2(_r2.d)
#endif
#ifdef TEST_FLOAT
#define TEST_OP_BODY_F_FF(_x, _r1, _r2) fp0.s = my_truncxfsf2(_x.x), TEST_FUNC_F_FF(fp0.s, &(_r1.s), &(_r2.s)), _r1.x = my_extendsfxf2(_r1.d), _r2.x = my_extendsfxf2(_r2.d)
#endif

#endif


#ifdef TEST_FUNC_IF_F
#ifdef TEST_LDOUBLE
#define TEST_OP_BODY_IF_F(_i, _x) fp0.x = TEST_FUNC_IF_F(_i, _x.x)
#endif
#ifdef TEST_DOUBLE
#define TEST_OP_BODY_IF_F(_i, _x) fp0.x = my_extenddfxf2(TEST_FUNC_IF_F(_i, my_truncxfdf2(_x.x)))
#endif
#ifdef TEST_FLOAT
#define TEST_OP_BODY_IF_F(_i, _x) fp0.x = my_extendsfxf2(TEST_FUNC_IF_F(_i, my_truncxfsf2(_x.x)))
#endif
#endif


#if !defined(NO_PRINTF_DOUBLE)
#if defined(TEST_FUNC_F_F)

static int test_table_f_f(const test_f_f_data *data, size_t n, const char *file)
{
	size_t l;
	int i;
	int status = 0, this_fail;
	ld_union fp0;
	
	for (l = 0; l < n; l++)
	{
		numtests++;
		if (!SKIP_TEST(data[l].x, data[l].flags))
		{
			if (testonly == 0 || testonly == numtests)
			{
				for (i = 0; i < jit_loops; i++)
				{
					TEST_OP_BODY_F_F(data[l].x);
					this_fail = check_fp(data[l].e.v.exponent, data[l].e.v.mantissa0, data[l].e.v.mantissa1, &fp0, data[l].flags, i, file, data[l].line);
					if (this_fail)
					{
						status |= this_fail != 0;
						break;
					}
				}
			}
		}
	}
	
	return status;
}
#endif


#if defined(TEST_FUNC_IF_F)

static int test_table_if_f(const test_if_f_data *data, size_t n, const char *file)
{
	size_t l;
	int i;
	int status = 0, this_fail;
	ld_union fp0;
	
	for (l = 0; l < n; l++)
	{
		numtests++;
		if (!SKIP_TEST(data[l].x, data[l].flags))
		{
			if (testonly == 0 || testonly == numtests)
			{
				for (i = 0; i < jit_loops; i++)
				{
					TEST_OP_BODY_IF_F(data[l].i, data[l].x);
					this_fail = check_fp(data[l].e.v.exponent, data[l].e.v.mantissa0, data[l].e.v.mantissa1, &fp0, data[l].flags, i, file, data[l].line);
					if (this_fail)
					{
						status |= this_fail != 0;
						break;
					}
				}
			}
		}
	}
	
	return status;
}
#endif


#if defined(TEST_FUNC_F_I)

static int test_table_f_i(const test_f_i_data *data, size_t n, const char *file)
{
	size_t l;
	int i;
	int status = 0, this_fail;
	int x;

	for (l = 0; l < n; l++)
	{
		numtests++;
		if (!SKIP_TEST(data[l].x, data[l].flags))
		{
			if (testonly == 0 || testonly == numtests)
			{
				for (i = 0; i < jit_loops; i++)
				{
					TEST_OP_BODY_F_I(data[l].x);
#ifdef INT_IS_BOOL
					x = x != 0;
#endif
					this_fail = check_int(data[l].e, x, data[l].flags, i, file, data[l].line);
					if (this_fail)
					{
						status |= this_fail != 0;
						break;
					}
				}
			}
		}
	}

	return status;
}

#endif


#if defined(TEST_FUNC_FF_I)

static int test_table_ff_i(const test_ff_i_data *data, size_t n, const char *file)
{
	size_t l;
	int i;
	int status = 0, this_fail;
	int x;

	for (l = 0; l < n; l++)
	{
		numtests++;
		if (!SKIP_TEST(data[l].x, data[l].flags) &&
			!SKIP_TEST(data[l].y, data[l].flags))
		{
			if (testonly == 0 || testonly == numtests)
			{
				for (i = 0; i < jit_loops; i++)
				{
					TEST_OP_BODY_FF_I(data[l].x, data[l].y);
#ifdef INT_IS_BOOL
					x = x != 0;
#endif
					this_fail = check_int(data[l].e, x, data[l].flags, i, file, data[l].line);
					if (this_fail)
					{
						status |= this_fail != 0;
						break;
					}
				}
			}
		}
	}

	return status;
}

#endif


#if defined(TEST_FUNC_F_L)

static int test_table_f_l(const test_f_l_data *data, size_t n, const char *file)
{
	size_t l;
	int i;
	int status = 0, this_fail;
	long x;

	for (l = 0; l < n; l++)
	{
		numtests++;
		if (!SKIP_TEST(data[l].x, data[l].flags))
		{
			if (testonly == 0 || testonly == numtests)
			{
				for (i = 0; i < jit_loops; i++)
				{
					TEST_OP_BODY_F_L(data[l].x);
					this_fail = check_long(data[l].e, x, data[l].flags, i, file, data[l].line);
					if (this_fail)
					{
						status |= this_fail != 0;
						break;
					}
				}
			}
		}
	}

	return status;
}

#endif


#if defined(TEST_FUNC_F_LL)

static int test_table_f_ll(const test_f_ll_data *data, size_t n, const char *file)
{
	size_t l;
	int i;
	int status = 0, this_fail;
	long long x;

	for (l = 0; l < n; l++)
	{
		numtests++;
		if (!SKIP_TEST(data[l].x, data[l].flags))
		{
			if (testonly == 0 || testonly == numtests)
			{
				for (i = 0; i < jit_loops; i++)
				{
					TEST_OP_BODY_F_LL(data[l].x);
					this_fail = check_longlong(data[l].e, x, data[l].flags, i, file, data[l].line);
					if (this_fail)
					{
						status |= this_fail != 0;
						break;
					}
				}
			}
		}
	}

	return status;
}

#endif


#if defined(TEST_FUNC_FF_F)

static int test_table_ff_f(const test_ff_f_data *data, size_t n, const char *file)
{
	size_t l;
	int i;
	int status = 0, this_fail;
	ld_union fp0;
	
	for (l = 0; l < n; l++)
	{
		numtests++;
		if (!SKIP_TEST(data[l].x, data[l].flags) && !SKIP_TEST(data[l].y, data[l].flags))
		{
			if (testonly == 0 || testonly == numtests)
			{
				for (i = 0; i < jit_loops; i++)
				{
					TEST_OP_BODY_FF_F(data[l].x, data[l].y);
					this_fail = check_fp(data[l].e.v.exponent, data[l].e.v.mantissa0, data[l].e.v.mantissa1, &fp0, data[l].flags, i, file, data[l].line);
					if (this_fail)
					{
						status |= this_fail != 0;
						break;
					}
				}
			}
		}
	}
	
	return status;
}

#endif
	

#if defined(TEST_FUNC_FFF_F)

static int test_table_fff_f(const test_fff_f_data *data, size_t n, const char *file)
{
	size_t l;
	int i;
	int status = 0, this_fail;
	ld_union fp0;
	
	for (l = 0; l < n; l++)
	{
		numtests++;
		if (!SKIP_TEST(data[l].a, data[l].flags) &&
			!SKIP_TEST(data[l].b, data[l].flags) &&
			!SKIP_TEST(data[l].c, data[l].flags))
		{
			if (testonly == 0 || testonly == numtests)
			{
				for (i = 0; i < jit_loops; i++)
				{
					TEST_OP_BODY_FFF_F(data[l].a, data[l].b, data[l].c);
					this_fail = check_fp(data[l].e.v.exponent, data[l].e.v.mantissa0, data[l].e.v.mantissa1, &fp0, data[l].flags, i, file, data[l].line);
					if (this_fail)
					{
						status |= this_fail != 0;
						break;
					}
				}
			}
		}
	}
	
	return status;
}

#endif
	

#if defined(TEST_FUNC_F_FFP)

static int test_table_f_ffp(const test_f_ffp_data *data, size_t n, const char *file)
{
	size_t l;
	int i;
	int status = 0, this_fail;
	ld_union r, r2;
	
	for (l = 0; l < n; l++)
	{
		numtests++;
		if (!SKIP_TEST(data[l].x, data[l].flags))
		{
			if (testonly == 0 || testonly == numtests)
			{
				for (i = 0; i < jit_loops; i++)
				{
					TEST_OP_BODY_F_FFP(data[l].x);
					this_fail = check_fp(data[l].e.v.exponent, data[l].e.v.mantissa0, data[l].e.v.mantissa1, &r, data[l].flags, i, file, data[l].line);
					this_fail |= check_fp(data[l].e2.v.exponent, data[l].e2.v.mantissa0, data[l].e2.v.mantissa1, &r2, data[l].flags, i, file, data[l].line);
					if (this_fail)
					{
						status |= this_fail != 0;
						break;
					}
				}
			}
		}
	}
	
	return status;
}

#endif
	

#if defined(TEST_FUNC_F_FF)

static int test_table_f_ff(const test_f_ff_data *data, size_t n, const char *file)
{
	size_t l;
	int i;
	int status = 0, this_fail;
	ld_union fp0;
	ld_union fp1;
	ld_union fp2;
	
	for (l = 0; l < n; l++)
	{
		numtests++;
		if (!SKIP_TEST(data[l].x, data[l].flags))
		{
			if (testonly == 0 || testonly == numtests)
			{
				for (i = 0; i < jit_loops; i++)
				{
					TEST_OP_BODY_F_FF(data[l].x, fp1, fp2);
					this_fail = check_fp(data[l].r1.v.exponent, data[l].r1.v.mantissa0, data[l].r1.v.mantissa1, &fp1, data[l].flags, i, file, data[l].line);
					this_fail |= check_fp(data[l].r2.v.exponent, data[l].r2.v.mantissa0, data[l].r2.v.mantissa1, &fp2, data[l].flags, i, file, data[l].line);
					if (this_fail)
					{
						status |= this_fail != 0;
						break;
					}
				}
			}
		}
	}
	
	return status;
}

#endif

#if defined(TEST_FUNC_FIP_F)

static int test_table_fip_f(const test_fip_f_data *data, size_t n, const char *file)
{
	size_t l;
	int i;
	int status = 0, this_fail;
	ld_union fp0;
	int exp;
	
	for (l = 0; l < n; l++)
	{
		numtests++;
		if (!SKIP_TEST(data[l].x, data[l].flags))
		{
			if (testonly == 0 || testonly == numtests)
			{
				for (i = 0; i < jit_loops; i++)
				{
					exp = 0;
					TEST_OP_BODY_FIP_F(data[l].x);
					if ((data[l].x.v.exponent & IEEE854_LONG_DOUBLE_MAXEXP) == IEEE854_LONG_DOUBLE_MAXEXP)
						exp = 0;
					this_fail = check_fp(data[l].e.v.exponent, data[l].e.v.mantissa0, data[l].e.v.mantissa1, &fp0, data[l].flags, i, file, data[l].line);
					this_fail |= check_int(data[l].exp, exp, data[l].flags, i, file, data[l].line);
					if (this_fail)
					{
						status |= this_fail != 0;
						break;
					}
				}
			}
		}
	}
	
	return status;
}
#endif


#if defined(TEST_FUNC_FI_F)

static int test_table_fi_f(const test_fi_f_data *data, size_t n, const char *file)
{
	size_t l;
	int i;
	int status = 0, this_fail;
	ld_union fp0;
	
	for (l = 0; l < n; l++)
	{
		numtests++;
		if (!SKIP_TEST(data[l].x, data[l].flags))
		{
			if (testonly == 0 || testonly == numtests)
			{
				for (i = 0; i < jit_loops; i++)
				{
					TEST_OP_BODY_FI_F(data[l].x, data[l].exp);
					this_fail = check_fp(data[l].e.v.exponent, data[l].e.v.mantissa0, data[l].e.v.mantissa1, &fp0, data[l].flags, i, file, data[l].line);
					if (this_fail)
					{
						status |= this_fail != 0;
						break;
					}
				}
			}
		}
	}
	
	return status;
}
#endif


#if defined(TEST_FUNC_FL_F)

static int test_table_fl_f(const test_fl_f_data *data, size_t n, const char *file)
{
	size_t l;
	int i;
	int status = 0, this_fail;
	ld_union fp0;
	
	for (l = 0; l < n; l++)
	{
		numtests++;
		if (!SKIP_TEST(data[l].x, data[l].flags))
		{
			if (testonly == 0 || testonly == numtests)
			{
				for (i = 0; i < jit_loops; i++)
				{
					TEST_OP_BODY_FL_F(data[l].x, data[l].exp);
					this_fail = check_fp(data[l].e.v.exponent, data[l].e.v.mantissa0, data[l].e.v.mantissa1, &fp0, data[l].flags, i, file, data[l].line);
					if (this_fail)
					{
						status |= this_fail != 0;
						break;
					}
				}
			}
		}
	}
	
	return status;
}
#endif


#endif


/*
 * enable:
 *   -1: query only
 *   0/1: disable/enable
 * returns:
 *   previous state as 0/1, or -1 if JIT is not available
 */
static __inline int jit_state(int enable)
{
	register long state __asm__("d0");
	
	__asm__ __volatile__(
		"\t.dc.w 0x713b\n" /* M68K_EMUL_OP_JIT */
		: "=d"(state)
		: "0"(enable)
		: "cc", "memory");
	return state;
}


/* Exception Enable Byte */
#define		FPCR_EXCEPTION_ENABLE	0x0000ff00
#define		FPCR_EXCEPTION_BSUN		0x00008000
#define		FPCR_EXCEPTION_SNAN		0x00004000
#define		FPCR_EXCEPTION_OPERR	0x00002000
#define		FPCR_EXCEPTION_OVFL		0x00001000
#define		FPCR_EXCEPTION_UNFL		0x00000800
#define		FPCR_EXCEPTION_DZ		0x00000400
#define		FPCR_EXCEPTION_INEX2	0x00000200
#define		FPCR_EXCEPTION_INEX1	0x00000100

/* Rounding precision */
#define		FPCR_ROUNDING_PRECISION	0x000000c0
#define		FPCR_PRECISION_SINGLE	0x00000040
#define		FPCR_PRECISION_DOUBLE	0x00000080
#define		FPCR_PRECISION_EXTENDED	0x00000000

/* Rounding mode */
#define		FPCR_ROUNDING_MODE		0x00000030
#define		FPCR_ROUND_NEAR			0x00000000
#define		FPCR_ROUND_ZERO			0x00000010
#define		FPCR_ROUND_MINF			0x00000020
#define		FPCR_ROUND_PINF			0x00000030
	
/* Floating-Point Condition Code Byte */
#define		FPSR_CCB				0x0f000000
#define		FPSR_CCB_NEGATIVE		0x08000000
#define		FPSR_CCB_ZERO			0x04000000
#define		FPSR_CCB_INFINITY		0x02000000
#define		FPSR_CCB_NAN			0x01000000

/* Quotient Byte */
#define		FPSR_QUOTIENT			0x00ff0000
#define		FPSR_QUOTIENT_SIGN		0x00800000
#define		FPSR_QUOTIENT_VALUE		0x007f0000

/* Exception Status Byte */
#define		FPSR_EXCEPTION			FPCR_EXCEPTION_ENABLE
#define		FPSR_EXCEPTION_BSUN		FPCR_EXCEPTION_BSUN
#define		FPSR_EXCEPTION_SNAN		FPCR_EXCEPTION_SNAN
#define		FPSR_EXCEPTION_OPERR	FPCR_EXCEPTION_OPERR
#define		FPSR_EXCEPTION_OVFL		FPCR_EXCEPTION_OVFL
#define		FPSR_EXCEPTION_UNFL		FPCR_EXCEPTION_UNFL
#define		FPSR_EXCEPTION_DZ		FPCR_EXCEPTION_DZ
#define		FPSR_EXCEPTION_INEX2	FPCR_EXCEPTION_INEX2
#define		FPSR_EXCEPTION_INEX1	FPCR_EXCEPTION_INEX1

/* Accrued Exception Byte */
#define		FPSR_ACCRUED_EXCEPTION	0x000000f8
#define		FPSR_ACCR_IOP			0x00000080
#define		FPSR_ACCR_OVFL			0x00000040
#define		FPSR_ACCR_UNFL			0x00000020
#define		FPSR_ACCR_DZ			0x00000010
#define		FPSR_ACCR_INEX			0x00000008


#ifdef __mc68000__

static uint32_t get_fpcr(void)
{
#ifdef __HAVE_68881__
	uint32_t fpcr;
	
	__asm__ __volatile__(
		"\tfmove.l %%fpcr,%[fpcr]\n"
		: [fpcr]"=d"(fpcr)
		:
		: "cc", "memory");
	return fpcr;
#else
	return FPCR_PRECISION_DOUBLE|FPCR_ROUND_NEAR;
#endif
}

static void set_fpcr(uint32_t fpcr)
{
#ifdef __HAVE_68881__
	__asm__ __volatile__(
		"\tfmove.l %[fpcr],%%fpcr\n"
		:
		: [fpcr]"d"(fpcr)
		: "cc", "memory");
#else
	(void) fpcr;
#endif
}

#else

#include <fpu_control.h>
#include <fenv.h>
#include <stdlib.h>

static void set_fpcr(uint32_t fpcr)
{
	fpu_control_t cw;
	
	_FPU_GETCW(cw);
	cw &= ~(_FPU_RC_NEAREST|_FPU_RC_DOWN|_FPU_RC_UP|_FPU_RC_ZERO);
	cw &= ~(_FPU_EXTENDED|_FPU_DOUBLE|_FPU_SINGLE);
	switch (fpcr & FPCR_ROUNDING_MODE)
	{
	case FPCR_ROUND_NEAR:
		fesetround(1);
		cw |= _FPU_RC_NEAREST;
		break;
	case FPCR_ROUND_ZERO:
		fesetround(0);
		cw |= _FPU_RC_ZERO;
		break;
	case FPCR_ROUND_MINF:
		fesetround(3);
		cw |= _FPU_RC_DOWN;
		break;
	case FPCR_ROUND_PINF:
		fesetround(2);
		cw |= _FPU_RC_UP;
		break;
	default:
		abort();
	}
	switch (fpcr & FPCR_ROUNDING_PRECISION)
	{
		case FPCR_PRECISION_SINGLE: cw |= _FPU_SINGLE; break;
		case FPCR_PRECISION_DOUBLE: cw |= _FPU_DOUBLE; break;
		case FPCR_PRECISION_EXTENDED: cw |= _FPU_EXTENDED; break;
		default: abort();
	}
	_FPU_SETCW(cw);
}

static uint32_t get_fpcr(void)
{
	uint32_t fpcr = 0;
	fpu_control_t cw;
	
	_FPU_GETCW(cw);
	switch (cw & (_FPU_RC_NEAREST|_FPU_RC_DOWN|_FPU_RC_UP|_FPU_RC_ZERO))
	{
		case _FPU_RC_NEAREST: fpcr |= FPCR_ROUND_NEAR; break;
		case _FPU_RC_ZERO: fpcr |= FPCR_ROUND_ZERO; break;
		case _FPU_RC_DOWN: fpcr |= FPCR_ROUND_MINF; break;
		case _FPU_RC_UP: fpcr |= FPCR_ROUND_PINF; break;
	}
	switch (cw & (_FPU_EXTENDED|_FPU_DOUBLE|_FPU_SINGLE))
	{
		case _FPU_SINGLE: fpcr |= FPCR_PRECISION_SINGLE; break;
		case _FPU_DOUBLE: fpcr |= FPCR_PRECISION_DOUBLE; break;
		case _FPU_EXTENDED: fpcr |= FPCR_PRECISION_EXTENDED; break;
	}
	return fpcr;
}

#endif

static void set_precision(uint32_t prec)
{
	uint32_t fpcr;
	
	fpcr = get_fpcr();
	fpcr &= ~FPCR_ROUNDING_PRECISION;
	fpcr |= prec;
	set_fpcr(fpcr);
}


static void set_rounding(uint32_t rounding)
{
	uint32_t fpcr;
	
	fpcr = get_fpcr();
	fpcr &= ~FPCR_ROUNDING_MODE;
	fpcr |= rounding;
	set_fpcr(fpcr);
}

#if defined(__GNUC__) && defined(__mc68000__) && !defined(__mcoldfire__)

#define NATFEAT_ID   0x7300
#define NATFEAT_CALL 0x7301

#pragma GCC optimize "-fomit-frame-pointer"

#define ASM_NATFEAT3(opcode) "\t.word " #opcode "\n"
#define ASM_NATFEAT2(opcode) ASM_NATFEAT3(opcode)
#define ASM_NATFEAT(n) ASM_NATFEAT2(n)

static unsigned short const nf_id_opcodes[] = { NATFEAT_ID, 0x4e75 };
static unsigned short const nf_call_opcodes[] = { NATFEAT_CALL, 0x4e75 };

#define _nf_get_id(feature_name) ((long (__CDECL *)(const char *))nf_id_opcodes)(feature_name)
#define _nf_call(id, ...) ((long (__CDECL *)(long, ...))nf_call_opcodes)(id, __VA_ARGS__)



/*
 * on ColdFire, the NATFEAT_ID opcode is actually
 * "mvs.b d0,d1",
 * which means the following code will NOT detect
 * the presence of an emulator (should there ever
 * be an emulator capable of emulating a ColdFire processor).
 * Luckily, executing the code on a CF processor is still
 * harmless since all it does is clobber D1.
 */
static long _nf_detect_tos(void)
{
	register long ret __asm__ ("d0");
	register const char *nf_version __asm__("a1") = "NF_VERSION";
	
	__asm__ volatile(
	"\tmove.l	%1,-(%%sp)\n"
	"\tmoveq	#0,%%d0\n"			/* assume no NatFeats available */
	"\tmove.l	%%d0,-(%%sp)\n"
	"\tlea		(1f:w,%%pc),%%a1\n"
	"\tmove.l	(0x0010).w,%%a0\n"	/* illegal instruction vector */
	"\tmove.l	%%a1,(0x0010).w\n"
	"\tmove.l	%%sp,%%a1\n"		/* save the ssp */

	"\tnop\n"						/* flush pipelines (for 68040+) */

	ASM_NATFEAT(NATFEAT_ID)			/* Jump to NATFEAT_ID */
	"\ttst.l	%%d0\n"
	"\tbeq.s	1f\n"
	"\tmoveq	#1,%%d0\n"			/* NatFeats detected */
	"\tmove.l	%%d0,(%%sp)\n"

"1:\n"
	"\tmove.l	%%a1,%%sp\n"
	"\tmove.l	%%a0,(0x0010).w\n"
	"\tmove.l	(%%sp)+,%%d0\n"
	"\taddq.l	#4,%%sp\n"			/* pop nf_version argument */

	"\tnop\n"						/* flush pipelines (for 68040+) */
	: "=g"(ret)  /* outputs */
	: "g"(nf_version)		/* inputs  */
	: __CLOBBER_RETURN("d0") "a0", "d1", "cc" AND_MEMORY
	);
	return ret;
}

static long nf_name_id;

#include <osbind.h>

static __attribute__((noinline)) void nf_init(void)
{
	long ret;
	char buf[80];
	
	ret = Supexec(_nf_detect_tos);
	if (ret == 1)
	{
		nf_name_id = _nf_get_id("NF_NAME");
		if (nf_name_id)
		{
			ret = _nf_call(nf_name_id | 0, (uint32_t)buf, (uint32_t)sizeof(buf));
			if (ret != 0)
			{
				fprintf(stderr, "emulator: %s\n\n", buf);
				if (strcmp(buf, "ARAnyM") == 0)
					emulator = EMULATOR_ARANYM;
				else if (strcmp(buf, "Hatari") == 0)
					emulator = EMULATOR_HATARI;
				else if (strcmp(buf, "STonX") == 0)
					emulator = EMULATOR_STONX;
			}
		}
	}
}

#else

static void nf_init(void)
{
}

#endif


static void test_init(int argc, char **argv)
{
	if (argc > 1)
		testonly = (int)strtol(argv[1], NULL, 0);
	
	set_precision(FPCR_PRECISION_EXTENDED);
	set_rounding(FPCR_ROUND_NEAR);

	nf_init();

	/*
	 * only ARAnyM has JIT so far;
	 * no need to run more than 1 loop for other emulators
	 */
	if (emulator != EMULATOR_ARANYM)
		jit_loops = 1;
}
 

/*
 * some constants
 */
#define ZERO_P HEXCONSTE(+0.0, 0x0000, 0x00000000L, 0x00000000L)
#define ZERO_M HEXCONSTE(-0.0, 0x8000, 0x00000000L, 0x00000000L)
#if defined(__i386__) || defined(__x86_64__)
/* 387 needs the explicit integer bit for INFs, otherwise it is interpreted as NaN */
#define INF_P HEXCONSTE("+inf", 0x7fff, 0x80000000L, 0x00000000L)
#define INF_M HEXCONSTE("-inf", 0xffff, 0x80000000L, 0x00000000L)
#else
#define INF_P HEXCONSTE("+inf", 0x7fff, 0x00000000L, 0x00000000L)
#define INF_M HEXCONSTE("-inf", 0xffff, 0x00000000L, 0x00000000L)
#endif
#define QNAN_P HEXCONSTE("nan", 0x7fff, 0xffffffffL, 0xffffffffL)
#define QNAN_M HEXCONSTE("-nan", 0xffff, 0xffffffffL, 0xffffffffL)
#define SNAN_P HEXCONSTE("snan", 0x7fff, 0xbfffffffL, 0xffffffffL)
#define SNAN_M HEXCONSTE("-snan", 0xffff, 0xbfffffffL, 0xffffffffL)

#ifndef __LDBL_MIN_EXP__
#ifdef __mc68000__
#define __LDBL_MIN_EXP__ (-16382)
#else
#define __LDBL_MIN_EXP__ (-16381)
#endif
#endif

#if __LDBL_MIN_EXP__ <= (-16382)
#define SUBNORM_P HEXCONSTE(+1.82259976594123730126e-4951 /* LDBL_DENORM_MIN */, 0x0000, 0x00000000L, 0x00000001L)
#define SUBNORM_M HEXCONSTE(-1.82259976594123730126e-4951 /* -LDBL_DENORM_MIN */, 0x8000, 0x00000000L, 0x00000001L)
#define MIN_P HEXCONSTE(+1.68105157155604675313e-4932 /* LDBL_MIN */, 0x0000, 0x80000000L, 0x00000000L)
#define MIN_M HEXCONSTE(-1.68105157155604675313e-4932 /* -LDBL_MIN */, 0x8000, 0x80000000L, 0x00000000L)
#else
#define SUBNORM_P HEXCONSTE(+3.64519953188247460253e-4951 /* LDBL_DENORM_MIN */, 0x0000, 0x00000000L, 0x00000001L)
#define SUBNORM_M HEXCONSTE(-3.64519953188247460253e-4951 /* -LDBL_DENORM_MIN */, 0x8000, 0x00000000L, 0x00000001L)
#define MIN_P HEXCONSTE(+3.36210314311209350626e-4932 /* LDBL_MIN */, 0x0001, 0x80000000L, 0x00000000L)
#define MIN_M HEXCONSTE(-3.36210314311209350626e-4932 /* -LDBL_MIN */, 0x8001, 0x80000000L, 0x00000000L)
#endif
#define MAX_P HEXCONSTE(+1.18973149535723176502e+4932 /* LDBL_MAX */, 0x7ffe, 0xffffffffL, 0xffffffffL)
#define MAX_M HEXCONSTE(-1.18973149535723176502e+4932 /* -LDBL_MAX */, 0xfffe, 0xffffffffL, 0xffffffffL)

static ld_union const zero_p = ZERO_P;
static ld_union const zero_m = ZERO_M;
static ld_union const inf_p = INF_P;
static ld_union const inf_m = INF_M;
static ld_union const qnan_p = QNAN_P;
static ld_union const qnan_m = QNAN_M;
static ld_union const snan_p = SNAN_P;
static ld_union const snan_m = SNAN_M;
static ld_union const subnorm_p = SUBNORM_P;
static ld_union const subnorm_m = SUBNORM_M;
static ld_union const min_p = MIN_P;
static ld_union const min_m = MIN_M;
static ld_union const max_p = MAX_P;
static ld_union const max_m = MAX_M;

#define HEXCONST_E        HEXCONSTE("2.718281828459045235360287471352662498", 0x4000, 0xadf85458L, 0xa2bb4a9aL) /* e */
#define HEXCONST_LOG2E    HEXCONSTE("1.442695040888963407359924681001892137", 0x3fff, 0xb8aa3b29L, 0x5c17f0bcL) /* log2(e) */
#define HEXCONST_LOG10E   HEXCONSTE("0.434294481903251827651128918916605082", 0x3ffd, 0xde5bd8a9L, 0x37287195L) /* log10(e) */
#define HEXCONST_LN2      HEXCONSTE("0.693147180559945309417232121458176568", 0x3ffe, 0xb17217f7L, 0xd1cf79acL) /* ln(2) */
#define HEXCONST_LN10     HEXCONSTE("2.302585092994045684017991454684364208", 0x4000, 0x935d8dddL, 0xaaa8ac17L) /* ln(10) */
#define HEXCONST_PI       HEXCONSTE("3.141592653589793238462643383279502884", 0x4000, 0xc90fdaa2L, 0x2168c235L) /* pi */
#define HEXCONST_PI_2     HEXCONSTE("1.570796326794896619231321691639751442", 0x3fff, 0xc90fdaa2L, 0x2168c235L) /* pi/2 */
#define HEXCONST_PI_4     HEXCONSTE("0.785398163397448309615660845819875721", 0x3ffe, 0xc90fdaa2L, 0x2168c235L) /* pi/4 */
#define HEXCONST_1_PI     HEXCONSTE("0.318309886183790671537767526745028724", 0x3ffd, 0xa2f9836eL, 0x4e44152aL) /* 1/pi */
#define HEXCONST_2_PI     HEXCONSTE("0.636619772367581343075535053490057448", 0x3ffe, 0xa2f9836eL, 0x4e44152aL) /* 2/pi */
#define HEXCONST_2_SQRTPI HEXCONSTE("1.128379167095512573896158903121545172", 0x3fff, 0x906eba82L, 0x14db688dL) /* 2/sqrt(pi) */
#define HEXCONST_SQRT2    HEXCONSTE("1.414213562373095048801688724209698079", 0x3fff, 0xb504f333L, 0xf9de6484L) /* sqrt(2) */
#define HEXCONST_SQRT1_2  HEXCONSTE("0.707106781186547524400844362104849039", 0x3ffe, 0xb504f333L, 0xf9de6484L) /* 1/sqrt(2) */

#define HEXCONST_PI_M     HEXCONSTE("3.141592653589793238462643383279502884", 0xc000, 0xc90fdaa2L, 0x2168c235L) /* -pi */
#define HEXCONST_PI_2_M   HEXCONSTE("-1.570796326794896619231321691639751442", 0xbfff, 0xc90fdaa2L, 0x2168c235L) /* -pi/2 */
#define HEXCONST_PI_4_M   HEXCONSTE("-0.785398163397448309615660845819875721", 0xbffe, 0xc90fdaa2L, 0x2168c235L) /* -pi/4 */
