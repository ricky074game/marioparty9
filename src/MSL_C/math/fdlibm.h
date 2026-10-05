#ifndef MSL_MATH_FDLIBM_H
#define MSL_MATH_FDLIBM_H

/*
 * Minimal fdlibm.h for MSL_C's copy of Sun's freely distributable math
 * library (fdlibm 5.3, http://www.netlib.org/fdlibm/).
 *
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#define __STDC__
#define _IEEE_LIBM

/* PowerPC is big-endian: the high word comes first. */
#define __HI(x) *(int*)&x
#define __LO(x) *(1 + (int*)&x)
#define __HIp(x) *(int*)x
#define __LOp(x) *(1 + (int*)x)

#define EDOM 33
#define ERANGE 34

extern int errno;
extern int __float_nan[];
extern int __float_huge[];
#define NAN (*(float*)__float_nan)
#define HUGE_VAL (*(double*)__float_huge)

#define FP_NAN 1
#define FP_INFINITE 2
#define FP_ZERO 3
#define FP_NORMAL 4
#define FP_SUBNORMAL 5

int __fpclassifyd(double);
#define isfinite(x) (__fpclassifyd(x) > FP_INFINITE)

double __ieee754_acos(double);
double __ieee754_asin(double);
double __ieee754_atan2(double, double);
double __ieee754_fmod(double, double);
double __ieee754_log(double);
double __ieee754_log10(double);
double __ieee754_pow(double, double);
double __ieee754_sqrt(double);
int __ieee754_rem_pio2(double, double*);

double __kernel_sin(double, double, int);
double __kernel_cos(double, double);
double __kernel_tan(double, double, int);
int __kernel_rem_pio2(double*, double*, int, int, int, const int*);

double atan(double);
double ceil(double);
double copysign(double, double);
double cos(double);
#define fabs(x) __fabs(x)
double floor(double);
double frexp(double, int*);
double ldexp(double, int);
double modf(double, double*);
double scalbn(double, int);
double sin(double);
double sqrt(double);
double tan(double);

#endif
