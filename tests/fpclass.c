#include "testconfig.h"
#if defined(HAVE_FPCLASSIFY) || defined(HAVE___FPCLASSIFY)
#ifdef HAVE___FPCLASSIFY
int __MATH_PRECNAME(__fpclassify)(REAL x);
#define TEST_FUNC_F_I __MATH_PRECNAME(__fpclassify)
#else
#define TEST_FUNC_F_I __MATH_PRECNAME(fpclassify)
#endif
#include "testdriver.h"

/* make sure we don't test the builtin */
#undef fpclassify
#undef __fpclassify

static test_f_i_data const fpclassify_data[] = {
	{ __LINE__, QNAN_P, FP_NAN, NO_INEXACT_EXCEPTION },
	{ __LINE__, QNAN_M, FP_NAN, NO_INEXACT_EXCEPTION },
	{ __LINE__, SNAN_P, FP_NAN, NO_INEXACT_EXCEPTION },
	{ __LINE__, SNAN_M, FP_NAN, NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_P, FP_INFINITE, NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_M, FP_INFINITE, NO_INEXACT_EXCEPTION },
	{ __LINE__, ZERO_P, FP_ZERO, NO_INEXACT_EXCEPTION },
	{ __LINE__, ZERO_M, FP_ZERO, NO_INEXACT_EXCEPTION },
#ifdef TEST_LDOUBLE /* FIXME: messed up by truncxfdf2 */
	{ __LINE__, SUBNORM_P, FP_SUBNORMAL, NO_INEXACT_EXCEPTION },
	{ __LINE__, SUBNORM_M, FP_SUBNORMAL, NO_INEXACT_EXCEPTION },
#endif
	{ __LINE__, MAX_P, FP_NORMAL, NO_INEXACT_EXCEPTION },
	{ __LINE__, MAX_M, FP_NORMAL, NO_INEXACT_EXCEPTION },
	{ __LINE__, MIN_P, FP_NORMAL, NO_INEXACT_EXCEPTION },
	{ __LINE__, MIN_M, FP_NORMAL, NO_INEXACT_EXCEPTION },

	{ __LINE__, HEXCONSTE(1000, 0x4008, 0xfa000000L, 0x00000000L), FP_NORMAL, NO_INEXACT_EXCEPTION },
};

int main(int argc, char **argv)
{
	int status;
	
	status = 0;
	
	test_init(argc, argv);
	
	status |= test_table_f_i(fpclassify_data, ARRAY_SIZE(fpclassify_data), __FILE__);
	(void)check_fp;
		
	return status ? EXIT_FAILURE : EXIT_SUCCESS;
}

#else

int main(void)
{
	return 77;
}

#endif
