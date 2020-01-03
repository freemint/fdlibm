#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

int __issignalingf(float x)
{
	uint32_t xi;

	GET_FLOAT_WORD(xi, x);
#ifdef HIGH_ORDER_BIT_IS_SET_FOR_SNAN
	/* We only have to care about the high-order bit of x's significand, because
	   having it set (sNaN) already makes the significand different from that
	   used to designate infinity.  */
	return (xi & UC(0x7fc00000)) == UC(0x7fc00000);
#else
	/* To keep the following comparison simple, toggle the quiet/signaling bit,
	   so that it is set for sNaNs.  This is inverse to IEEE 754-2008 (as well as
	   common practice for IEEE 754-1985).  */
	xi ^= UC(0x00400000);
	/* We have to compare for greater (instead of greater or equal), because x's
	   significand being all-zero designates infinity not NaN.  */
	return (xi & UC(0x7fffffff)) > UC(0x7fc00000);
#endif
}

__typeof(__issignalingf) issignalingf __attribute__((weak, alias("__issignalingf")));
