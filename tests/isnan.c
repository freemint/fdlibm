#include "testconfig.h"
#if defined(HAVE_ISNAN) || defined(HAVE___ISNAN)
/* FIXME: that would use the function from mintlib */
#if defined(HAVE___ISNAN) && 0
int __MATH_PRECNAME(__isnan)(REAL x);
#define TEST_FUNC_F_I __MATH_PRECNAME(__isnan)
#else
#define TEST_FUNC_F_I __MATH_PRECNAME(isnan)
#endif
#include "testdriver.h"

/* make sure we don't test the builtin */
#undef isnan
#undef __isnan

static test_f_i_data const isnan_data[] = {
	{ __LINE__, ZERO_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, ZERO_M, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, SUBNORM_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, SUBNORM_M, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_M, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, QNAN_P, 1, NO_INEXACT_EXCEPTION },
	{ __LINE__, QNAN_M, 1, NO_INEXACT_EXCEPTION },
	{ __LINE__, SNAN_P, 1, NO_INEXACT_EXCEPTION },
	{ __LINE__, SNAN_M, 1, NO_INEXACT_EXCEPTION },
	{ __LINE__, MAX_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, MAX_M, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, MIN_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, MIN_M, 0, NO_INEXACT_EXCEPTION },

	{ __LINE__, HEXCONSTE(10, 0x4002, 0xa0000000L, 0x00000000L), 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, HEXCONSTE(-10, 0xc002, 0xa0000000L, 0x00000000L), 0, NO_INEXACT_EXCEPTION },
};

int main(int argc, char **argv)
{
	int status;
	
	status = 0;
	
	test_init(argc, argv);
	
	status |= test_table_f_i(isnan_data, ARRAY_SIZE(isnan_data), __FILE__);
	(void)check_fp;
		
	return status ? EXIT_FAILURE : EXIT_SUCCESS;
}

#else

int main(void)
{
	return 77;
}

#endif
