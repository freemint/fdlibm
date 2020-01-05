#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

#ifndef __NO_LONGLONG

long long int __llrintl(long double x)
{
	int32_t se, j0;
	uint32_t i0, i1;
	long long int result;
	int32_t sx;
	int rm;

	rm = fegetround();

	GET_LDOUBLE_WORDS(se, i0, i1, x);

	sx = se & 0x8000;
	j0 = (se & IEEE854_LONG_DOUBLE_MAXEXP) - IEEE854_LONG_DOUBLE_BIAS;
	/* printf("%08x %08x %d\n", i0, i1, j0); */
	
	if (j0 < 31)
	{
		uint32_t i;
		uint32_t j;

		if (j0 == -IEEE854_LONG_DOUBLE_BIAS)
			return 0;
		/* Raise inexact if x != 0.  */
		if (j0 < 0 || ((i0 & IC(0x7fffffff)) | i1) != 0)
			feraiseexcept(FE_INEXACT);
		if (j0 < 0)
		{
			switch (rm)
			{
				case FE_TOWARDZERO: return 0;
				case FE_DOWNWARD: return sx ? -1 : 0;
				case FE_UPWARD: return sx ? 0 : 1;
			}
			/* case FE_TONEAREST: */
			if (j0 == -1 && ((i0 & IC(0x7fffffff)) | i1) != 0)
				return sx ? -1 : 1;
			return 0;
		}

		i = UC(0x7fffffff) >> j0;
		j = i0;
		switch (rm)
		{
		case FE_TOWARDZERO:
			break;
		case FE_DOWNWARD:
			if (sx && ((i0 & i) | i1) != 0)
			{
				j += UC(0x80000000) >> j0;
				if (j < i0)
				{
					j += j;
					j0++;
				}
			}
			break;
		case FE_UPWARD:
			if (!sx && ((i0 & i) | i1) != 0)
			{
				j += UC(0x80000000) >> j0;
				if (j < i0)
				{
					i = UC(0x7fffffff);
					j += j;
					j0++;
				}
			}
			break;
		case FE_TONEAREST:
		default:
			if ((i0 & (i >> 1)) != 0 || i1 != 0 || (i0 & (UC(0x80000000) >> j0)))
			{
				j += (UC(0x40000000) >> j0);
	
				if (j < i0)
				{
					j += j;
					j0++;
				}
			}
			break;
		}
		i0 = (j & ~i) | IC(0x80000000);
		result = i0 >> (31 - j0);
	} else if (j0 < (int32_t) (8 * sizeof(long long int)) - 1)
	{
		if (j0 >= 63)
		{
			result = (long long int) i0 << (j0 - 31);
			if ((j0 - 63) < 32)
				result |= (unsigned long long int)i1 << (j0 - 63);
		} else
		{
			uint32_t i = UC(0xffffffff) >> (j0 - 31);
			uint32_t j;
			
			switch (rm)
			{
			case FE_TOWARDZERO:
				break;
			case FE_DOWNWARD:
				if (sx && (i1 & i) != 0)
				{
					if (j0 == 31)
						i0 += 1;
					else
					{
						j = i1 + (UC(1) << (63 - j0));
						if (j < i1)
						{
							i0 += 1;		/* got a carry */
							if (i0 == 0)
							{
								i0 = UC(0x80000000);
								j0++;
							}
						}
						i1 = j;
					}
				}
				break;
			case FE_UPWARD:
				if (!sx && (i1 & i) != 0)
				{
					if (j0 == 31)
						i0 += 1;
					else
					{
						j = i1 + (UC(1) << (63 - j0));
						if (j < i1)
						{
							i0 += 1;		/* got a carry */
							if (i0 == 0)
							{
								i0 = UC(0x80000000);
								j0++;
							}
						}
						i1 = j;
					}
				}
				break;
			case FE_TONEAREST:
			default:
				if ((i1 & (i >> 1)) != 0 || (j0 == 32 && (i0 & 1)) || (i1 & (UC(1) << (63 - j0))))
				{
					j = i1 + (UC(1) << (62 - j0));
	
					if (j < i1)
					{
						uint32_t k = i0 + 1;
		
						if (k < i0)
						{
							j += j;
							j0++;
							k |= UC(0x80000000);
						}
						i0 = k;
					}
					i1 = j;
				}
				break;
			}
			i1 &= ~i;
			if (j0 == 31)
				result = (long long int) i0;
			else
				result = ((long long int) i0 << (j0 - 31)) | (i1 >> (63 - j0));
		}
	} else
	{
		/* The number is too large.  It is left implementation defined
		   what happens.  */
		feraiseexcept(FE_INVALID|FE_INEXACT);
		if (j0 == (IEEE854_LONG_DOUBLE_MAXEXP - IEEE854_LONG_DOUBLE_BIAS))
		{
			/* inf or nan */
			return LONG_LONG_MIN;
		}
		return (long long int) x;
	}

	return sx ? -result : result;
}

__typeof(__llrintl) llrintl __attribute__((weak, alias("__llrintl")));

#endif

#endif
