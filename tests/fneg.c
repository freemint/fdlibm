#include "testconfig.h"
#define TEST_FUNC_F_F __MATH_PRECNAME(wrap_neg)
static REAL TEST_FUNC_F_F(REAL x)
{
	return -x;
}
#include "testdriver.h"



static test_f_f_data const fneg_data[] = {
	{ __LINE__, ZERO_P, ZERO_M, FLAG_FAIL_ARANYM2 },
	{ __LINE__, ZERO_M, ZERO_P, 0 },
	{ __LINE__, INF_P, INF_M, 0 },
	{ __LINE__, INF_M, INF_P, 0 },
	{ __LINE__, QNAN_P, QNAN_M, 0 },
	{ __LINE__, QNAN_M, QNAN_P, 0 },
	{ __LINE__, SNAN_P, QNAN_P, 0 },
	{ __LINE__, SNAN_M, QNAN_P, 0 },
	{ __LINE__, SUBNORM_P, SUBNORM_M, FLAG_SUBNORM },
	{ __LINE__, SUBNORM_M, SUBNORM_P, FLAG_SUBNORM },
	{ __LINE__, MIN_M, MIN_P, FLAG_MINVAL },
	{ __LINE__, MIN_P, MIN_M, FLAG_MINVAL },
	{ __LINE__, MAX_M, MAX_P, 0 },
	{ __LINE__, MAX_P, MAX_M, 0 },

	{ __LINE__, HEXCONSTE(38.0, 0x4004, 0x98000000L, 0x00000000L), HEXCONSTE(-38.0, 0xc004, 0x98000000L, 0x00000000L), 0 },
	{ __LINE__, HEXCONSTE(2.718281828459045235360287471352662498, 0x4000, 0xadf85458L, 0xa2bb4a9aL), HEXCONSTE(-2.718281828459045235360287471352662498, 0xc000, 0xadf85458L, 0xa2bb4a9aL), 0 },
	{ __LINE__, HEXCONSTE(-2.718281828459045235360287471352662498, 0xc000, 0xadf85458L, 0xa2bb4a9aL), HEXCONSTE(2.718281828459045235360287471352662498, 0x4000, 0xadf85458L, 0xa2bb4a9aL), 0 },
	{ __LINE__, HEXCONSTE(-55, 0xc004, 0xdc000000L, 0x00000000L), HEXCONSTE(55, 0x4004, 0xdc000000L, 0x00000000L), 0 },
	{ __LINE__, HEXCONSTE(-1024, 0xc009, 0x80000000L, 0x00000000L), HEXCONSTE(1024, 0x4009, 0x80000000L, 0x00000000L), 0 },
};

int main(int argc, char **argv)
{
	int status;
	
	status = 0;
	
	test_init(argc, argv);
	
	status |= test_table_f_f(fneg_data, ARRAY_SIZE(fneg_data), __FILE__);
	
	return status ? EXIT_FAILURE : EXIT_SUCCESS;
}
