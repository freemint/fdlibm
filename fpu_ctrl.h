/* FPU control word bits.  x86-64 version.
   Copyright (C) 1993,1995,1996,1997,1998,2000,2001,2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Olaf Flebbe.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#ifndef ___FPU_CONTROL_H__
#define ___FPU_CONTROL_H__	1

#if defined(__i386__) || defined(__x86_64__)

/* _WIN32 does not have <fpu_control.h>, include our definitions */
#define _FPU_CONTROL_H
#include "../../include/bits/x86/fpu_control.h"

#else

#include <fpu_control.h>

#endif

/* Default control word set at startup.  */
extern fpu_control_t __fpu_control;

/* Called at startup.  It can be used to manipulate fpu control register.  */
void __setfpucw(fpu_control_t set);

#ifndef FE_ROUNDING_MASK
#define FE_ROUNDING_MASK (FE_TONEAREST|FE_DOWNWARD|FE_UPWARD|FE_TOWARDZERO)
#endif
#ifndef FE_PRECISION_MASK
#define FE_PRECISION_MASK (_FPU_EXTENDED|_FPU_DOUBLE|_FPU_SINGLE)
#endif

#endif	/* fpu_control.h */
