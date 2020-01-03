/* Set current rounding direction.
   Copyright (C) 1997-2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Andreas Schwab <schwab@issan.informatik.uni-dortmund.de>

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif
#include "fpu_ctrl.h"

#ifdef __mc68000__

int __fesetround(int round)
{
	fpu_control_t fpcr;

	if ((round & ~FE_ROUNDING_MASK) != 0)
		/* ROUND is no valid rounding mode.  */
		return 1;

	_FPU_GETCW(fpcr);
	fpcr &= ~FE_ROUNDING_MASK;
	fpcr |= round;
	_FPU_SETCW(fpcr);

	return 0;
}

#endif

#if defined __i386__ || defined __x86_64__

int __fesetround(int round)
{
	fpu_control_t cw;

	if ((round & ~FE_ROUNDING_MASK) != 0)
		/* ROUND is no valid rounding mode.  */
		return 1;

	/* First set the x87 FPU.  */
	_FPU_GETCW(cw);
	cw &= ~FE_ROUNDING_MASK;
	cw |= round;
	_FPU_SETCW(cw);

	/* And now the MSCSR register for SSE, the precision is at different bit
	   positions in the different units, we need to shift it 3 bits.  */
	if (HAVE_SSE)
	{
		uint32_t xcw;

		__asm__("stmxcsr %0":"=m"(*&xcw));
		xcw &= ~(FE_ROUNDING_MASK << __MXCSR_ROUND_FLAG_SHIFT);
		xcw |= round << __MXCSR_ROUND_FLAG_SHIFT;
		__asm__("ldmxcsr %0": :"m"(*&xcw));
	}

	return 0;
}

#endif

__typeof(__fesetround) fesetround __attribute__((weak, alias("__fesetround")));
