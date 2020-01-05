#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __have_fpu_lrint

long int __ieee754_lrintf(float x)
{
	int32_t j0;
	uint32_t i0;
	long int result;
	int32_t sx;
	int rm;

	rm = fegetround();

	GET_FLOAT_WORD(i0, x);

	sx = i0 & IC(0x80000000);
	j0 = ((i0 >> IEEE754_FLOAT_SHIFT) & IEEE754_FLOAT_MAXEXP) - IEEE754_FLOAT_BIAS;
	i0 &= UC(0x7fffff);

	if (j0 < (int32_t) (sizeof(long int) * 8) - 1)
	{
		if (j0 == -IEEE754_FLOAT_BIAS)
			return 0;
		/* Raise inexact if x != 0.  */
		if (j0 < 0 || i0 != 0)
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
			if (j0 == -1 && i0 != 0)
				return sx ? -1 : 1;
			return 0;
		}

		if (j0 >= IEEE754_FLOAT_SHIFT)
		{
			i0 |= UC(0x800000);
			result = (long int) i0 << (j0 - IEEE754_FLOAT_SHIFT);
		} else
		{
			uint32_t i = UC(0x007fffff) >> j0;
			
			switch (rm)
			{
			case FE_TOWARDZERO:
				i0 |= UC(0x800000);
				break;
			case FE_DOWNWARD:
				i0 |= UC(0x800000);
				if (sx && (i0 & i) != 0)
					i0 += UC(0x00800000) >> j0;
				break;
			case FE_UPWARD:
				i0 |= UC(0x800000);
				if (!sx && (i0 & i) != 0)
					i0 += UC(0x00800000) >> j0;
				break;
			case FE_TONEAREST:
			default:
				i0 |= UC(0x800000);
				if ((i0 & (i >> 1)) != 0 || (i0 & (UC(0x00800000) >> j0)))
				{
					i0 += UC(0x00400000) >> j0;
				}
				break;
			}
			i0 &= (~i);
			result = i0 >> (IEEE754_FLOAT_SHIFT - j0);
		}
	} else
	{
		/* The number is too large.  It is left implementation defined
		   what happens.  */
		feraiseexcept(FE_INVALID|FE_INEXACT);
		if (j0 == (IEEE754_FLOAT_MAXEXP - IEEE754_FLOAT_BIAS))
		{
			/* inf or nan */
			return LONG_MIN;
		}
		return (long int) x;
	}

	return sx ? -result : result;
}

#endif

long int __lrintf(float x)
{
	return __ieee754_lrintf(x);
}

__typeof(__lrintf) lrintf __attribute__((weak, alias("__lrintf")));
