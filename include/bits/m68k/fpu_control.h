#ifndef _FPU_CONTROL_H
# error "Never use <bits/fpu_control.h> directly; include <fpu_control.h> instead."
#endif

/*
 * Motorola floating point control register bits.
 *
 * 31-16  -> reserved (read as 0, ignored on write)
 * 15     -> enable trap for BSUN exception
 * 14     -> enable trap for SNAN exception
 * 13     -> enable trap for OPERR exception
 * 12     -> enable trap for OVFL exception
 * 11     -> enable trap for UNFL exception
 * 10     -> enable trap for DZ exception
 *  9     -> enable trap for INEX2 exception (INEX on Coldfire)
 *  8     -> enable trap for INEX1 exception (IDE on Coldfire)
 *  7-6   -> Precision Control (only bit 6 is used on Coldfire)
 *  5-4   -> Rounding Control
 *  3-0   -> zero (read as 0, write as 0)
 *
 *
 * Precision Control:
 * 00 - round to extended precision
 * 01 - round to single precision
 * 10 - round to double precision
 * 11 - undefined
 *
 * Rounding Control:
 * 00 - rounding to nearest (RN)
 * 01 - rounding toward zero (RZ)
 * 10 - rounding (down)toward minus infinity (RM)
 * 11 - rounding (up) toward plus infinity (RP)
 *
 * The hardware default is 0x0000. I choose 0x5400.
 */

/* masking of interrupts */
#define _FPU_MASK_BSUN  0x8000
#define _FPU_MASK_SNAN  0x4000
#define _FPU_MASK_OPERR 0x2000
#define _FPU_MASK_OVFL  0x1000
#define _FPU_MASK_UNFL  0x0800
#define _FPU_MASK_DZ    0x0400
#define _FPU_MASK_INEX1 0x0200
#define _FPU_MASK_INEX2 0x0100

/* precision control */
#ifdef __mcoldfire__
# define _FPU_EXTENDED 0x00
# define _FPU_DOUBLE   0x00
#else
# define _FPU_EXTENDED 0x00   /* RECOMMENDED */
# define _FPU_DOUBLE   0x80
#endif
#define _FPU_SINGLE   0x40     /* DO NOT USE */

/* rounding control */
#define _FPU_RC_NEAREST 0x00    /* RECOMMENDED */
#define _FPU_RC_ZERO    0x10
#define _FPU_RC_DOWN    0x20
#define _FPU_RC_UP      0x30

#ifdef __mcoldfire__
# define _FPU_RESERVED 0xFFFF800FL
#else
# define _FPU_RESERVED 0xFFFF000FL  /* Reserved bits in fpucr */
#endif


/* Now two recommended fpucr */

/* The fdlibm code requires no interrupts for exceptions.  Don't
   change the rounding mode, it would break long double I/O!  */
#define _FPU_DEFAULT  0x00000000

/* IEEE:  same as above, but exceptions.  We must make it non-zero so
   that __setfpucw works.  This bit will be ignored.  */
#define _FPU_IEEE     0x00000001

/* Type of the control word.  */
typedef __uint32_t fpu_control_t;

/* Macros for accessing the hardware control word.  */
#if defined(__GNUC__) && ((defined(__mcoldfire__) && defined (__mcffpu__)) || defined(__HAVE_68881__))
# define _FPU_GETCW(cw) __asm__ ("fmove%.l %!, %0" : "=dm" (cw))
# define _FPU_SETCW(cw) __asm__ volatile ("fmove%.l %0, %!" : : "dm" (cw))
# define _FPU_GETSR(sr) __asm__ ("fmove%.l %/fpsr,%0" : "=dm" (sr))
# define _FPU_SETSR(sr) __asm__ __volatile__ ("fmove%.l %0,%/fpsr" : : "dm" (sr))
# define _FPU_GETIAR(addr) __asm__ ("fmove%.l %/fpiar,%0" : "=dm" (addr));
#elif (defined(__AHCC__) || defined(__PUREC__) || defined(__TURBOC__)) && ((defined(__mcoldfire__) && defined (__mcffpu__)) || defined(__HAVE_68881__))
# define _FPU_GETCW(cw) cw = __FGETCONTROL__(4)
# define _FPU_SETCW(cw) __FSETCONTROL__(4, cw)
# define _FPU_GETSR(sr) sr = __FGETCONTROL__(2)
# define _FPU_SETSR(sr) __FSETCONTROL__(2, sr)
# define _FPU_GETIAR(addr) addr = __FGETCONTROL__(1)
#elif defined(__VBCC__) && (defined (__mcffpu__) || defined(__M68881) || defined(__M68882))
static fpu_control_t __asm_fpu_getcw(void)="\tfmove.l\tfpcr,d0\n";
# define _FPU_GETCW(cw) cw = __asm_fpu_getcw()
static void __asm_fpu_setcw(__reg("d0")fpu_control_t)="\tfmove.l\td0,fpcr\n";
# define _FPU_SETCW(cw) __asm_fpu_setcw(cw)
static fpu_control_t __asm_fpu_getsr(void)="\tfmove.l\tfpsr,d0\n";
# define _FPU_GETSR(sr) sr = __asm_fpu_getsr()
static void __asm_fpu_setsr(__reg("d0")fpu_control_t)="\tfmove.l\td0,fpsr\n";
# define _FPU_SETSR(sr) __asm_fpu_setsr(sr)
static fpu_control_t __asm_fpu_getiar(void)="\tfmove.l\tfpiar,d0\n";
# define _FPU_GETIAR(addr) addr = __asm_fpu_getiar()
#endif

/* Default control word set at startup.  */
extern fpu_control_t __fpu_control;

#ifndef _FPU_GETCW
# define _FPU_GETCW(cw) cw = __fpu_control
# define _FPU_SETCW(cw) __fpu_control = cw
# define _FPU_GETSR(sr) sr = __fpu_status
# define _FPU_SETSR(sr) __fpu_status = sr
# define _FPU_GETIAR(addr) addr = 0
#endif
