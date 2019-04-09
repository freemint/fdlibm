#include "testconfig.h"
#ifdef HAVE_SCALBLN
#define TEST_FUNC_FL_F __MATH_PRECNAME(scalbln)
#include "testdriver.h"

#define ONE_P HEXCONSTE(1, 0x3fff, 0x80000000L, 0x00000000L)
#define ONE_M HEXCONSTE(-1, 0xbfff, 0x80000000L, 0x00000000L)


#if __LDBL_MIN_EXP__ <= (-16382)
#define MIN_HALF_P HEXCONSTE(+8.40525785778023376566e-4933, 0x0000, 0x40000000L, 0x00000000L) /* min_value / 2 */
#define MIN_HALF_M HEXCONSTE(-8.40525785778023376566e-4933, 0x8000, 0x40000000L, 0x00000000L) /* -min_value / 2 */
#define MIN_P_2    HEXCONSTE(+3.36205184148942639058e-4932, 0x0000, 0x7fff8000L, 0x00000000L) /* min_value * 0x0.ffffp0 */
#define MIN_M_2    HEXCONSTE(-3.36205184148942639058e-4932, 0x8000, 0x7fff8000L, 0x00000000L) /* -min_value * 0x0.ffffp0 */
#define MIN_P_15   HEXCONSTE(+2.52157735733407012970e-4932, 0x0000, 0xc0000000L, 0x00000000L) /* min_value * 1.5 */
#define MIN_M_15   HEXCONSTE(-2.52157735733407012970e-4932, 0x8000, 0xc0000000L, 0x00000000L) /* -min_value * 1.5 */
#define MIN_P_125  HEXCONSTE(+2.10131446444505844141e-4932, 0x0000, 0xa0000000L, 0x00000000L) /* min_value * 1.25 */
#define MIN_M_125  HEXCONSTE(-2.10131446444505844141e-4932, 0x8000, 0xa0000000L, 0x00000000L) /* -min_value * 1.25 */
#define MIN_4TH_P  HEXCONSTE(+4.20262892889011688283e-4933, 0x0000, 0x20000000L, 0x00000000L) /* min_value / 4 */
#define MIN_4TH_M  HEXCONSTE(-4.20262892889011688283e-4933, 0x8000, 0x20000000L, 0x00000000L) /* -min_value / 4 */
#define SUBNORM2_P HEXCONSTE(+7.29039906376494920506e-4951, 0x0000, 0x00000000L, 0x00000002L) /* subnorm_value * 2 */
#define SUBNORM2_M HEXCONSTE(-7.29039906376494920506e-4951, 0x8000, 0x00000000L, 0x00000002L) /* -subnorm_value * 2 */
#else
#define MIN_HALF_P HEXCONSTE(+1.68105157155604675313e-4932, 0x0000, 0x40000000L, 0x00000000L) /* min_value / 2 */
#define MIN_HALF_M HEXCONSTE(-1.68105157155604675313e-4932, 0x8000, 0x40000000L, 0x00000000L) /* -min_value / 2 */
#define MIN_P_2    HEXCONSTE(+3.36205184148942639058e-4932, 0x0000, 0x7fff8000L, 0x00000000L) /* min_value * 0x0.ffffp0 */
#define MIN_M_2    HEXCONSTE(-3.36205184148942639058e-4932, 0x8000, 0x7fff8000L, 0x00000000L) /* -min_value * 0x0.ffffp0 */
#define MIN_P_15   HEXCONSTE(+5.04315471466814025939e-4932, 0x0001, 0xc0000000L, 0x00000000L) /* min_value * 1.5 */
#define MIN_M_15   HEXCONSTE(-5.04315471466814025939e-4932, 0x8001, 0xc0000000L, 0x00000000L) /* -min_value * 1.5 */
#define MIN_P_125  HEXCONSTE(+4.20262892889011688283e-4932, 0x0001, 0xa0000000L, 0x00000000L) /* min_value * 1.25 */
#define MIN_M_125  HEXCONSTE(-4.20262892889011688283e-4932, 0x8001, 0xa0000000L, 0x00000000L) /* -min_value * 1.25 */
#define MIN_4TH_P  HEXCONSTE(+8.40525785778023376566e-4933, 0x0000, 0x20000000L, 0x00000000L) /* min_value / 4 */
#define MIN_4TH_M  HEXCONSTE(-8.40525785778023376566e-4933, 0x8000, 0x20000000L, 0x00000000L) /* -min_value / 4 */
#define SUBNORM2_P HEXCONSTE(+7.29039906376494920506e-4951, 0x0000, 0x00000000L, 0x00000002L) /* subnorm_value * 2 */
#define SUBNORM2_M HEXCONSTE(-7.29039906376494920506e-4951, 0x8000, 0x00000000L, 0x00000002L) /* -subnorm_value * 2 */
#endif

#define plus_uflow ZERO_P
#define minus_uflow ZERO_M
#define plus_oflow INF_P
#define minus_oflow INF_M


static test_fl_f_data const scalbln_data[] = {
	{ __LINE__, ZERO_P, 0, ZERO_P, 0 },
	{ __LINE__, ZERO_M, 0, ZERO_M, FLAG_FAIL_ARANYM2 },
	
	{ __LINE__, INF_P, 1, INF_P, FLAG_XFAIL_LINUX },
	{ __LINE__, INF_M, 1, INF_M, FLAG_XFAIL_LINUX },
	{ __LINE__, QNAN_P, 1, QNAN_P, 0 },
	{ __LINE__, QNAN_M, 1, QNAN_P, 0 },
	{ __LINE__, SNAN_P, 1, QNAN_P, 0 },
	{ __LINE__, SNAN_M, 1, QNAN_P, 0 },
	
	{ __LINE__, HEXCONSTE(0.8L, 0x3ffe, 0xccccccccL, 0xcccccccdL), 4, HEXCONSTE(12.8L, 0x4002, 0xccccccccL, 0xcccccccdL), 0 },
	{ __LINE__, HEXCONSTE(-0.854375L, 0xbffe, 0xdab851ebL, 0x851eb852L), 5, HEXCONSTE(-27.34L, 0xc003, 0xdab851ebL, 0x851eb852L), 0 },
	
	{ __LINE__, ONE_P, 0, ONE_P, 0 },
	
#ifdef TEST_LDOUBLE
	{ __LINE__, MIN_HALF_P, 0, MIN_HALF_P, 0 },
	{ __LINE__, MIN_HALF_M, 0, MIN_HALF_M, 0 },
	{ __LINE__, MIN_HALF_P, 1, MIN_P, FLAG_MINVAL|FLAG_SUBNORM },
	{ __LINE__, MIN_HALF_M, 1, MIN_M, FLAG_MINVAL|FLAG_SUBNORM },
	{ __LINE__, MIN_P_2, 0, MIN_P_2, 0 },
	{ __LINE__, MIN_M_2, 0, MIN_M_2, 0 },
	{ __LINE__, SUBNORM_P, 0, SUBNORM_P, FLAG_SUBNORM },
	{ __LINE__, SUBNORM_M, 0, SUBNORM_M, FLAG_SUBNORM },
	{ __LINE__, SUBNORM_P, LDBL_MANT_DIG - 1, MIN_P, FLAG_SUBNORM },
	{ __LINE__, SUBNORM_M, LDBL_MANT_DIG - 1, MIN_M, FLAG_SUBNORM },
#endif
	
#ifdef TEST_LDOUBLE
	{ __LINE__, MIN_P, -LDBL_MANT_DIG + 1, SUBNORM_P, FLAG_SUBNORM },
	{ __LINE__, MIN_M, -LDBL_MANT_DIG + 1, SUBNORM_M, FLAG_SUBNORM },
	{ __LINE__, MIN_P, -LDBL_MANT_DIG, plus_uflow, FLAG_MINVAL|FLAG_SUBNORM },
	{ __LINE__, MIN_M, -LDBL_MANT_DIG, minus_uflow, FLAG_MINVAL|FLAG_SUBNORM },
	{ __LINE__, MIN_P, -LDBL_MANT_DIG - 1, plus_uflow, FLAG_MINVAL|FLAG_SUBNORM },
	{ __LINE__, MIN_M, -LDBL_MANT_DIG - 1, minus_uflow, FLAG_MINVAL|FLAG_SUBNORM },
	{ __LINE__, MIN_P_15, -LDBL_MANT_DIG + 1, SUBNORM2_P, FLAG_SUBNORM },
	{ __LINE__, MIN_M_15, -LDBL_MANT_DIG + 1, SUBNORM2_M, FLAG_MINVAL|FLAG_SUBNORM },
	{ __LINE__, MIN_P_15, -LDBL_MANT_DIG, SUBNORM_P, FLAG_SUBNORM },
	{ __LINE__, MIN_M_15, -LDBL_MANT_DIG, SUBNORM_M, FLAG_SUBNORM },
	{ __LINE__, MIN_P_15, -LDBL_MANT_DIG - 1, plus_uflow, 0 },
	{ __LINE__, MIN_M_15, -LDBL_MANT_DIG - 1, minus_uflow, 0 },
	{ __LINE__, MIN_P_125, -LDBL_MANT_DIG + 1, SUBNORM_P, FLAG_SUBNORM }, /* works in STonX, but fails in ARAnyM */
	{ __LINE__, MIN_M_125, -LDBL_MANT_DIG + 1, SUBNORM_M, FLAG_XFAIL_LINUX|FLAG_SUBNORM },
	{ __LINE__, MIN_P_125, -LDBL_MANT_DIG, SUBNORM_P, FLAG_XFAIL_LINUX|FLAG_SUBNORM },
	{ __LINE__, MIN_M_125, -LDBL_MANT_DIG, SUBNORM_M, FLAG_SUBNORM }, /* works in STonX, but fails in ARAnyM */
	{ __LINE__, MIN_P_125, -LDBL_MANT_DIG - 1, plus_uflow, 0 },
	{ __LINE__, MIN_M_125, -LDBL_MANT_DIG - 1, minus_uflow, 0 },
#endif
	
	{ __LINE__, ONE_P, INT_MAX, plus_oflow, FLAG_FAIL_ARANYM }, /* glibc returns nan */
	{ __LINE__, ONE_P, INT_MIN, plus_uflow, 0 },
	{ __LINE__, MAX_P, INT_MAX, plus_oflow, 0 },
	{ __LINE__, MAX_P, INT_MIN, plus_uflow, 0 },
	{ __LINE__, MIN_P, INT_MAX, plus_oflow, 0 },
	{ __LINE__, MIN_P, INT_MIN, plus_uflow, 0 },
#ifdef TEST_LDOUBLE
	{ __LINE__, MIN_4TH_P, INT_MAX, plus_oflow, 0 },
	{ __LINE__, MIN_4TH_P, INT_MIN, plus_uflow, 0 },
	{ __LINE__, SUBNORM_P, INT_MAX, plus_oflow, FLAG_SUBNORM },
	{ __LINE__, SUBNORM_P, INT_MIN, plus_uflow, FLAG_SUBNORM },
#endif
	
	{ __LINE__, ONE_M, INT_MAX, minus_oflow, FLAG_FAIL_ARANYM }, /* glibc returns nan */
	{ __LINE__, ONE_M, INT_MIN, minus_uflow, 0 },
#ifdef TEST_LDOUBLE
	{ __LINE__, MAX_M, INT_MAX, minus_oflow, 0 },
	{ __LINE__, MAX_M, INT_MIN, minus_uflow, 0 },
	{ __LINE__, MIN_M, INT_MAX, minus_oflow, 0 },
	{ __LINE__, MIN_M, INT_MIN, minus_uflow, 0 },
	{ __LINE__, MIN_4TH_M, INT_MAX, minus_oflow, 0 },
	{ __LINE__, MIN_4TH_M, INT_MIN, minus_uflow, FLAG_XFAIL_LINUX|FLAG_SUBNORM },
	{ __LINE__, SUBNORM_M, INT_MAX, minus_oflow, FLAG_SUBNORM },
	{ __LINE__, SUBNORM_M, INT_MIN, minus_uflow, FLAG_SUBNORM },
#endif

	{ __LINE__, ONE_P, LONG_MAX, plus_oflow, FLAG_FAIL_ARANYM }, /* glibc returns nan */
	{ __LINE__, ONE_P, LONG_MIN, plus_uflow, 0 },
	{ __LINE__, MAX_P, LONG_MAX, plus_oflow, 0 },
	{ __LINE__, MAX_P, LONG_MIN, plus_uflow, 0 },
	{ __LINE__, MIN_P, LONG_MAX, plus_oflow, 0 },
	{ __LINE__, MIN_P, LONG_MIN, plus_uflow, 0 },
#ifdef TEST_LDOUBLE
	{ __LINE__, MIN_4TH_P, LONG_MAX, plus_oflow, 0 },
	{ __LINE__, MIN_4TH_P, LONG_MIN, plus_uflow, 0 },
	{ __LINE__, SUBNORM_P, LONG_MAX, plus_oflow, FLAG_SUBNORM },
	{ __LINE__, SUBNORM_P, LONG_MIN, plus_uflow, FLAG_SUBNORM },
#endif
	
	{ __LINE__, ONE_M, LONG_MAX, minus_oflow, FLAG_FAIL_ARANYM }, /* glibc returns nan */
	{ __LINE__, ONE_M, LONG_MIN, minus_uflow, FLAG_FAIL_SOFTFLOAT },
#ifdef TEST_LDOUBLE
	{ __LINE__, MAX_M, LONG_MAX, minus_oflow, 0 },
	{ __LINE__, MAX_M, LONG_MIN, minus_uflow, 0 },
	{ __LINE__, MIN_M, LONG_MAX, minus_oflow, 0 },
	{ __LINE__, MIN_M, LONG_MIN, minus_uflow, 0 },
	{ __LINE__, MIN_4TH_M, LONG_MAX, minus_oflow, 0 },
	{ __LINE__, MIN_4TH_M, LONG_MIN, minus_uflow, FLAG_XFAIL_LINUX|FLAG_SUBNORM },
	{ __LINE__, SUBNORM_M, LONG_MAX, minus_oflow, FLAG_SUBNORM },
	{ __LINE__, SUBNORM_M, LONG_MIN, minus_uflow, FLAG_SUBNORM },
#endif

	{ __LINE__, ONE_P, 0, ONE_P, 0 },
	{ __LINE__, ONE_M, 0, ONE_M, 0 },

	{ __LINE__, HEXCONSTE(5.0, 0x4001, 0xa0000000L, 0x00000000L), 1, HEXCONSTE(10.0, 0x4002, 0xa0000000L, 0x00000000L), 0 },
	{ __LINE__, HEXCONSTE(5.0, 0x4001, 0xa0000000L, 0x00000000L), -1, HEXCONSTE(2.5, 0x4000, 0xa0000000L, 0x00000000L), 0 },
	{ __LINE__, HEXCONSTE("0x1p16382", 0x7ffd, 0x80000000L, 0x0L), 1, HEXCONSTE("0x1p16383", 0x7ffe, 0x80000000L, 0x0L), 0 },
	{ __LINE__, HEXCONSTE("0x1p16383", 0x7ffe, 0x80000000L, 0x0L), 1, INF_P, 0 },
};

int main(int argc, char **argv)
{
	int status;
	
	status = 0;
	
	test_init(argc, argv);
	
	status |= test_table_fl_f(scalbln_data, ARRAY_SIZE(scalbln_data), __FILE__);
	
	return status ? EXIT_FAILURE : EXIT_SUCCESS;
}

#else

int main(void)
{
	return 77;
}

#endif
