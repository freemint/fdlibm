#include "testconfig.h"
#if defined(HAVE_ISINF) || defined(HAVE___ISINF)
/* FIXME: that would use the function from mintlib */
#if defined(HAVE___ISINF) && 0
int __MATH_PRECNAME(__isinf)(REAL x);
#define TEST_FUNC_F_I __MATH_PRECNAME(__isinf)
#else
#define TEST_FUNC_F_I __MATH_PRECNAME(isinf)
#endif
#include "testdriver.h"

/* make sure we don't test the builtin */
#undef isinf
#undef __isinf

static test_f_i_data const isinf_data[] = {
	{ __LINE__, ZERO_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, ZERO_M, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, SUBNORM_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, SUBNORM_M, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_P, 1, NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_M, -1, NO_INEXACT_EXCEPTION },
	{ __LINE__, QNAN_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, QNAN_M, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, SNAN_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, SNAN_M, 0, NO_INEXACT_EXCEPTION },
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
	
	status |= test_table_f_i(isinf_data, ARRAY_SIZE(isinf_data), __FILE__);
	(void)check_fp;
		
	return status ? EXIT_FAILURE : EXIT_SUCCESS;
}

#else

int main(void)
{
	return 77;
}

#endif
