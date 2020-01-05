/* Install given floating-point environment.
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

int __fesetenv(const fenv_t *envp)
{
	fenv_t temp;

	/* Install the environment specified by ENVP.  But there are a few
	   values which we do not want to come from the saved environment.
	   Therefore, we get the current environment and replace the values
	   we want to use from the environment specified by the parameter.  */
	fegetenv(&temp);

	temp.__status_register &= ~FE_ALL_EXCEPT;
	temp.__control_register &= ~((FE_ALL_EXCEPT << 6) | FE_ROUNDING_MASK | FE_PRECISION_MASK);
	if (envp == FE_DFL_ENV)
	{
		temp.__control_register |= FE_TONEAREST | _FPU_EXTENDED;
	} else if (envp == FE_NOMASK_ENV)
	{
		temp.__control_register |= FE_ALL_EXCEPT << 6;
		temp.__control_register |= FE_TONEAREST | _FPU_EXTENDED;
	} else
	{
		temp.__control_register |= (envp->__control_register & ((FE_ALL_EXCEPT << 6) | FE_ROUNDING_MASK | FE_PRECISION_MASK));
		temp.__status_register |= envp->__status_register & FE_ALL_EXCEPT;
	}

	_FPU_SETCW(temp.__control_register);
	_FPU_SETSR(temp.__status_register);

	/* Success.  */
	return 0;
}

__typeof(__fesetenv) fesetenv __attribute__((weak, alias("__fesetenv")));
