#include "testconfig.h"
#if defined(HAVE_ISNORMAL) || defined(HAVE___ISNORMAL)
#ifdef HAVE___ISNORMAL
int __MATH_PRECNAME(__isnormal)(REAL x);
#define TEST_FUNC_F_I __MATH_PRECNAME(__isnormal)
#else
#define TEST_FUNC_F_I __MATH_PRECNAME(isnormal)
#endif
#include "testdriver.h"

/* make sure we don't test the builtin */
#undef isnormal
#undef __isnormal

static test_f_i_data const isnormal_data[] = {
	{ __LINE__, ZERO_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, ZERO_M, 0, NO_INEXACT_EXCEPTION },
#ifdef TEST_LDOUBLE /* FIXME: messed up by truncxfdf2 */
	{ __LINE__, SUBNORM_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, SUBNORM_M, 0, NO_INEXACT_EXCEPTION },
#endif
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
	
	status |= test_table_f_i(isnormal_data, ARRAY_SIZE(isnormal_data), __FILE__);
	(void)check_fp;
		
	return status ? EXIT_FAILURE : EXIT_SUCCESS;
}

#else

int main(void)
{
	return 77;
}

#endif
