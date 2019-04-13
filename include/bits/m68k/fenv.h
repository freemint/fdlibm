#ifndef _FENV_H
# error "Never use <bits/fenv.h> directly; include <fenv.h> instead."
#endif

/* Define bits representing the exception.  We use the bit positions of
   the appropriate bits in the FPSR Accrued Exception Byte.  */
enum
  {
    FE_INEXACT =	(1 << 3),
    FE_DIVBYZERO =  (1 << 4),
    FE_UNDERFLOW =  (1 << 5),
    FE_OVERFLOW =   (1 << 6),
    FE_INVALID =    (1 << 7)
  };

#define FE_INEXACT FE_INEXACT
#define FE_DIVBYZERO FE_DIVBYZERO
#define FE_UNDERFLOW FE_UNDERFLOW
#define FE_OVERFLOW FE_OVERFLOW
#define FE_INVALID FE_INVALID

#define FE_ALL_EXCEPT (FE_INEXACT | FE_DIVBYZERO | FE_UNDERFLOW | FE_OVERFLOW | FE_INVALID)

/* rounding precision used by fegetprec() and fesetprec() */
#ifdef __mcoldfire__
#define FE_LDBLPREC 0x00 /* long double precision */
#define FE_DBLPREC  0x00 /* double precision */
#else
#define FE_LDBLPREC 0x00 /* long double precision */
#define FE_DBLPREC  0x80 /* double precision */
#endif
#define FE_FLTPREC  0x40 /* float precision */
#define FE_PRECISION_MASK (FE_LDBLPREC|FE_FLTPREC|FE_DBLPREC)

/* The m68k FPU supports all of the four defined rounding modes.  We use
   the bit positions in the FPCR Mode Control Byte as the values for the
   appropriate macros.  */
enum
  {
    FE_TONEAREST = 0,
    FE_TOWARDZERO = (1 << 4),
    FE_DOWNWARD =   (2 << 4),
    FE_UPWARD =     (3 << 4)
  };

#define FE_TONEAREST FE_TONEAREST
#define FE_TOWARDZERO FE_TOWARDZERO
#define FE_DOWNWARD FE_DOWNWARD
#define FE_UPWARD FE_UPWARD

/* Type representing exception flags.  */
typedef __uint32_t fexcept_t;


/* Type representing floating-point environment.  This structure
   corresponds to the layout of the block written by `fmovem'.  */
typedef struct
  {
    __uint32_t __control_register;
    __uint32_t __status_register;
    __uint32_t __instruction_address;
  }
fenv_t;

/* If the default argument is used we use this value.  */
#define FE_DFL_ENV	((const fenv_t *) -1)

#ifdef __USE_GNU
/* Floating-point environment where none of the exceptions are masked.  */
# define FE_NOMASK_ENV	((const fenv_t *) -2)
#endif

extern fexcept_t __fpu_status;
