/* `HUGE_VAL' constants for m68k (where it is infinity).
   Used by <stdlib.h> and <math.h> functions for overflow.
   Copyright (C) 1992, 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef _MATH_H
# error "Never use <bits/huge_val.h> directly; include <math.h> instead."
#endif

/* IEEE positive infinity (-HUGE_VAL is negative infinity).  */

#if __GNUC_PREREQ(3,3)
# define HUGE_VAL	(__builtin_huge_val())

#ifdef	 __USE_ISOC99
# define HUGE_VALF	(__builtin_huge_valf())
# define HUGE_VALL	(__builtin_huge_vall())
#endif

#elif __GNUC_PREREQ(2,96)
# define HUGE_VAL	(__extension__ 0x1.0p2047)

#ifdef	 __USE_ISOC99
# define HUGE_VALF	(__extension__ 0x1.0p255f)
# define HUGE_VALL (__extension__ 0x1.0p32767L)
#endif

#elif defined __GNUC__

# define HUGE_VAL					\
  (__extension__					\
   ((union { unsigned long long __l; double __d; })	\
    { __l: 0x7ff0000000000000ULL }).__d)

#ifdef	 __USE_ISOC99
#  define HUGE_VALF					\
  (__extension__					\
   ((union { unsigned long __l; float __f; })		\
    { __l: 0x7f800000UL }).__f)

# define HUGE_VALL	((long double) HUGE_VAL)

#endif

#else /* not GCC */

#if defined(__DOUBLE_64__)
static union { unsigned char __c[8]; double __d; } __huge_val =
  { { 0x7f, 0xf0, 0, 0, 0, 0, 0, 0 } };
#elif defined(__HAVE_68881__)
static union { unsigned char __c[12]; double __d; } __huge_val =
  { { 0x7f, 0xff, 0, 0, 0x80, 0, 0, 0, 0, 0, 0, 0 } };
#else
static union { unsigned char __c[10]; double __d; } __huge_val =
  { { 0x7f, 0xff, 0x80, 0, 0, 0, 0, 0, 0, 0 } };
#endif
#define HUGE_VAL	(__huge_val.__d)
/*
 * definition above is more accurrate, but cannot be
 * used in static initializations.
 */
#undef HUGE_VAL
#define HUGE_VAL 1.189731495357231765E+4932

#ifdef	 __USE_ISOC99
static union { unsigned char __c[4]; float __f; } __huge_valf =
  { { 0x7f, 0x80, 0, 0 } };
#  define HUGE_VALF	(__huge_valf.__f)

/*
 * definition above is more accurrate, but cannot be
 * used in static initializations.
 */
#undef HUGE_VALF
#define HUGE_VALF ((float)HUGE_VAL)

#if defined(__HAVE_68881__)
static union { unsigned char __c[12]; long double __ld; } __huge_vall =
  { { 0x7f, 0xff, 0, 0, 0x80, 0, 0, 0, 0, 0, 0, 0 } };
#else
static union { unsigned char __c[10]; long double __ld; } __huge_vall =
  { { 0x7f, 0xff, 0x80, 0, 0, 0, 0, 0, 0, 0 } };
#endif
#  define HUGE_VALL	(__huge_vall.__ld)

/*
 * definition above is more accurrate, but cannot be
 * used in static initializations.
 */
#undef HUGE_VALL
#define HUGE_VALL	((long double) HUGE_VAL)

#endif

#endif	/* GCC.  */
