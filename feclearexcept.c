/* Clear given exceptions in current floating-point environment.
   Copyright (C) 1997-2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1997.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef __FDLIBM_H__
#include "fdlibm.h"
#endif
#include "fpu_ctrl.h"

int __feclearexcept(int excepts)
{
	fexcept_t fpsr;

	/* Mask out unsupported bits/exceptions. */
	excepts &= FE_ALL_EXCEPT;

	/* Fetch the fpu status register. */
	_FPU_GETSR(fpsr);

	/* Clear the relevant bits. */
	fpsr &= ~excepts;

	/* Put the new data in effect. */
	_FPU_SETSR(fpsr);

	/* Success. */
	return 0;
}

__typeof(__feclearexcept) feclearexcept __attribute__((weak, alias("__feclearexcept")));
