#include "testconfig.h"
#ifdef HAVE_COPYSIGN
#define TEST_FUNC_FF_F __MATH_PRECNAME(copysign)
#include "testdriver.h"


static test_ff_f_data const copysign_data[] = {
	{ __LINE__, ZERO_P, HEXCONSTE(4, 0x4001, 0x80000000L, 0x00000000L), ZERO_P, NO_INEXACT_EXCEPTION },
	{ __LINE__, ZERO_P, HEXCONSTE(-4, 0xc001, 0x80000000L, 0x00000000L), ZERO_M, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM2 },
	{ __LINE__, ZERO_M, HEXCONSTE(4, 0x4001, 0x80000000L, 0x00000000L), ZERO_P, NO_INEXACT_EXCEPTION },
	{ __LINE__, ZERO_M, HEXCONSTE(-4, 0xc001, 0x80000000L, 0x00000000L), ZERO_M, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM2 },

	{ __LINE__, INF_P, ZERO_P, INF_P, NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_P, ZERO_M, INF_M, NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_M, ZERO_P, INF_P, NO_INEXACT_EXCEPTION },
	{ __LINE__, INF_M, ZERO_M, INF_M, NO_INEXACT_EXCEPTION },

	{ __LINE__, ZERO_P, INF_P, ZERO_P, NO_INEXACT_EXCEPTION },
	{ __LINE__, ZERO_P, ZERO_M, ZERO_M, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM2 },
	{ __LINE__, ZERO_M, INF_P, ZERO_P, NO_INEXACT_EXCEPTION },
	{ __LINE__, ZERO_M, ZERO_M, ZERO_M, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM2 },

	/* XXX More correctly we would have to check the sign of the NaN.  */
	{ __LINE__, QNAN_P, ZERO_P, QNAN_P, NO_INEXACT_EXCEPTION },
	{ __LINE__, QNAN_P, ZERO_M, QNAN_P, NO_INEXACT_EXCEPTION },
	{ __LINE__, QNAN_M, ZERO_P, QNAN_P, NO_INEXACT_EXCEPTION },
	{ __LINE__, QNAN_M, ZERO_M, QNAN_P, NO_INEXACT_EXCEPTION },
};

int main(int argc, char **argv)
{
	int status;
	
	status = 0;
	
	test_init(argc, argv);
	
	status |= test_table_ff_f(copysign_data, ARRAY_SIZE(copysign_data), __FILE__);
	
	return status ? EXIT_FAILURE : EXIT_SUCCESS;
}

#else

int main(void)
{
	return 77;
}

#endif
