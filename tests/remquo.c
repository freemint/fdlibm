#include "testconfig.h"
#ifdef HAVE_REMQUO
#define TEST_FUNC_FF_FI __MATH_PRECNAME(remquo)
#include "testdriver.h"

typedef struct { int line; ld_union x; ld_union y; ld_union e; int exp; flag_t flags; } test_ff_fi_data;

#ifdef TEST_LDOUBLE
#define TEST_OP_BODY_FF_FI(_x, _y, q) r.x = TEST_FUNC_FF_FI(_x.x, _y.x, &(q))
#endif
#ifdef TEST_DOUBLE
#define TEST_OP_BODY_FF_FI(_x, _y, q) r.x = my_extenddfxf2(TEST_FUNC_FF_FI(my_truncxfdf2(_x.x), my_truncxfdf2(_y.x), &(q)))
#endif
#ifdef TEST_FLOAT
#define TEST_OP_BODY_FF_FI(_x, _y, q) r.x = my_extenddfxf2(TEST_FUNC_FF_FI(my_truncxfsf2(_x.x), my_truncxfsf2(_y.x), &(q)))
#endif

static int test_table_ff_fi(const test_ff_fi_data *data, size_t n, const char *file)
{
	size_t l;
	int i;
	int status = 0, this_fail;
	ld_union r;
	int q;

	for (l = 0; l < n; l++)
	{
		numtests++;
		if (!SKIP_TEST(data[l].x, data[l].flags))
		{
			if (testonly == 0 || testonly == numtests)
			{
				for (i = 0; i < jit_loops; i++)
				{
					TEST_OP_BODY_FF_FI(data[l].x, data[l].y, q);
					this_fail = check_fp(data[l].e.v.exponent, data[l].e.v.mantissa0, data[l].e.v.mantissa1, &r, data[l].flags, i, file, data[l].line);
					this_fail |= check_int(data[l].exp, q, data[l].flags, i, file, data[l].line);
					if (this_fail)
					{
						status |= this_fail != 0;
						break;
					}
				}
			}
		}
	}
	
	return status;
}


static test_ff_fi_data const remquo_data[] = {
	{ __LINE__, HEXCONSTE(1, 0x3fff, 0x80000000L, 0x00000000L), ZERO_P, QNAN_P, 0, NO_INEXACT_EXCEPTION|INVALID_EXCEPTION },
	{ __LINE__, HEXCONSTE(1, 0x3fff, 0x80000000L, 0x00000000L), ZERO_M, QNAN_P, 0, NO_INEXACT_EXCEPTION|INVALID_EXCEPTION },
	{ __LINE__, INF_P, HEXCONSTE(1, 0x3fff, 0x80000000L, 0x00000000L), QNAN_P, 0, NO_INEXACT_EXCEPTION|INVALID_EXCEPTION },
	{ __LINE__, INF_M, HEXCONSTE(1, 0x3fff, 0x80000000L, 0x00000000L), QNAN_P, 0, NO_INEXACT_EXCEPTION|INVALID_EXCEPTION },
	{ __LINE__, QNAN_P, QNAN_P, QNAN_P, 0, NO_INEXACT_EXCEPTION },

	{ __LINE__, HEXCONSTE(1.625, 0x3fff, 0xd0000000L, 0x00000000L), HEXCONSTE(1.0, 0x3fff, 0x80000000L, 0x00000000L), HEXCONSTE(-0.375, 0xbffd, 0xc0000000L, 0x00000000L), 2, NO_INEXACT_EXCEPTION },
	{ __LINE__, HEXCONSTE(-1.625, 0xbfff, 0xd0000000L, 0x00000000L), HEXCONSTE(1.0, 0x3fff, 0x80000000L, 0x00000000L), HEXCONSTE(0.375, 0x3ffd, 0xc0000000L, 0x00000000L), -2, NO_INEXACT_EXCEPTION },
	{ __LINE__, HEXCONSTE(1.625, 0x3fff, 0xd0000000L, 0x00000000L), HEXCONSTE(-1.0, 0xbfff, 0x80000000L, 0x00000000L), HEXCONSTE(-0.375, 0xbffd, 0xc0000000L, 0x00000000L), -2, NO_INEXACT_EXCEPTION },
	{ __LINE__, HEXCONSTE(-1.625, 0xbfff, 0xd0000000L, 0x00000000L), HEXCONSTE(-1.0, 0xbfff, 0x80000000L, 0x00000000L), HEXCONSTE(0.375, 0x3ffd, 0xc0000000L, 0x00000000L), 2, NO_INEXACT_EXCEPTION },

	{ __LINE__, HEXCONSTE(5, 0x4001, 0xa0000000L, 0x00000000L), HEXCONSTE(2, 0x4000, 0x80000000L, 0x00000000L), HEXCONSTE(1, 0x3fff, 0x80000000L, 0x00000000L), 2, NO_INEXACT_EXCEPTION },
	{ __LINE__, HEXCONSTE(3, 0x4000, 0xc0000000L, 0x00000000L), HEXCONSTE(2, 0x4000, 0x80000000L, 0x00000000L), HEXCONSTE(-1, 0xbfff, 0x80000000L, 0x00000000L), 2, NO_INEXACT_EXCEPTION },
};


int main(int argc, char **argv)
{
	int status;
	
	status = 0;
	
	test_init(argc, argv);
	
	status |= test_table_ff_fi(remquo_data, ARRAY_SIZE(remquo_data), __FILE__);
	
	return status ? EXIT_FAILURE : EXIT_SUCCESS;
}

#else

int main(void)
{
	return 77;
}

#endif
