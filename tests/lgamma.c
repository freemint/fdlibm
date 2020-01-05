#include "testconfig.h"
#define TEST_FUNC_F_F __MATH_PRECNAME(lgamma)
#include "testdriver.h"

#define M_LOG_SQRT_PI         HEXCONSTE(0.57236494292470008707171367567652933L, 0x3ffe, 0x92868247L, 0x3d0de85fL)  /* log(sqrt(M_PI))  */
#define M_LOG_2_SQRT_PI       HEXCONSTE(1.265512123484645396488945797134706L, 0x3fff, 0xa1fc4d1fL, 0x876eb105L)    /* log(2*sqrt(M_PIl))  */


static test_f_f_data const lgamma_data[] = {
	{ __LINE__, QNAN_P, QNAN_P, /* signgam = 0, */ 0 },
	{ __LINE__, QNAN_M, QNAN_M, /* signgam = 0, */ 0 },
	{ __LINE__, SNAN_P, QNAN_P, /* signgam = 0, */ 0 },
	{ __LINE__, SNAN_M, QNAN_M, /* signgam = 0, */ 0 },
	{ __LINE__, INF_P, INF_P, /* signgam = 1, */ 0 },
	{ __LINE__, INF_M, INF_P, /* signgam = 0, */ 0 },
	{ __LINE__, ZERO_P, INF_P, /* signgam = 1, */ DIVIDE_BY_ZERO_EXCEPTION|ERRNO_ERANGE },
	{ __LINE__, ZERO_M, INF_P, /* signgam = -1, */ DIVIDE_BY_ZERO_EXCEPTION|ERRNO_ERANGE },
	{ __LINE__, MAX_M, INF_P, /* signgam = 0, */ DIVIDE_BY_ZERO_EXCEPTION|ERRNO_ERANGE },
	{ __LINE__, MAX_P, INF_P, /* signgam = 1, */ OVERFLOW_EXCEPTION|ERRNO_ERANGE },

	/* lgamma (x) == +inf plus divide by zero exception for integer x <= 0.  */
	{ __LINE__, HEXCONSTE(-3, 0xc000, 0xc0000000L, 0x00000000L), INF_P, /* signgam = 0, */ DIVIDE_BY_ZERO_EXCEPTION|ERRNO_ERANGE },

	{ __LINE__, HEXCONSTE(1, 0x3fff, 0x80000000L, 0x00000000L), ZERO_P, /* signgam = 1, */ 0 },
	{ __LINE__, HEXCONSTE(2, 0x4000, 0x80000000L, 0x00000000L), ZERO_P, /* signgam = 1, */ 0 },
	{ __LINE__, HEXCONSTE(3, 0x4000, 0xc0000000L, 0x00000000L), HEXCONST_LN2, /* signgam = 1, */ 0 },

	{ __LINE__, HEXCONSTE(0.5, 0x3ffe, 0x80000000L, 0x00000000L), M_LOG_SQRT_PI, /* signgam = 1, */ 0 },
	{ __LINE__, HEXCONSTE(-0.5, 0xbffe, 0x80000000L, 0x00000000L), M_LOG_2_SQRT_PI, /* signgam = -1 */ FLAG_INEXACT },
	{ __LINE__, HEXCONSTE(0.7L, 0x3ffe, 0xb3333333L, 0x33333333L), HEXCONSTE(0.260867246531666514385732417016759578L, 0x3ffd, 0x85906448L, 0xe6f39423L), /* signgam = 1, */ FLAG_INEXACT },
	{ __LINE__, HEXCONSTE(1.2L, 0x3fff, 0x99999999L, 0x9999999aL), HEXCONSTE(-0.853740900033158497197028392998854470e-1L, 0xbffb, 0xaed89c63L, 0xeb863966L), /* signgam = 1, */ FLAG_INEXACT },
};


int main(int argc, char **argv)
{
	int status;
	
	status = 0;
	
	test_init(argc, argv);
	
	status |= test_table_f_f(lgamma_data, ARRAY_SIZE(lgamma_data), __FILE__);
	
	return status ? EXIT_FAILURE : EXIT_SUCCESS;
}
