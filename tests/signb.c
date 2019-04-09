#include "testconfig.h"
#if defined(HAVE_SIGNBIT) || defined(HAVE___SIGNBIT)
/* FIXME: that use in the function from mintlib */
#if defined(HAVE___SIGNBIT) && 0
int __MATH_PRECNAME(__signbit)(REAL x);
#define TEST_FUNC_F_I __MATH_PRECNAME(__signbit)
#else
/* FIXME: not declared */
int __MATH_PRECNAME(signbit)(REAL x);
#define TEST_FUNC_F_I __MATH_PRECNAME(signbit)
#endif
#define INT_IS_BOOL
#include "testdriver.h"

/* make sure we don't test the builtin */
#undef signbit
#undef __signbit

static test_f_i_data const signbit_data[] = {
	{ __LINE__, ZERO_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, ZERO_M, 1, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM2 },
	{ __LINE__, SUBNORM_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, SUBNORM_M, 1, NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_M, 1, NO_INEXACT_EXCEPTION },
	{ __LINE__, QNAN_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, QNAN_M, 1, NO_INEXACT_EXCEPTION },
	{ __LINE__, SNAN_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, SNAN_M, 1, NO_INEXACT_EXCEPTION },
	{ __LINE__, MAX_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, MAX_M, 1, NO_INEXACT_EXCEPTION },
	{ __LINE__, MIN_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, MIN_M, 1, NO_INEXACT_EXCEPTION },

	{ __LINE__, HEXCONSTE(10, 0x4002, 0xa0000000L, 0x00000000L), 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, HEXCONSTE(-10, 0xc002, 0xa0000000L, 0x00000000L), 1, NO_INEXACT_EXCEPTION },

	/* signbit (x) != 0 for x < 0.	*/
	{ __LINE__, HEXCONSTE(-1, 0xbfff, 0x80000000L, 0x00000000L), 1, NO_INEXACT_EXCEPTION },
	/* signbit (x) == 0 for x >= 0.  */
	{ __LINE__, HEXCONSTE(1, 0x3fff, 0x80000000L, 0x00000000L), 0, NO_INEXACT_EXCEPTION },
};

int main(int argc, char **argv)
{
	int status;
	
	status = 0;
	
	test_init(argc, argv);
	
	status |= test_table_f_i(signbit_data, ARRAY_SIZE(signbit_data), __FILE__);
	(void)check_fp;
		
	return status ? EXIT_FAILURE : EXIT_SUCCESS;
}

#else

int main(void)
{
	return 77;
}

#endif
