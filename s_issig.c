#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#undef issignaling

int __issignaling(double x)
{
#ifdef HIGH_ORDER_BIT_IS_SET_FOR_SNAN
	uint32_t hxi;

	GET_HIGH_WORD(hxi, x);
	/* We only have to care about the high-order bit of x's significand, because
	   having it set (sNaN) already makes the significand different from that
	   used to designate infinity.  */
	return (hxi & UC(0x7ff80000)) == UC(0x7ff80000);
#else
	uint32_t hxi, lxi;

	GET_DOUBLE_WORDS(hxi, lxi, x);
	/* To keep the following comparison simple, toggle the quiet/signaling bit,
	   so that it is set for sNaNs.  This is inverse to IEEE 754-2008 (as well as
	   common practice for IEEE 754-1985).  */
	hxi ^= UC(0x00080000);
	/* If lxi != 0, then set any suitable bit of the significand in hxi.  */
	hxi |= (lxi | -lxi) >> 31;
	/* We have to compare for greater (instead of greater or equal), because x's
	   significand being all-zero designates infinity not NaN.  */
	return (hxi & UC(0x7fffffff)) > UC(0x7ff80000);
#endif
}

__typeof(__issignaling) issignaling __attribute__((weak, alias("__issignaling")));
#ifdef __NO_LONG_DOUBLE_MATH
int __issignalingl(long double x) __attribute__((alias("__issignaling")));
__typeof(__issignalingl) issignalingl __attribute__((weak, alias("__issignaling")));
#endif
