#ifndef _FENV_H
# error "Never use <bits/fenv.h> directly; include <fenv.h> instead."
#endif

/* Define bits representing the exception.  We use the bit positions
   of the appropriate bits in the FPU control word.  */
enum
  {
    FE_INVALID = 0x01,
    FE_DENORMAL = 0x02,
    FE_DIVBYZERO = 0x04,
    FE_OVERFLOW = 0x08,
    FE_UNDERFLOW = 0x10,
    FE_INEXACT = 0x20
  };

#define FE_INVALID	FE_INVALID
#define FE_DIVBYZERO	FE_DIVBYZERO
#define FE_OVERFLOW	FE_OVERFLOW
#define FE_UNDERFLOW	FE_UNDERFLOW
#define FE_INEXACT	FE_INEXACT
#define __FE_DENORM FE_DENORMAL

#define FE_ALL_EXCEPT (FE_INEXACT | FE_DIVBYZERO | FE_UNDERFLOW | FE_OVERFLOW | FE_INVALID)

/* rounding precision used by fegetprec() and fesetprec() */
#define FE_LDBLPREC 0x300 /* long double precision */
#define FE_FLTPREC  0x000 /* float precision */
#define FE_DBLPREC  0x200 /* double precision */
#define FE_PRECISION_MASK (FE_LDBLPREC|FE_FLTPREC|FE_DBLPREC)

/* The ix87 FPU supports all of the four defined rounding modes.  We
   use again the bit positions in the FPU control word as the values
   for the appropriate macros.  */
enum
  {
    FE_TONEAREST = 0,
    FE_DOWNWARD = 0x400,
    FE_UPWARD = 0x800,
    FE_TOWARDZERO = 0xc00
  };

#define FE_TONEAREST	FE_TONEAREST
#define FE_DOWNWARD	FE_DOWNWARD
#define FE_UPWARD	FE_UPWARD
#define FE_TOWARDZERO	FE_TOWARDZERO

/* Type representing exception flags.  */
typedef unsigned short int fexcept_t;


/* Type representing floating-point environment.  This structure
   corresponds to the layout of the block written by the `fstenv'
   instruction and has additional fields for the contents of the MXCSR
   register as written by the `stmxcsr' instruction.  */
typedef struct
  {
    unsigned short int __control_word;
    unsigned short int __unused1;
    unsigned short int __status_word;
    unsigned short int __unused2;
    unsigned short int __tags;
    unsigned short int __unused3;
    unsigned int __eip;
    unsigned short int __cs_selector;
    unsigned int __opcode:11;
    unsigned int __unused4:5;
    unsigned int __data_offset;
    unsigned short int __data_selector;
    unsigned short int __unused5;
#if defined(__x86_64__) || defined(_WIN32)
    unsigned int __mxcsr;
#endif
  }
fenv_t;

/* If the default argument is used we use this value.  */
#ifdef _WIN32
#define FE_DFL_ENV ((const fenv_t *) 0)
#else
#define FE_DFL_ENV	((__const fenv_t *) -1)
#endif

#ifdef __USE_GNU
/* Floating-point environment where none of the exception is masked.  */
# define FE_NOMASK_ENV	((__const fenv_t *) -2)
#endif

/* The default Intel x87 floating point environment (64-bit mantissa) */
#define FE_PC64_ENV ((const fenv_t *)-1)

/* The floating point environment set by MSVCRT _fpreset (53-bit mantissa) */
#define FE_PC53_ENV ((const fenv_t *)-2)

/* The MXCSR exception flags are the same as the
   FE flags. */
#define __MXCSR_EXCEPT_FLAG_SHIFT 0

/* How much to shift FE status word exception flags
   to get the MXCSR exeptions masks,  */
#define __MXCSR_EXCEPT_MASK_SHIFT 7

/* How much to shift FE control word rounding flags
   to get MXCSR rounding flags,  */
#define __MXCSR_ROUND_FLAG_SHIFT 3
