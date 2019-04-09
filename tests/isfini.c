#include "testconfig.h"
#if defined(HAVE_ISFINITE) || defined(HAVE___ISFINITE)
#ifdef HAVE___ISFINITE
int __MATH_PRECNAME(__isfinite)(REAL x);
#define TEST_FUNC_F_I __MATH_PRECNAME(__isfinite)
#else
#define TEST_FUNC_F_I __MATH_PRECNAME(isfinite)
#endif
#include "testdriver.h"

/* make sure we don't test the builtin */
#undef isfinite
#undef __isfinite

static test_f_i_data const isfinite_data[] = {
	{ __LINE__, ZERO_P, 1, NO_INEXACT_EXCEPTION },
	{ __LINE__, ZERO_M, 1, NO_INEXACT_EXCEPTION },
	{ __LINE__, SUBNORM_P, 1, NO_INEXACT_EXCEPTION },
	{ __LINE__, SUBNORM_M, 1, NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_M, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, QNAN_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, QNAN_M, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, SNAN_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, SNAN_M, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, MAX_P, 1, NO_INEXACT_EXCEPTION },
	{ __LINE__, MAX_M, 1, NO_INEXACT_EXCEPTION },
	{ __LINE__, MIN_P, 1, NO_INEXACT_EXCEPTION },
	{ __LINE__, MIN_M, 1, NO_INEXACT_EXCEPTION },

	{ __LINE__, HEXCONSTE(10, 0x4002, 0xa0000000L, 0x00000000L), 1, NO_INEXACT_EXCEPTION },
	{ __LINE__, HEXCONSTE(-10, 0xc002, 0xa0000000L, 0x00000000L), 1, NO_INEXACT_EXCEPTION },
};

int main(int argc, char **argv)
{
	int status;
	
	status = 0;
	
	test_init(argc, argv);
	
	status |= test_table_f_i(isfinite_data, ARRAY_SIZE(isfinite_data), __FILE__);
	(void)check_fp;
		
	return status ? EXIT_FAILURE : EXIT_SUCCESS;
}

#else

int main(void)
{
	return 77;
}

#endif
