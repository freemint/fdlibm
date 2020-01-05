/* Store current floating-point environment.
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

int __fegetenv(fenv_t *envp)
{
#if defined(__GNUC__) && defined(__HAVE_68881__) && !defined(__mcoldfire__)
	 __asm__("fmovem%.l %/fpcr/%/fpsr/%/fpiar,%0":"=m"(*envp));
#else
	_FPU_GETCW(envp->__control_register);
	_FPU_GETSR(envp->__status_register);
	_FPU_GETIAR(envp->__instruction_address);
#endif

	/* Success.  */
	return 0;
}

__typeof(__fegetenv) fegetenv __attribute__((weak, alias("__fegetenv")));
