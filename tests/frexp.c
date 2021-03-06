#include "testconfig.h"
#define TEST_FUNC_FIP_F __MATH_PRECNAME(frexp)
#include "testdriver.h"



static test_fip_f_data const frexp_data[] = {
	{ __LINE__, ZERO_P, ZERO_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, ZERO_M, ZERO_M, 0, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM2 },
	{ __LINE__, INF_P, INF_P, 0, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM|FLAG_FAIL_HARDFLOAT }, /* glibc returns NaN; inline version in math-68881.h returns NaN */
	{ __LINE__, INF_M, INF_M, 0, NO_INEXACT_EXCEPTION|FLAG_FAIL_ARANYM|FLAG_FAIL_HARDFLOAT }, /* glibc returns NaN; inline version in math-68881.h returns NaN */
	{ __LINE__, QNAN_P, QNAN_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, QNAN_M, QNAN_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, SNAN_P, QNAN_P, 0, NO_INEXACT_EXCEPTION },
	{ __LINE__, SNAN_M, QNAN_P, 0, NO_INEXACT_EXCEPTION },

	{ __LINE__, HEXCONSTE(12.8, 0x4002, 0xccccccccL, 0xcccccccdL), HEXCONSTE(0.8, 0x3ffe, 0xccccccccL, 0xcccccccdL), 4, NO_INEXACT_EXCEPTION },
	{ __LINE__, HEXCONSTE(-27.34, 0xc003, 0xdab851ebL, 0x851eb852L), HEXCONSTE(-0.854375L, 0xbffe, 0xdab851ebL, 0x851eb852L), 5, NO_INEXACT_EXCEPTION },

	{ __LINE__, HEXCONSTE(0.25, 0x3ffd, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), -1, 0 },
	{ __LINE__, HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), 0, 0 },
	{ __LINE__, HEXCONSTE(1.0, 0x3fff, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), 1, 0 },
	{ __LINE__, HEXCONSTE(2.0, 0x4000, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), 2, 0 },
	{ __LINE__, HEXCONSTE(4.0, 0x4001, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), 3, 0 },
	{ __LINE__, HEXCONSTE(8.0, 0x4002, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), 4, 0 },
	{ __LINE__, HEXCONSTE(256, 0x4007, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), 9, 0 },
	{ __LINE__, HEXCONSTE(4096, 0x400b, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), 13, 0 },
	{ __LINE__, HEXCONSTE(32768, 0x400e, 0x80000000L, 0x00000000L), HEXCONSTE(5.000000e-01, 0x3ffe, 0x80000000L, 0x00000000L), 16, 0 },
	{ __LINE__, HEXCONSTE(-1.0, 0xbfff, 0x80000000L, 0x00000000L), HEXCONSTE(-5.000000e-01, 0xbffe, 0x80000000L, 0x00000000L), 1, 0 },
	{ __LINE__, HEXCONSTE(-2.0, 0xc000, 0x80000000L, 0x00000000L), HEXCONSTE(-5.000000e-01, 0xbffe, 0x80000000L, 0x00000000L), 2, 0 },
	{ __LINE__, HEXCONSTE(-4.0, 0xc001, 0x80000000L, 0x00000000L), HEXCONSTE(-5.000000e-01, 0xbffe, 0x80000000L, 0x00000000L), 3, 0 },
	{ __LINE__, HEXCONSTE(-8.0, 0xc002, 0x80000000L, 0x00000000L), HEXCONSTE(-5.000000e-01, 0xbffe, 0x80000000L, 0x00000000L), 4, 0 },
	{ __LINE__, HEXCONSTE(-256, 0xc007, 0x80000000L, 0x00000000L), HEXCONSTE(-5.000000e-01, 0xbffe, 0x80000000L, 0x00000000L), 9, 0 },
	{ __LINE__, HEXCONSTE(-4096, 0xc00b, 0x80000000L, 0x00000000L), HEXCONSTE(-0.5, 0xbffe, 0x80000000L, 0x00000000L), 13, 0 },
	{ __LINE__, HEXCONSTE(-32768, 0xc00e, 0x80000000L, 0x00000000L), HEXCONSTE(-5.000000e-01, 0xbffe, 0x80000000L, 0x00000000L), 16, 0 },
	{ __LINE__, HEXCONSTE(1.234500e+00, 0x3fff, 0x9e041893L, 0x74bc6a7fL), HEXCONSTE(6.172500e-01, 0x3ffe, 0x9e041893L, 0x74bc6a7fL), 1, 0 },
	{ __LINE__, HEXCONSTE(1.234500e+10, 0x4020, 0xb7f47010L, 0x00000000L), HEXCONSTE(7.185736e-01, 0x3ffe, 0xb7f47010L, 0x00000000l), 34, 0 },
	{ __LINE__, HEXCONSTE(3.456700e+20, 0x4043, 0x95e9144cL, 0x7eaa3800L), HEXCONSTE(5.855878e-01, 0x3ffe, 0x95e9144cL, 0x7eaa3800L), 69, 0 },
	{ __LINE__, HEXCONSTE(3.456700e-05, 0x3ff0, 0x90fc089bL, 0xfd438487L), HEXCONSTE(5.663457e-01, 0x3ffe, 0x90fc089bL, 0xfd438487L), -14, 0 },
	{ __LINE__, HEXCONSTE(-1.234500e+00, 0xbfff, 0x9e041893L, 0x74bc6a7fL), HEXCONSTE(-6.172500e-01, 0xbffe, 0x9e041893L, 0x74bc6a7fL), 1, 0 },
	{ __LINE__, HEXCONSTE(-1.234500e+10, 0xc020, 0xb7f47010L, 0x00000000L), HEXCONSTE(-7.185736e-01, 0xbffe, 0xb7f47010L, 0x00000000L), 34, 0 },
	{ __LINE__, HEXCONSTE(-3.456700e+20, 0xc043, 0x95e9144cL, 0x7eaa3800L), HEXCONSTE(-5.855878e-01, 0xbffe, 0x95e9144cL, 0x7eaa3800L), 69, 0 },
	{ __LINE__, HEXCONSTE(-3.456700e-05, 0xbff0, 0x90fc089bL, 0xfd438487L), HEXCONSTE(-5.663457e-01, 0xbffe, 0x90fc089bL, 0xfd438487L), -14, 0 },

	{ __LINE__, HEXCONSTE(1.70141183460469231732e+38, 0x407e, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), 128, 0 },
	{ __LINE__, HEXCONSTE(-1.70141183460469231732e+38, 0xc07e, 0x80000000L, 0x00000000L), HEXCONSTE(-0.5, 0xbffe, 0x80000000L, 0x00000000L), 128, 0 },
	{ __LINE__, HEXCONSTE(1.17549435082228750797e-38, 0x3f81, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), -125, 0 },
	{ __LINE__, HEXCONSTE(5.87747175411143753984e-39, 0x3f80, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), -126, 0 },
	{ __LINE__, HEXCONSTE(1.40129846432481707092e-45, 0x3f6a, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), -148, 0 },
	{ __LINE__, HEXCONSTE(-1.40129846432481707092e-45, 0xbf6a, 0x80000000L, 0x00000000L), HEXCONSTE(-0.5, 0xbffe, 0x80000000L, 0x00000000L), -148, 0 },
	{ __LINE__, HEXCONSTE(8.98846567431157953865e+307, 0x43fe, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), 1024,  },
	{ __LINE__, HEXCONSTE(-8.98846567431157953865e+307, 0xc3fe, 0x80000000L, 0x00000000L), HEXCONSTE(-0.5, 0xbffe, 0x80000000L, 0x00000000L), 1024, 0 },
	{ __LINE__, HEXCONSTE(2.22507385850720138309e-308, 0x3c01, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), -1021, 0 },
	{ __LINE__, HEXCONSTE(1.11253692925360069155e-308, 0x3c00, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), -1022, 0 },
	{ __LINE__, HEXCONSTE(4.94065645841246544177e-324, 0x3bcd, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), -1073, 0 },
	{ __LINE__, HEXCONSTE(-4.94065645841246544177e-324, 0xbbcd, 0x80000000L, 0x00000000L), HEXCONSTE(-0.5, 0xbffe, 0x80000000L, 0x00000000L), -1073, 0 },
	{ __LINE__, HEXCONSTE(5.94865747678615882543e+4931, 0x7ffe, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), 16384, 0 },
	{ __LINE__, HEXCONSTE(-5.94865747678615882543e+4931, 0xfffe, 0x80000000L, 0x00000000L), HEXCONSTE(-0.5, 0xbffe, 0x80000000L, 0x00000000L), 16384, 0 },
	{ __LINE__, HEXCONSTE(3.36210314311209350626e-4932, 0x0001, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), -16381, 0 },
#if defined(TEST_LDOUBLE) && __LDBL_MIN_EXP__ <= (-16382)
	{ __LINE__, HEXCONSTE(1.68105157155604675313e-4932, 0x0000, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), -16381, FLAG_SUBNORM },
	{ __LINE__, HEXCONSTE(8.40525785778023376566e-4933, 0x0000, 0x40000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), -16382, FLAG_SUBNORM },
	{ __LINE__, HEXCONSTE(3.64519953188247460253e-4951, 0x0000, 0x00000000L, 0x00000001L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), -16444, FLAG_SUBNORM },
	{ __LINE__, HEXCONSTE(-3.64519953188247460253e-4951, 0x8000, 0x00000000L, 0x00000001L), HEXCONSTE(-0.5, 0xbffe, 0x80000000L, 0x00000000L), -16444, FLAG_SUBNORM },
#endif
#if defined(TEST_LDOUBLE) && __LDBL_MIN_EXP__ <= (-16381)
	{ __LINE__, HEXCONSTE(3.36210314311209350626e-4932, 0x0000, 0x80000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), -16381, 0 },
	{ __LINE__, HEXCONSTE(1.68105157155604675313e-4932, 0x0000, 0x40000000L, 0x00000000L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), -16382, 0 },
	{ __LINE__, HEXCONSTE(1.82259976594123730126e-4951, 0x0000, 0x00000000L, 0x00000001L), HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), -16444, 0 },
	{ __LINE__, HEXCONSTE(-1.82259976594123730126e-4951, 0x8000, 0x00000000L, 0x00000001L), HEXCONSTE(-0.5, 0xbffe, 0x80000000L, 0x00000000L), -16444, 0 },
#endif
};

int main(int argc, char **argv)
{
	int status;
	
	status = 0;
	
	test_init(argc, argv);
	
	status |= test_table_fip_f(frexp_data, ARRAY_SIZE(frexp_data), __FILE__);
	
	return status ? EXIT_FAILURE : EXIT_SUCCESS;
}
