#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif

#ifndef __NO_LONG_DOUBLE_MATH

int __issignalingl(long double x)
{
	uint32_t exi, hxi, lxi;

	GET_LDOUBLE_WORDS(exi, hxi, lxi, x);
#ifdef HIGH_ORDER_BIT_IS_SET_FOR_SNAN
# error not implemented
#else
	/* To keep the following comparison simple, toggle the quiet/signaling bit,
	   so that it is set for sNaNs.  This is inverse to IEEE 754-2008 (as well as
	   common practice for IEEE 754-1985).  */
	hxi ^= UC(0x40000000);
	/* If lxi != 0, then set any suitable bit of the significand in hxi.  */
	hxi |= (lxi | -lxi) >> 31;
	/* We do not recognize a pseudo NaN as sNaN; they're invalid on 80387 and
	   later.  */
	/* We have to compare for greater (instead of greater or equal), because x's
	   significand being all-zero designates infinity not NaN.  */
	return ((exi & 0x7fff) == 0x7fff) && (hxi > UC(0xc0000000));
#endif
}

__typeof(__issignalingl) issignalingl __attribute__((weak, alias("__issignalingl")));

#endif
