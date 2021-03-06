#include "testconfig.h"
#ifdef HAVE_FMIN
/* FIXME: not declared for fpu version */
double fmin(double, double);
#define TEST_FUNC_FF_F __MATH_PRECNAME(fmin)
#include "testdriver.h"


static test_ff_f_data const fmin_data[] = {
	{ __LINE__, HEXCONSTE(6.0, 0x4001, 0xc0000000L, 0x00000000L), HEXCONSTE(5.0, 0x4001, 0xa0000000L, 0x00000000L), HEXCONSTE(5.0, 0x4001, 0xa0000000L, 0x00000000L), 0 },
	{ __LINE__, ZERO_P, ZERO_P, ZERO_P, 0 },
	{ __LINE__, HEXCONSTE(0.1, 0x3ffb, 0xccccccccL, 0xcccccccdL), HEXCONSTE(0.2, 0x3ffc, 0xccccccccL, 0xcccccccdL), HEXCONSTE(0.1, 0x3ffb, 0xccccccccL, 0xcccccccdL), 0 },
	{ __LINE__, HEXCONSTE(-0.1, 0xbffb, 0xccccccccL, 0xcccccccdL), HEXCONSTE(-0.2, 0xbffc, 0xccccccccL, 0xcccccccdL), HEXCONSTE(-0.2, 0xbffc, 0xccccccccL, 0xcccccccdL), 0 },
	{ __LINE__, HEXCONSTE(-0.1, 0xbffb, 0xccccccccL, 0xcccccccdL), HEXCONSTE(0.2, 0x3ffc, 0xccccccccL, 0xcccccccdL), HEXCONSTE(-0.1, 0xbffb, 0xccccccccL, 0xcccccccdL), 0 },
	{ __LINE__, HEXCONSTE(0.1, 0x3ffb, 0xccccccccL, 0xcccccccdL), HEXCONSTE(0.2, 0x3ffc, 0xccccccccL, 0xcccccccdL), HEXCONSTE(0.1, 0x3ffb, 0xccccccccL, 0xcccccccdL), 0 },
	{ __LINE__, HEXCONSTE(0.1, 0x3ffb, 0xccccccccL, 0xcccccccdL), HEXCONSTE(-0.2, 0xbffc, 0xccccccccL, 0xcccccccdL), HEXCONSTE(-0.2, 0xbffc, 0xccccccccL, 0xcccccccdL), 0 },
	{ __LINE__, HEXCONSTE(1e10, 0x4020, 0x9502f900L, 0x00000000L), HEXCONSTE(1.1e10, 0x4020, 0xa3e9ab80L, 0x00000000L), HEXCONSTE(1e10, 0x4020, 0x9502f900L, 0x00000000L), 0 },
	{ __LINE__, HEXCONSTE(-1e10, 0xc020, 0x9502f900L, 0x00000000L), HEXCONSTE(-1.1e10, 0xc020, 0xa3e9ab80L, 0x00000000L), HEXCONSTE(-1.1e10, 0xc020, 0xa3e9ab80L, 0x00000000L), 0 },
	{ __LINE__, HEXCONSTE(1e-10, 0x3fdd, 0xdbe6feceL, 0xbdedd5bfL), HEXCONSTE(1.1e-10, 0x3fdd, 0xf1e47eb0L, 0x37526b1fL), HEXCONSTE(1e-10, 0x3fdd, 0xdbe6feceL, 0xbdedd5bfL), 0 },

	{ __LINE__, ZERO_P, ZERO_P, ZERO_P, NO_INEXACT_EXCEPTION },
	{ __LINE__, ZERO_M, ZERO_P, ZERO_P, NO_INEXACT_EXCEPTION|FLAG_IGNORE_ZEROSIGN },
	{ __LINE__, ZERO_P, ZERO_M, ZERO_P, NO_INEXACT_EXCEPTION|FLAG_IGNORE_ZEROSIGN },
	{ __LINE__, ZERO_M, ZERO_M, ZERO_M, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM2 },
	{ __LINE__, HEXCONSTE(9, 0x4002, 0x90000000L, 0x00000000L), ZERO_P, ZERO_P, NO_INEXACT_EXCEPTION },
	{ __LINE__, HEXCONSTE(-9, 0xc002, 0x90000000L, 0x00000000L), ZERO_P, HEXCONSTE(-9, 0xc002, 0x90000000L, 0x00000000L), NO_INEXACT_EXCEPTION },
	{ __LINE__, ZERO_P, HEXCONSTE(9, 0x4002, 0x90000000L, 0x00000000L), ZERO_P, NO_INEXACT_EXCEPTION },
	{ __LINE__, ZERO_P, HEXCONSTE(-9, 0xc002, 0x90000000L, 0x00000000L), HEXCONSTE(-9, 0xc002, 0x90000000L, 0x00000000L), NO_INEXACT_EXCEPTION },

	{ __LINE__, INF_P, HEXCONSTE(9, 0x4002, 0x90000000L, 0x00000000L), HEXCONSTE(9, 0x4002, 0x90000000L, 0x00000000L), NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_P, HEXCONSTE(-9, 0xc002, 0x90000000L, 0x00000000L), HEXCONSTE(-9, 0xc002, 0x90000000L, 0x00000000L), NO_INEXACT_EXCEPTION },
	{ __LINE__, HEXCONSTE(9, 0x4002, 0x90000000L, 0x00000000L), INF_P, HEXCONSTE(9, 0x4002, 0x90000000L, 0x00000000L), NO_INEXACT_EXCEPTION },
	{ __LINE__, HEXCONSTE(-9, 0xc002, 0x90000000L, 0x00000000L), INF_P, HEXCONSTE(-9, 0xc002, 0x90000000L, 0x00000000L), NO_INEXACT_EXCEPTION },
	{ __LINE__, ZERO_P, INF_P, ZERO_P, NO_INEXACT_EXCEPTION },
	{ __LINE__, ZERO_M, INF_P, ZERO_M, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM2 },
	{ __LINE__, INF_P, ZERO_P, ZERO_P, NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_P, ZERO_M, ZERO_M, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM2 },

	{ __LINE__, INF_M, HEXCONSTE(9, 0x4002, 0x90000000L, 0x00000000L), INF_M, NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_M, HEXCONSTE(-9, 0xc002, 0x90000000L, 0x00000000L), INF_M, NO_INEXACT_EXCEPTION },
	{ __LINE__, HEXCONSTE(9, 0x4002, 0x90000000L, 0x00000000L), INF_M, INF_M, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM }, /* FIXME: fcmp/fjun problem? */
	{ __LINE__, HEXCONSTE(-9, 0xc002, 0x90000000L, 0x00000000L), INF_M, INF_M, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM }, /* FIXME: fcmp/fjun problem? */
	{ __LINE__, ZERO_P, INF_M, INF_M, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM }, /* FIXME: fcmp/fjun problem? */
	{ __LINE__, ZERO_M, INF_M, INF_M, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM }, /* FIXME: fcmp/fjun problem? */
	{ __LINE__, INF_M, ZERO_P, INF_M, NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_M, ZERO_M, INF_M, NO_INEXACT_EXCEPTION },

	/* If one argument is a NaN, the other argument is returned. */
	{ __LINE__, ZERO_P, QNAN_P, ZERO_P, NO_INEXACT_EXCEPTION },
	{ __LINE__, ZERO_M, QNAN_P, ZERO_M, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM2 },
	{ __LINE__, HEXCONSTE(9, 0x4002, 0x90000000L, 0x00000000L), QNAN_P, HEXCONSTE(9, 0x4002, 0x90000000L, 0x00000000L), NO_INEXACT_EXCEPTION },
	{ __LINE__, HEXCONSTE(-9, 0xc002, 0x90000000L, 0x00000000L), QNAN_P, HEXCONSTE(-9, 0xc002, 0x90000000L, 0x00000000L), NO_INEXACT_EXCEPTION },
	{ __LINE__, QNAN_P, ZERO_P, ZERO_P, NO_INEXACT_EXCEPTION },
	{ __LINE__, QNAN_P, ZERO_M, ZERO_M, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM2 },
	{ __LINE__, QNAN_P, HEXCONSTE(9, 0x4002, 0x90000000L, 0x00000000L), HEXCONSTE(9, 0x4002, 0x90000000L, 0x00000000L), NO_INEXACT_EXCEPTION },
	{ __LINE__, QNAN_P, HEXCONSTE(-9, 0xc002, 0x90000000L, 0x00000000L), HEXCONSTE(-9, 0xc002, 0x90000000L, 0x00000000L), NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_P, QNAN_P, INF_P, NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_M, QNAN_P, INF_M, NO_INEXACT_EXCEPTION },
	{ __LINE__, QNAN_P, INF_P, INF_P, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM }, /* FIXME: fcmp/fjun problem? */
	{ __LINE__, QNAN_P, INF_M, INF_M, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM }, /* FIXME: fcmp/fjun problem? */

	/* If both arguments are NaN, a NaN is returned. */
	{ __LINE__, QNAN_P, QNAN_P, QNAN_P, NO_INEXACT_EXCEPTION },
};

int main(int argc, char **argv)
{
	int status;
	
	status = 0;
	
	test_init(argc, argv);
	
	status |= test_table_ff_f(fmin_data, ARRAY_SIZE(fmin_data), __FILE__);
	
	return status ? EXIT_FAILURE : EXIT_SUCCESS;
}

#else

int main(void)
{
	return 77;
}

#endif
