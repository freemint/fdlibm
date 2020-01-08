/* Raise given exceptions.
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

#ifdef __mcoldfire__

static void raise_one_exception(fexcept_t excepts, fexcept_t mask)
{
	if (excepts & mask)
	{
		fexcept_t fpsr;
		double unused;

		_FPU_GETSR(fpsr);
		fpsr |= (mask << 6) | mask;
		_FPU_SETSR(fpsr);
		__asm__ __volatile__ ("fmove%.l (%%sp),%0":"=f" (unused));
	}
}

int __feraiseexcept(int excepts)
{
	/* Raise exceptions represented by EXCEPTS.  But we must raise only one
	   signal at a time.  It is important that if the overflow/underflow
	   exception and the divide by zero exception are given at the same
	   time, the overflow/underflow exception follows the divide by zero
	   exception.

	   The Coldfire FPU allows an exception to be raised by asserting
	   the associated EXC bit and then executing an arbitrary arithmetic
	   instruction.  fmove.l is classified as an arithmetic instruction
	   and suffices for this purpose.

	   We therefore raise an exception by setting both the EXC and AEXC
	   bit associated with the exception (the former being 6 bits to the
	   left of the latter) and then loading the longword at (%sp) into an
	   FP register.  */

#ifdef __HAVE_FPU__
	raise_one_exception(excepts, FE_INVALID);
	raise_one_exception(excepts, FE_DIVBYZERO);
	raise_one_exception(excepts, FE_OVERFLOW);
	raise_one_exception(excepts, FE_UNDERFLOW);
	raise_one_exception(excepts, FE_INEXACT);
#else
	__fpu_status |= excepts & FE_ALL_EXCEPT;
#endif

	/* Success.  */
	return 0;
}

#else

int __feraiseexcept(int excepts)
{
#ifdef __HAVE_FPU__
	/* Raise exceptions represented by EXCEPTS.  But we must raise only one
	   signal at a time.  It is important that if the overflow/underflow
	   exception and the divide by zero exception are given at the same
	   time, the overflow/underflow exception follows the divide by zero
	   exception.  */

	/* First: invalid exception.  */
	if (excepts & FE_INVALID)
	{
		/* One example of a invalid operation is 0 * Infinity.  */
		double d = HUGE_VAL;

		__asm__ __volatile__("fmul%.s %#0r0,%0; fnop":"=f"(d):"0"(d));
	}

	/* Next: division by zero.  */
	if (excepts & FE_DIVBYZERO)
	{
		double d = 1.0;

		__asm__ __volatile__("fdiv%.s %#0r0,%0; fnop":"=f"(d):"0"(d));
	}

	/* Next: overflow.  */
	if (excepts & FE_OVERFLOW)
	{
		long double d = LDBL_MAX;

		__asm__ __volatile__("fmul%.x %0,%0; fnop":"=f"(d):"0"(d));
	}

	/* Next: underflow.  */
	if (excepts & FE_UNDERFLOW)
	{
		long double d = -LDBL_MAX;

		__asm__ __volatile__("fetox%.x %0; fnop":"=f"(d):"0"(d));
	}

	/* Last: inexact.  */
	if (excepts & FE_INEXACT)
	{
		long double d = 1.0;

		__asm__ __volatile__("fdiv%.s %#0r3,%0; fnop":"=f"(d):"0"(d));
	}
#else
	__fpu_status |= excepts & FE_ALL_EXCEPT;
#endif

	/* Success.  */
	return 0;
}

#endif

__typeof(__feraiseexcept) feraiseexcept __attribute__((weak, alias("__feraiseexcept")));
