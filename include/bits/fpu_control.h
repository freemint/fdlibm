#ifndef _FPU_CONTROL_H
# error "Never use <bits/fpu_control.h> directly; include <fpu_control.h> instead."
#endif

#ifdef __mc68000__
#include <bits/m68k/fpu_control.h>
#endif

#if defined(__i386__) || defined(__x86_64__)
#include <bits/x86/fpu_control.h>
#endif

#ifndef _FPU_GETCW
#error "<fpu_control.h> not supported"
#endif
