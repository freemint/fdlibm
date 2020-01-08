/* Enable floating-point exceptions.
   Copyright (C) 2000-2013 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Andreas Schwab <schwab@suse.de>, 2000.

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

int __feenableexcept(int excepts)
{
	fpu_control_t new_exc, old_exc;

	/* Get the current control register contents.  */
	_FPU_GETCW(new_exc);

	old_exc = (new_exc >> 6) & FE_ALL_EXCEPT;

	excepts &= FE_ALL_EXCEPT;

	new_exc |= excepts << 6;
	_FPU_SETCW(new_exc);

	return (int)old_exc;
}

__typeof(__feenableexcept) feenableexcept __attribute__((weak, alias("__feenableexcept")));
