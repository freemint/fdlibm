#include "testconfig.h"
#if defined(HAVE_ISLESS) && defined(HAVE_ISLESSGREATER) && defined(HAVE_ISUNORDERED)
#define TEST_FUNC_I_FF __MATH_PRECNAME(wrap_fcmp)
#define NO_PRINTF_DOUBLE
static uint32_t TEST_FUNC_I_FF(REAL x, REAL y)
{
	uint32_t cc = 0;
	
	if (isless(x, y))
		cc |= FPSR_CCB_NEGATIVE;
	if (!islessgreater(x, y))
		cc |= FPSR_CCB_ZERO;
	if (isunordered(x, y))
		cc |= FPSR_CCB_NAN;
	return cc;
}
#include "testdriver.h"



typedef struct { int line; union ld_union x; union ld_union y; uint32_t cc; flag_t flags; } test_i_ff_data;

#define TEST_OP_BODY_I_FF(_x, _y) fp0 = _x; cc = TEST_FUNC_I_FF(fp0.x, _y.x)


static int test_table_i_ff(const test_i_ff_data *data, size_t n, const char *file)
{
	size_t l;
	int i;
	int status = 0, this_fail;
	union ld_union fp0;
	uint32_t cc;
	
	for (l = 0; l < n; l++)
	{
		numtests++;
		if (!SKIP_TEST(data[l].x, data[l].flags) && !SKIP_TEST(data[l].y, data[l].flags))
		{
			if (testonly == 0 || testonly == numtests)
			{
				for (i = 0; i < jit_loops; i++)
				{
					TEST_OP_BODY_I_FF(data[l].x, data[l].y);
					/*
					 * fcmp on '040 sets N flag if either (or both) parameters are negative nan
					 */
					if ((data[l].x.v.exponent == 0xffff && ((data[l].x.v.mantissa0 & 0x7fffffff) != 0 || data[l].x.v.mantissa1 != 0)) ||
						(data[l].y.v.exponent == 0xffff && ((data[l].y.v.mantissa0 & 0x7fffffff) != 0 || data[l].y.v.mantissa1 != 0)))
						cc &= ~FPSR_CCB_NEGATIVE;
					this_fail = data[l].cc != (cc & FPSR_CCB);
					if (this_fail)
					{
						fprintf(stderr, "%s:%d: test %d(%d): expected %c%c%c%c, got %c%c%c%c" CR "\n", file, data[l].line, numtests, i,
							data[l].cc & FPSR_CCB_NEGATIVE ? 'N' : '-',
							data[l].cc & FPSR_CCB_ZERO ? 'Z' : '-',
							data[l].cc & FPSR_CCB_INFINITY ? 'I' : '-',
							data[l].cc & FPSR_CCB_NAN ? 'U' : '-',
							cc & FPSR_CCB_NEGATIVE ? 'N' : '-',
							cc & FPSR_CCB_ZERO ? 'Z' : '-',
							cc & FPSR_CCB_INFINITY ? 'I' : '-',
							cc & FPSR_CCB_NAN ? 'U' : '-');
						status |= this_fail;
						break;
					}
				}
			}
		}
	}
	
	return status;
}



static test_i_ff_data const fcmp_data[] = {
	{ __LINE__, HEXCONSTE(1.0, 0x3fff, 0x80000000L, 0x00000000L), HEXCONSTE(2.0, 0x4000, 0x80000000L, 0x00000000L),  FPSR_CCB_NEGATIVE, 0 },
	{ __LINE__, HEXCONSTE(1.0, 0x3fff, 0x80000000L, 0x00000000L), HEXCONSTE(-2.0, 0xc000, 0x80000000L, 0x00000000L), 0, 0 },
	{ __LINE__, HEXCONSTE(-1.0, 0xbfff, 0x80000000L, 0x00000000L), HEXCONSTE(2.0, 0x4000, 0x80000000L, 0x00000000L), FPSR_CCB_NEGATIVE, 0 },
	{ __LINE__, HEXCONSTE(-1.0, 0xbfff, 0x80000000L, 0x00000000L), HEXCONSTE(-2.0, 0xc000, 0x80000000L, 0x00000000L), 0, 0 },
	{ __LINE__, HEXCONSTE(1.0, 0x3fff, 0x80000000L, 0x00000000L), ZERO_P, 0, 0 },
	{ __LINE__, HEXCONSTE(1.0, 0x3fff, 0x80000000L, 0x00000000L), ZERO_M, 0, 0 },
	{ __LINE__, HEXCONSTE(-1.0, 0xbfff, 0x80000000L, 0x00000000L), ZERO_P, FPSR_CCB_NEGATIVE, 0 },
	{ __LINE__, HEXCONSTE(-1.0, 0xbfff, 0x80000000L, 0x00000000L), ZERO_M, FPSR_CCB_NEGATIVE, 0 },
	{ __LINE__, HEXCONSTE(1.0, 0x3fff, 0x80000000L, 0x00000000L), INF_P, FPSR_CCB_NEGATIVE, 0 },
	{ __LINE__, HEXCONSTE(1.0, 0x3fff, 0x80000000L, 0x00000000L), INF_M, 0, 0 },
	{ __LINE__, HEXCONSTE(-1.0, 0xbfff, 0x80000000L, 0x00000000L), INF_P, FPSR_CCB_NEGATIVE, 0 },
	{ __LINE__, HEXCONSTE(-1.0, 0xbfff, 0x80000000L, 0x00000000L), INF_M, 0, 0 },
	{ __LINE__, ZERO_P, HEXCONSTE(2.0, 0x4000, 0x80000000L, 0x00000000L),  FPSR_CCB_NEGATIVE, 0 },
	{ __LINE__, ZERO_P, HEXCONSTE(-2.0, 0xc000, 0x80000000L, 0x00000000L), 0, 0 },
	{ __LINE__, ZERO_M, HEXCONSTE(2.0, 0x4000, 0x80000000L, 0x00000000L), FPSR_CCB_NEGATIVE, 0 },
	{ __LINE__, ZERO_M, HEXCONSTE(-2.0, 0xc000, 0x80000000L, 0x00000000L), 0, 0 },
	{ __LINE__, ZERO_P, ZERO_P, FPSR_CCB_ZERO, 0 },
	{ __LINE__, ZERO_P, ZERO_M, FPSR_CCB_ZERO, 0 },
	{ __LINE__, ZERO_M, ZERO_P, FPSR_CCB_ZERO|FPSR_CCB_NEGATIVE, 0 },
	{ __LINE__, ZERO_M, ZERO_M, FPSR_CCB_ZERO|FPSR_CCB_NEGATIVE, 0 },
	{ __LINE__, ZERO_P, INF_P, FPSR_CCB_NEGATIVE, 0 },
	{ __LINE__, ZERO_P, INF_M, 0, 0 },
	{ __LINE__, ZERO_M, INF_P, FPSR_CCB_NEGATIVE, 0 },
	{ __LINE__, ZERO_M, INF_M, 0, 0 },
	{ __LINE__, INF_P, HEXCONSTE(1.0, 0x3fff, 0x80000000L, 0x00000000L), 0, 0 },
	{ __LINE__, INF_P, HEXCONSTE(-1.0, 0xbfff, 0x80000000L, 0x00000000L), 0, 0 },
	{ __LINE__, INF_M, HEXCONSTE(1.0, 0x3fff, 0x80000000L, 0x00000000L), FPSR_CCB_NEGATIVE, 0 },
	{ __LINE__, INF_M, HEXCONSTE(-1.0, 0xbfff, 0x80000000L, 0x00000000L), FPSR_CCB_NEGATIVE, 0 },
	{ __LINE__, INF_P, ZERO_P, 0, 0 },
	{ __LINE__, INF_P, ZERO_M, 0, 0 },
	{ __LINE__, INF_M, ZERO_P, FPSR_CCB_NEGATIVE, 0 },
	{ __LINE__, INF_M, ZERO_M, FPSR_CCB_NEGATIVE, 0 },
	{ __LINE__, INF_P, INF_P, FPSR_CCB_ZERO, 0 },
	{ __LINE__, INF_P, INF_M, 0, 0 },
	{ __LINE__, INF_M, INF_P, FPSR_CCB_NEGATIVE, 0 },
	{ __LINE__, INF_M, INF_M, FPSR_CCB_ZERO|FPSR_CCB_NEGATIVE, 0 },

	{ __LINE__, ZERO_P, QNAN_P, FPSR_CCB_NAN, 0 },
	{ __LINE__, ZERO_P, QNAN_M, FPSR_CCB_NAN, 0 },
	{ __LINE__, ZERO_M, QNAN_P, FPSR_CCB_NAN, 0 },
	{ __LINE__, ZERO_M, QNAN_M, FPSR_CCB_NAN, 0 },
	{ __LINE__, INF_P, QNAN_P, FPSR_CCB_NAN, 0 },
	{ __LINE__, INF_P, QNAN_M, FPSR_CCB_NAN, 0 },
	{ __LINE__, INF_M, QNAN_P, FPSR_CCB_NAN, 0 },
	{ __LINE__, INF_M, QNAN_M, FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_P, ZERO_P, FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_M, ZERO_P, FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_P, ZERO_M, FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_M, ZERO_M, FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_P, INF_P, FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_M, INF_P, FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_P, INF_M, FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_M, INF_M, FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_P, QNAN_P, FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_M, QNAN_P, FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_P, QNAN_M, FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_M, QNAN_M, FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_P, QNAN_P, FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_M, QNAN_P, FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_P, QNAN_M, FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_M, QNAN_M, FPSR_CCB_NAN, 0 },
	{ __LINE__, HEXCONSTE(1.0, 0x3fff, 0x80000000L, 0x00000000L), QNAN_P, FPSR_CCB_NAN, 0 },
	{ __LINE__, HEXCONSTE(1.0, 0x3fff, 0x80000000L, 0x00000000L), QNAN_M, FPSR_CCB_NAN, 0 },
	{ __LINE__, HEXCONSTE(-1.0, 0xbfff, 0x80000000L, 0x00000000L), QNAN_P, FPSR_CCB_NAN, 0 },
	{ __LINE__, HEXCONSTE(-1.0, 0xbfff, 0x80000000L, 0x00000000L), QNAN_M, FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_P, HEXCONSTE(1.0, 0x3fff, 0x80000000L, 0x00000000L), FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_M, HEXCONSTE(1.0, 0x3fff, 0x80000000L, 0x00000000L), FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_P, HEXCONSTE(-1.0, 0xbfff, 0x80000000L, 0x00000000L), FPSR_CCB_NAN, 0 },
	{ __LINE__, QNAN_M, HEXCONSTE(-1.0, 0xbfff, 0x80000000L, 0x00000000L), FPSR_CCB_NAN, 0 },

	{ __LINE__, HEXCONST_PI, HEXCONSTE(3.0, 0x4000, 0xc0000000L, 0x00000000L), 0, 0 },
};


int main(int argc, char **argv)
{
	int status;
	
	status = 0;
	
	test_init(argc, argv);
	
	status |= test_table_i_ff(fcmp_data, ARRAY_SIZE(fcmp_data), __FILE__);
	
	return status ? EXIT_FAILURE : EXIT_SUCCESS;
}

#else

int main(void)
{
	return 77;
}

#endif
