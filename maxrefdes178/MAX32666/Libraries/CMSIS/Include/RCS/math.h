// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __math_h
#define __math_h

#define FP_ZERO      0x00
#define FP_SUBNORMAL 0x01
#define FP_NORMAL    0x02
#define FP_INFINITE  0x03
#define FP_NAN       0x04

#ifndef FP_ILOGB0
# define FP_ILOGB0 (-INT_MAX)
#endif
#ifndef FP_ILOGBNAN
# define FP_ILOGBNAN INT_MAX
#endif

#define M_E		2.7182818284590452354
#define M_LOG2E		1.4426950408889634074
#define M_LOG10E	0.43429448190325182765
#define M_LN2		0.693147180559945309417
#define M_LN10		2.30258509299404568402
#define M_PI		3.14159265358979323846
#define M_PI_2		1.57079632679489661923
#define M_PI_4		0.78539816339744830962
#define M_1_PI		0.31830988618379067154
#define M_2_PI		0.63661977236758134308
#define M_2_SQRTPI	1.12837916709551257390
#define M_SQRT2		1.41421356237309504880
#define M_SQRT1_2	0.70710678118654752440

#ifndef __ASSEMBLY_INCLUDE

#ifdef __cplusplus
extern "C" {
#endif

// These are stored in memory; don't ask, it's hard to do this stuff in a portable manner.
#if defined(__CROSSWORKS_AVR) || defined(__CROSSWORKS_MAXQ) || defined(__CROSSWORKS_MAXQ30) 

extern __code const float __float32_infinity;
extern __code const double __float64_infinity;
extern __code const float __float32_nan;
extern __code const double __float64_nan;

#else

extern const float __float32_infinity;
extern const double __float64_infinity;
extern const float __float32_nan;
extern const double __float64_nan;

#endif

#ifndef __NO_PRIVATE_PROTOTYPES
/* Private functions. */
int __float32_isinf(float);
int __float64_isinf(double);
int __float32_isnan(float);
int __float64_isnan(double);
int __float32_isfinite(float);
int __float64_isfinite(double);
int __float32_isnormal(float);
int __float64_isnormal(double);
int __float32_signbit(float);
int __float64_signbit(double);
int __float32_classify(float);
int __float64_classify(double);
#endif

#if defined(__CROSSWORKS_AVR) || defined(__CROSSWORKS_MSP430) || defined(__CROSSWORKS_MAXQ) || defined(__CROSSWORKS_MAXQ30) 

#define HUGE_VAL  __maxof(double)
#define HUGE_VALF __maxof(float)
#define INFINITY  __infinityof(float)
#define NAN       __float32_nan

#else

#define INFINITY  __builtin_huge_val()
#define NAN       __builtin_nanf("0x7fc00000")
#define HUGE_VAL  __builtin_huge_val()
#define HUGE_VALF __builtin_huge_valf()

#endif

#ifdef __cplusplus
extern "C++" {
inline bool isinf(float __x) { return __float32_isinf(__x) == 1; }
inline bool isinf(double __x) { return __float64_isinf(__x) == 1; }
inline bool isnan(float __x) { return __float32_isnan(__x) == 1; }
inline bool isnan(double __x) { return __float64_isnan(__x) == 1; }
inline bool isfinite(float __x) { return __float32_isfinite(__x) == 1; }
inline bool isfinite(double __x) { return __float64_isfinite(__x) == 1; }
inline bool isnormal(float __x) { return __float32_isnormal(__x) == 1; }
inline bool isnormal(double __x) { return __float64_isnormal(__x) == 1; }
inline bool signbit(float __x) { return __float32_signbit(__x) != 0; }
inline bool signbit(double __x) { return __float64_signbit(__x) != 0; }
inline int fpclassify(float __x) { return __float32_classify(__x); }
inline int fpclassify(double __x) { return __float64_classify(__x); }
inline bool isunordered(float __a, float __b) { return fpclassify(__a) == FP_NAN || fpclassify(__b) == FP_NAN; }
inline bool isunordered(double __a, double __b) { return fpclassify(__a) == FP_NAN || fpclassify(__b) == FP_NAN; }
inline bool isgreater(float __x, float __y) { return !isunordered(__x, __y) && (__x > __y); }
inline bool isgreater(double __x, double __y) { return !isunordered(__x, __y) && (__x > __y); }
inline bool isgreaterequal(float __x, float __y) { return !isunordered(__x, __y) && (__x >= __y); }
inline bool isgreaterequal(double __x, double __y) { return !isunordered(__x, __y) && (__x >= __y); }
inline bool isless(float __x, float __y) { return !isunordered(__x, __y) && (__x < __y); }
inline bool isless(double __x, double __y) { return !isunordered(__x, __y) && (__x < __y); }
inline bool islessequal(float __x, float __y) { return !isunordered(__x, __y) && (__x <= __y); }
inline bool islessequal(double __x, double __y) { return !isunordered(__x, __y) && (__x <= __y); }
inline bool islessgreater(float __x, float __y) { return !isunordered(__x, __y) && (__x < __y || __x > __y); }
inline bool islessgreater(double __x, double __y) { return !isunordered(__x, __y) && (__x < __y || __x > __y); }
}
#else

#define __is_float32(x) \
  /*lint -save -r506*/ (sizeof(x) == sizeof(float)) /*lint -restore*/

/*! \brief Test for infinity \ingroup Classification Macros \synopsis

  \description \b \this determines whether \a x is an infinity 
  (positive or negative). The determination is based on the type of the argument.
*/
#define isinf(x) (sizeof(x) == sizeof(float) ? __float32_isinf(x) : __float64_isinf(x))


/*! \brief Test for NaN \ingroup Classification Macros \synopsis

  \description \b \this determines whether \a x is a NaN.
  The determination is based on the type of the argument.
*/
#define isnan(x) (sizeof(x) == sizeof(float) ? __float32_isnan(x) : __float64_isnan(x))


/*! \brief Test for a finite value \ingroup Classification Macros \synopsis

  \description \b \this determines whether \a x is a finite 
  value (zero, subnormal, or normal, and not infinite or NaN). \b \this 
  returns a non-zero value if and only if \a x has a finite value.
*/
#define isfinite(x) (sizeof(x) == sizeof(float) ? __float32_isfinite(x) : __float64_isfinite(x))


/*! \brief Test for a normal value \ingroup Classification Macros \synopsis

  \description \b \this determines whether \a x is a normal
  value (zero, subnormal, or normal, and not infinite or NaN).. \b \this
  returns a non-zero value if and only if \a x has a normal value.
*/
#define isnormal(x) (sizeof(x) == sizeof(float) ? __float32_isnormal(x) : __float64_isnormal(x))


/*! \brief Test sign \ingroup Classification Macros \synopsis

  \description \this macro determines whether the sign of \a x is negative. 
  \b \this returns a non-zero value if and only if \a x is negative.
*/
#define signbit(x) (sizeof(x) == sizeof(float) ? __float32_signbit(x) : __float64_signbit(x))


/*! \brief Classify floating type \ingroup Classification Macros \synopsis

  \desc \b \this classifies \a x as NaN, infinite, normal,
  subnormal, zero, or into another implementation-defined category.
  \b \this returns one of:

  \item \b FP_ZERO
  \item \b FP_SUBNORMAL
  \item \b FP_NORMAL
  \item \b FP_INFINITE
  \item \b FP_NAN
*/
#define fpclassify(x) (__is_float32(x) ? __float32_classify(x) : __float64_classify(x))

/*! \brief Is greater \ingroup Comparison Macros \synopsis

  \desc \b \this returns whether \a x is greater than \a y.
*/
#define isgreater(x,y) (!isunordered(x, y) && (x > y))

/*! \brief Is greater or equal \ingroup Comparison Macros \synopsis

  \desc \b \this returns whether \a x is greater than or equal to \a y.
*/
#define isgreaterequal(x,y) (!isunordered(x, y) && (x >= y))

/*! \brief Is less \ingroup Comparison Macros \synopsis

  \desc \b \this returns whether \a x is less than \a y.
*/
#define isless(x,y) (!isunordered(x, y) && (x < y))

/*! \brief Is less or equal \ingroup Comparison Macros \synopsis

  \desc \b \this returns whether \a x is less than or equal to \a y.
*/
#define islessequal(x,y) (!isunordered(x, y) && (x <= y))

/*! \brief Is less or greater \ingroup Comparison Macros \synopsis

  \desc \b \this returns whether \a x is less than or greater than \a y.
*/
#define islessgreater(x,y) (!isunordered(x, y) && (x < y || x > y))

/*! \brief Is unordered \ingroup Comparison Macros \synopsis

  \desc \b \this returns whether \a x or \a y are unordered values.
*/
#define isunordered(a,b) (fpclassify(a) == FP_NAN || fpclassify(b) == FP_NAN)

#endif

// This is simple.
typedef float float_t;
typedef double double_t;


/*! \brief Compute cosine of a float \ingroup Trigonometric functions \synopsis

  \desc \b \this returns the radian circular cosine of x. 

  If |\a x| \gt 10^9, \b errno is set to \b EDOM and \b \this returns \b HUGE_VALF. 

  If \a x is NaN, \b \this returns \a x.
  If |\a x| is \inf, \b \this returns NaN. 
*/
float cosf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float cos(float __x);
double cos(double __x);
}
extern "C" {
#else
/*! \brief Compute cosine of a double \ingroup Trigonometric functions \synopsis
 
  \desc \b \this returns the radian circular cosine of \a x.

  If |\a x| \gt 10^9, \b errno is set to \b EDOM and \b \this returns \b HUGE_VAL. 

  If \a x is NaN, \b \this returns \a x.
  If |\a x| is \inf, \b \this returns NaN. 
*/
double cos(double __x);
#endif

/*! \brief Compute sine of a float \ingroup Trigonometric functions \synopsis

  \desc \b \this returns the radian circular sine of \a x. 

  If |\a x| \gt 10^9, \b errno is set to \b EDOM and \b \this returns \b HUGE_VALF. 

  \b \this returns \a x if \a x is NaN.
  \b \this returns NaN if |\a x| is \inf. 
*/
float sinf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float sin(float __x);
double sin(double __x);
}
extern "C" {
#else
/*! \brief Compute sine of a double \ingroup Trigonometric functions \synopsis

  \desc \b \this returns the radian circular sine of \a x. 

  If |\a x| \gt 10^9, \b errno is set to \b EDOM and \b \this returns \b HUGE_VAL. 

  \b \this returns \a x if \a x is NaN.
  \b \this returns NaN if |\a x| is \inf. 
*/
double sin(double __x);
#endif


/*! \brief Compute tangent of a double \ingroup Trigonometric functions \synopsis

  \desc \b \this returns the radian circular tangent of \a x. 

  If |\a x| \gt 10^9, \b errno is set to \b EDOM and \b \this returns \b HUGE_VALF. 

  If \a x is NaN, \this returns \a x.
  If |\a x| is \inf, \this returns NaN. 
*/
float tanf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float tan(float __x);
double tan(double __x);
}
extern "C" {
#else
/*! \brief Compute tangent of a double \ingroup Trigonometric functions \synopsis

  \desc \b \this returns the radian circular tangent of \a x. 

  If |\a x| \gt 10^9, \b errno is set to \b EDOM and \b \this returns \b HUGE_VAL.

  If \a x is NaN, \b \this returns \a x.
  If |\a x| is \inf, \b \this returns NaN. 
*/
double tan(double __x);
#endif


/*! \brief Compute inverse cosine of a float \ingroup Inverse trigonometric functions \synopsis

  \desc \b \this returns the principal value, in radians, of the inverse circular 
  cosine of \a x. The principal value lies in the interval [0, PI] radians. 

  If |\b a|  1, \b errno is set to \b EDOM and \b \this returns \b HUGE_VAL.

  If \a x is NaN, \b \this returns \a x.
  If |\a x| \gt 1, \b \this returns NaN.
*/
float acosf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float acos(float __x);
double acos(double __x);
}
extern "C" {
#else
/*! \brief Compute inverse cosine of a double \ingroup Inverse trigonometric functions \synopsis

  \desc \b \this returns the principal value, in radians, of the inverse circular 
  cosine of \a x. The principal value lies in the interval [0, PI] radians. 

  If |\a x| > 1, \b errno is set to \b EDOM and \b \this returns \b HUGE_VAL. 

  If \a x is NaN, \b \this returns \a x.
  If |\a x| \gt 1, \b \this returns NaN. 
*/
double acos(double __x);
#endif


/*! \brief Compute inverse sine of a float \ingroup Inverse trigonometric functions \synopsis

  \desc \b \this returns the principal value, in radians, of the inverse circular 
  sine of \b val. The principal value lies in the interval [\minus\half\pi, +\half\pi] 
  radians.

  If |\a x| \gt 1, \b errno is set to \b EDOM and \b \this returns \b HUGE_VALF. 

  If \a x is NaN, \b \this returns \a x.
  If |\a x| \gt 1, \b \this returns NaN. 
*/
float asinf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float asin(float __x);
double asin(double __x);
}
extern "C" {
#else
/*! \brief Compute inverse sine of a double \ingroup Inverse trigonometric functions \synopsis

  \desc \b \this returns the principal value, in radians, of the inverse circular
  sine of \a x. The principal value lies in the interval [\minus\half\pi, +\half\pi]
  radians. 

  If |\a x| \gt 1, \b errno is set to \b EDOM and \b \this returns \b HUGE_VAL. 

  If \a x is NaN, \b \this returns \a x.
  If |\a x| \gt 1, \b \this returns NaN. 
*/
double asin(double __x);
#endif


/*! \brief Compute inverse tangent of a float \ingroup Inverse trigonometric functions \synopsis
 
  \desc \b \this returns the principal value, in radians, of the inverse circular 
  tangent of \a x. The principal value lies in the interval [\minus\half\pi, +\half\pi] 
  radians. 
*/
float atanf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float atan(float __x);
double atan(double __x);
}
extern "C" {
#else
/*! \brief Compute inverse tangent of a double \ingroup Inverse trigonometric functions \synopsis

  \desc \b \this returns the principal value, in radians, of the inverse circular 
  tangent of \a x. The principal value lies in the interval [\minus\half\pi, +\half\pi] radians. 
*/
double atan(double __x);
#endif


/*! \brief Compute inverse tangent of a ratio of floats \ingroup Inverse trigonometric functions \synopsis

  \desc \b \this returns the value, in radians, of the inverse circular tangent 
  of \b y divided by \a x using the signs of \a x and \b y to 
  compute the quadrant of the return value. The principal value lies in the interval 
  [\minus\pi, +\pi] radians. 

  If \a x = \b y = 0, \b errno is set to \b EDOM and \b \this returns \b HUGE_VALF.

  \b \this(\a x, NaN) is NaN. \n
  \b \this(NaN, \a x) is NaN. \n
  \b \this(\plusminus~0, +(anything but NaN)) is \plusminus~0. \n
  \b \this(\plusminus~0, \minus(anything but NaN)) is \plusminus\pi. \n
  \b \this(\plusminus(anything but 0 and NaN), 0) is \plusminus\half\pi. \n
  \b \this(\plusminus(anything but \inf and NaN), +\inf) is \plusminus~0. \n
  \b \this(\plusminus(anything but \inf and NaN), \minus\inf) is \plusminus\pi. \n
  \b \this(\plusminus\inf, +\inf) is \plusminus\quarter\pi. \n
  \b \this(\plusminus\inf, \minus\inf) is \plusminus\threequarter\pi. \n
  \b \this(\plusminus\inf, (anything but 0, NaN, and \inf)) is \plusminus\half\pi.
*/
float atan2f(float __y, float __x);

#ifdef __cplusplus
}
extern "C++" {
float atan2(float __y, float __x);
double atan2(double __y, double __x);
}
extern "C" {
#else
/*! \brief Compute inverse tangent of a ratio of doubles \ingroup Inverse trigonometric functions \synopsis

  \desc \b \this returns the value, in radians, of the inverse circular tangent 
  of \b y divided by \a x using the signs of \a x and \b y to 
  compute the quadrant of the return value. The principal value lies in the interval 
  [\minus\pi, +\pi] radians. 
 
  If \a x = \b y = 0, \b errno is set to \b EDOM and \b \this returns \b HUGE_VAL. 

  \b \this(\a x, NaN) is NaN. \n
  \b \this(NaN, \a x) is NaN. \n
  \b \this(\plusminus~0, +(anything but NaN)) is \plusminus~0. \n
  \b \this(\plusminus~0, \minus(anything but NaN)) is \plusminus\pi. \n
  \b \this(\plusminus(anything but 0 and NaN), 0) is \plusminus\half\pi. \n
  \b \this(\plusminus(anything but \inf and NaN), +\inf) is \plusminus~0. \n
  \b \this(\plusminus(anything but \inf and NaN), \minus\inf) is \plusminus\pi. \n
  \b \this(\plusminus\inf, +\inf) is \plusminus\quarter\pi. \n
  \b \this(\plusminus\inf, \minus\inf) is \plusminus\threequarter\pi. \n
  \b \this(\plusminus\inf, (anything but 0, NaN, and \inf)) is \plusminus\half\pi.
*/
double atan2(double __y, double __x);
#endif


/*! \brief Set exponent of a float \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this breaks a floating-point number into a normalized fraction 
  and an integral power of 2.

  \b \this stores power of two in the \b int object 
  pointed to by \b \this and returns the value \a x, 
  such that \a x has a magnitude in the interval [\half, 1) or zero, 
  and value equals \a x * 2^\a exp.

  If \a x is zero, both parts of the result are zero.

  If \a x is \inf or NaN, \b \this \b returns \a x and stores zero 
  into the int object pointed to by \a exp.
*/
float frexpf(float __x, int *__exp);

#ifdef __cplusplus
}
extern "C++" {
float frexp(float __x, int *__exp);
double frexp(double __x, int *__exp);
}
extern "C" {
#else
/*! \brief Set exponent of a double \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this breaks a floating-point number into a normalized fraction 
  and an integral power of 2.

  \b \this stores power of two in the \b int object 
  pointed to by \a exp and returns the value \a x, 
  such that \a x has a magnitude in the interval [1/2, 1) or zero, 
  and value equals \a x * 2^\a exp.

  If \a x is zero, both parts of the result are zero.

  If \a x is \inf or NaN, \b \this returns \a x and stores zero into 
  the int object pointed to by \a exp.
*/
double frexp(double __x, int *__exp);
#endif


/*! \brief Adjust exponent of a float \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this multiplies a floating-point number by an integral power of 2.

  \b \this returns \a x * 2^\b exp.
  
  If the result overflows, \b errno is set to \b ERANGE 
  and \b \this returns \b HUGE_VALF. 

  If \a x is \inf or NaN, \b \this returns \a x.
  If the result overflows, \b \this returns \inf.
*/
float ldexpf(float __x, int __exp);

#ifdef __cplusplus
}
extern "C++" {
float ldexp(float __x, int __exp);
double ldexp(double __x, int __exp);
}
extern "C" {
#else
/*! \brief Adjust exponent of a double \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this multiplies a floating-point number by an integral power of 2.

  \b \this returns \a x * 2^\b exp.

  If the result overflows, \b errno is set to \b ERANGE and \b \this returns \b HUGE_VALF. 

  If \a x is \inf or NaN, \b \this returns \a x.
  If the result overflows, \b \this returns \inf.
*/
double ldexp(double __x, int __exp);
#endif


/*! \brief Scale a float \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this multiplies a floating-point number by an integral power
  of \b FLT_RADIX.

  As floating-point arithmetic conforms to IEC 60559, \b FLT_RADIX 
  is 2 and \b \this is (in this implementation) identical to \b ldexpf.

  \b \this returns \a x * \b FLT_RADIX ^\b exp.

  If the result overflows, \b errno is set to \b ERANGE
  and \b \this returns \b HUGE_VALF. 

  If \a x is \inf or NaN, \b \this returns \a x.
  If the result overflows, \b \this returns \inf.

  \sa ldexpf
*/
float scalbnf(float __x, int __exp);

#ifdef __cplusplus
}
extern "C++" {
float scalbn(float __x, int __exp);
double scalbn(double __x, int __exp);
}
extern "C" {
#else
/*! \brief Scale a double \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this multiplies a floating-point number by an integral power 
  of \b DBL_RADIX.

  As floating-point arithmetic conforms to IEC 60559, \b DBL_RADIX 
  is 2 and \b \this is (in this implementation) identical to \b ldexp.

  \b \this returns \a x * \b DBL_RADIX^\b exp.

  If the result overflows, \b errno is set to \b ERANGE and \b \this returns \b HUGE_VAL. 

  If \a x is \inf or NaN, \b \this returns \a x. \n
  If the result overflows, \b \this returns \inf.

  \sa ldexp
*/
double scalbn(double __x, int __exp);
#endif


/*! \brief Compute natural logarithm of a float \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this computes the base-\i e logarithm of \a x. 

  If \a x = 0, \b errno is set to \b ERANGE and \b \this returns \minus\b HUGE_VALF. 
  If \a x \lt 0, \b errno is set to \b EDOM and \b \this returns \minus\b HUGE_VALF. 

  If \a x \lt 0 or \a x = \minus\inf, \b \this returns NaN. \n
  If \a x = 0, \b \this returns \minus\inf. \n
  If \a x = \inf, \b \this returns \inf. \n
  If \a x = NaN, \b \this returns \a x.
*/
float logf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float log(float __x);
double log(double __x);
}
extern "C" {
#else
/*! \brief Compute natural logarithm of a double \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this computes the base-\i e logarithm of \a x. 

  If \a x = 0, \b errno is set to \b ERANGE and \b log returns \minus\b HUGE_VAL. 
  If \a x \lt 0, \b errno is set to \b EDOM and \b \this returns \minus\b HUGE_VAL. 

  If \a x \lt 0 or \a x = \minus\inf, \b \this returns NaN. \n
  If \a x = 0, \b \this returns \minus\inf. \n
  If \a x = \inf, \b \this returns \inf. \n
  If \a x = NaN, \b \this returns \a x.
*/
double log(double __x);
#endif


/*! \brief Compute common logarithm of a float \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this computes the base-10 logarithm of \a x. 

  If \a x = 0, \b errno is set to \b ERANGE and \b \this returns \minus\b HUGE_VALF. 
  If \a x \lt 0, \b errno is set to \b EDOM and \b \this returns \minus\b HUGE_VALF. 

  If \a x \lt 0 or \a x = \minus\inf, \b \this returns NaN. \n
  If \a x = 0, \b \this returns \minus\inf. \n
  If \a x = \inf, \b \this returns \inf. \n
  If \a x = NaN, \b \this returns \a x.
*/
float log10f(float __x);

#ifdef __cplusplus
}
extern "C++" {
float log10(float __x);
double log10(double __x);
}
extern "C" {
#else
/*! \brief Compute common logarithm of a double \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this computes the base-10 logarithm of \a x. 

  If \a x = 0, \b errno is set to \b ERANGE and \b \this returns \minus\b HUGE_VAL. 
  If \a x \lt 0, \b errno is set to \b EDOM and \b \this returns \minus\b HUGE_VAL. 

  If \a x \lt 0 or \a x = \minus\inf, \b \this returns NaN. \n
  If \a x = 0, \b \this returns \minus\inf. \n
  If \a x = \inf, \b \this returns \inf. \n
  If \a x = NaN, \b \this returns \a x.
*/
double log10(double __x);
#endif


/*! \brief Compute remainder after division of two floats \ingroup Rounding and remainder functions \synopsis

  \desc \b \this computes the floating-point remainder of \a x 
  divided by \b y. \b \this returns the value \a x \minus \i n \b y, 
  for some integer \i n such that, if \b y is nonzero, the 
  result has the same sign as \a x and magnitude less than the magnitude 
  of \b y.

  \b \this (NaN, \em y) is NaN.
  \b \this (\em x, NaN) is NaN.
  \b \this (\plusminus 0, \b y) is \plusminus 0 for \b y not zero. \n
  \b \this (\inf, \b y) is NaN. \n
  \b \this (\a x, 0) is NaN. \n
  \b \this (x, \plusminus \inf) is \a x for \a x not infinite. \n
*/
float fmodf(float __x, float __y);

#ifdef __cplusplus
}
extern "C++" {
float fmod(float __x, float __y);
double fmod(double __x, double __y);
}
extern "C" {
#else
/*! \brief Compute remainder after division of two doubles \ingroup Rounding and remainder functions \synopsis

  \desc \b \this computes the floating-point remainder of \a x 
  divided by \b y. #b #this returns the value \a x \minus \i n \b y, 
  for some integer \i n such that, if \b y is nonzero, the 
  result has the same sign as \a x and magnitude less than the magnitude 
  of \b y.

  \b \this (NaN, \em y) is NaN.
  \b \this (\em x, NaN) is NaN.
  \b \this (\plusminus 0, \a y) is \plusminus 0 for \b y not zero. \n
  \b \this (\inf, \b y) is NaN. \n
  \b \this (\a x, 0) is NaN. \n
  \b \this (x, \plusminus \inf) is \a x for \a x not infinite. \n
*/
double fmod(double __x, double __y);
#endif


/*! \brief Break a float into integer and fractional parts \ingroup Rounding and remainder functions \synopsis

  \desc \b \this breaks \a x into integral and fractional 
  parts, each of which has the same type and sign as \a x.

  The integral part (in floating-point format) is stored in the object pointed 
  to by \b iptr and \b \this returns the signed fractional 
  part of \a x.
*/
float modff(float __x, float *__iptr);

#ifdef __cplusplus
}
extern "C++" {
float modf(float __x, float *__iptr);
double modf(double __x, double *__iptr);
}
extern "C" {
#else
/*! \brief Break a double into integer and fractional parts \ingroup Rounding and remainder functions \synopsis

  \desc \b \this breaks \a x into integral and fractional 
  parts, each of which has the same type and sign as \a x.

  The integral part (in floating-point format) is stored in the object pointed 
  to by \b iptr and \b \this returns the signed fractional 
  part of \a x.
*/
double modf(double __x, double *__iptr);
#endif


/*! \brief Raise a float to a power \ingroup Power functions \synopsis

  \desc \b \this computes \a x raised to the power \b y. 

  If \a x \lt 0 and \b y \le 0, \b errno.
  is set to \b EDOM and \b \this returns \minus\b HUGE_VALF.
  If \a x \le 0 and \b y is not an integer value, \b errno is set to \b EDOM 
  and \b pow returns \minus\b HUGE_VALF.

  If \b y = 0, \b \this returns 1. \n
  If \b y = 1, \b \this returns \a x. \n
  If \b y = NaN, \b \this returns NaN. \n
  If \a x = NaN and \b y is anything other than 0, \b \this returns NaN. \n
  If \a x \lt \minus~1 or 1 \lt \a x, and \b y = +\inf, \b \this returns +\inf. \n
  If \a x \lt \minus~1 or 1 \lt \a x, and \b y = \minus\inf, \b \this returns 0. \n
  If \minus~1 \lt \a x \lt 1 and \b y = +\inf, \b \this returns +0. \n
  If \minus~1 \lt \a x \lt 1 and \b y = \minus\inf, \b \this returns +\inf. \n
  If \a x = +1 or \a x = \minus~1 and \b y = +\inf or \b y = \minus\inf, \b \this returns NaN. \n
  If \a x = +0 and \b y \gt 0 and \b y \ne NaN, \b \this returns +0. \n
  If \a x = \minus~0 and \b y \gt 0 and \b y \ne NaN or \b y not an odd integer, \b \this returns +0. \n
  If \a x = +0 and \b y \lt0 and \b y \ne NaN, \b \this returns +\inf. \n
  If \a x = \minus~0 and \b y \gt 0 and \b y \ne NaN or \b y not an odd integer, \b \this returns +\inf. \n
  If \a x = \minus~0 and \b y is an odd integer, \b \this returns \minus~0. \n
  If \a x = +\inf and \b y \gt 0 and \b y \ne NaN, \b \this returns +\inf. \n
  If \a x = +\inf and \b y \lt 0 and \b y  \ne NaN, \b \this returns +0. \n
  If \b x = \minus\inf, \b \this returns \b \this(\minus~0, \b y) \n
  If \a x \lt 0 and \a x \ne \inf and \b y is a non-integer, \b \this returns NaN. \n
*/
float powf(float __x, float __y);

#ifdef __cplusplus
}
extern "C++" {
float pow(float __x, float __y);
double pow(double __x, double __y);
}
extern "C" {
#else
/*! \brief Raise a double to a power \ingroup Power functions \synopsis

  \desc \b \this computes \a x raised to the power \b y. 

  If \a x \lt 0 and \b y \le 0, \b errno 
  is set to \b EDOM and \b \this returns \minus\b HUGE_VAL.
  If \a x \le 0 and \b y is not an integer value, \b errno is set to \b EDOM
  and \b \this returns \minus\b HUGE_VAL.

  If \b y = 0, \b \this returns 1. \n
  If \b y = 1, \b \this returns \a x. \n
  If \b y = NaN, \b \this returns NaN. \n
  If \a x = NaN and \b y is anything other than 0, \b \this returns NaN. \n
  If \a x \lt \minus~1 or 1 \lt \a x, and \b y = +\inf, \b \this returns +\inf. \n
  If \a x \lt \minus~1 or 1 \lt \a x, and \b y = \minus\inf, \b \this returns 0. \n
  If \minus~1 \lt \a x \lt 1 and \b y = +\inf, \b \this returns +0. \n
  If \minus~1 \lt \a x \lt 1 and \b y = \minus\inf, \b \this returns +\inf. \n
  If \a x = +1 or \a x = \minus~1 and \b y = +\inf or \b y = \minus\inf, \b \this returns NaN. \n
  If \a x = +0 and \b y \gt 0 and \b y \ne NaN, \b \this returns +0. \n
  If \a x = \minus~0 and \b y \gt 0 and \b y \ne NaN or \b y not an odd integer, \b \this returns +0. \n
  If \a x = +0 and \b y \lt0 and \b y \ne NaN, \b \this returns +\inf. \n
  If \a x = \minus~0 and \b y \gt 0 and \b y \ne NaN or \b y not an odd integer, \b \this returns +\inf. \n
  If \a x = \minus~0 and \b y is an odd integer, \b \this returns \minus~0. \n
  If \a x = +\inf and \b y \gt 0 and \b y \ne NaN, \b \this returns +\inf. \n
  If \a x = +\inf and \b y \lt 0 and \b y  \ne NaN, \b \this returns +0. \n
  If \b x = \minus\inf, \b \this returns \b \this(\minus~0, \b y) \n
  If \a x \lt 0 and \a x \ne \inf and \b y is a non-integer, \b \this returns NaN. \n
*/
double pow(double __x, double __y);
#endif


/*! \brief Compute square root of a float \ingroup Power functions \synopsis

  \desc \b \this computes the nonnegative square root of \a x. 
  C90 and C99 require that a domain error occurs if the argument is less than 
  zero \this deviates and always uses IEC 60559 semantics. 

  If \b x is +0, \b \this returns +0. \n
  If \b x is \minus~0, \b \this returns \minus~0. \n
  If \b x is \inf, \b \this returns \inf. \n
  If \b x \lt 0, \b \this returns NaN. \n
  If \b x is NaN, \b \this returns that NaN.
*/
float sqrtf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float sqrt(float __x);
double sqrt(double __x);
}
extern "C" {
#else
/*! \brief Compute square root of a double \ingroup Power functions \synopsis

  \desc \b \this computes the nonnegative square root of \a x. 
  C90 and C99 require that a domain error occurs if the argument is less than 
  zero \this deviates and always uses IEC 60559 semantics. 

  If \b x is +0, \b \this returns +0. \n
  If \b x is \minus~0, \b \this returns \minus~0. \n
  If \b x is \inf, \b \this returns \inf. \n
  If \b x \lt 0, \b \this returns NaN. \n
  If \b x is NaN, \b \this returns that NaN.
*/
double sqrt(double __x);
#endif


/*! \brief Compute cube root of a float \ingroup Power functions \synopsis

  \desc \b \this computes the cube root of \a x.
*/
float cbrtf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float cbrt(float __x);
double cbrt(double __x);
}
extern "C" {
#else
/*! \brief Compute cube root of a double \ingroup Power functions \synopsis

  \desc \b \this computes the cube root of \a x.
*/
double cbrt(double __x);
#endif


/*! \brief Compute smallest integer not greater than a float \ingroup Rounding and remainder functions \synopsis

  \desc \b \this computes the smallest integer value not less than \a x.

  \b \this (\plusminus~0) is \plusminus~0.
  \b \this (\plusminus \inf) is \plusminus \inf.
*/
float ceilf(float __x);


#ifdef __cplusplus
}
extern "C++" {
float ceil(float __x);
double ceil(double __x);
}
extern "C" {
#else
/*! \brief Compute smallest integer not greater than a double \ingroup Rounding and remainder functions \synopsis

 \desc \b \this computes the smallest integer value not less than \a x.

 \b \this (\plusminus~0) is \plusminus~0.
 \b \this (\plusminus \inf) is \plusminus \inf.
*/
double ceil(double __x);
#endif


/*! \brief Compute absolute value of a float \ingroup Absolute value functions \synopsis

  \desc \b \this computes the absolute value of the floating-point number \a x.
*/
float fabsf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float fabs(float __x);
double fabs(double __x);
}
extern "C" {
#else
/*! \brief Compute absolute value of a double \ingroup Absolute value functions \synopsis

  #desc \b \this computes the absolute value of the floating-point number \a x.
*/
double fabs(double __x);
#endif


/*! \brief Compute minimum of two floats \ingroup Maximum, minimum, and positive difference functions \synopsis

  \desc \b \this determines the minimum of \a x and \b y.

  \b \this (NaN, \b y) is \b y.
  \b \this (\a x, NaN) is \a x. 
*/
float fminf(float __x, float __y);

#ifdef __cplusplus
}
extern "C++" {
float fmin(float __x, float __y);
double fmin(double __x, double __y);
}
extern "C" {
#else
/*! \brief Compute minimum of two doubles \ingroup Maximum, minimum, and positive difference functions \synopsis

  \desc \b \this determines the minimum of \a x and \b y.

  \b \this (NaN, \b y) is \b y.
  \b \this (x, NaN) is \a x.
*/
double fmin(double __x, double __y);
#endif


/*! \brief Compute maximum of two floats \ingroup Maximum, minimum, and positive difference functions \synopsis

  \desc \b \this determines the maximum of \a x and \b y.

  \b \this (NaN, \b y) is \b y.
  \b \this(\a x, NaN) is \a x.
*/
float fmaxf(float __x, float __y);

#ifdef __cplusplus
}
extern "C++" {
float fmax(float __x, float __y);
double fmax(double __x, double __y);
}
extern "C" {
#else
/*! \brief Compute maximum of two doubles \ingroup Maximum, minimum, and positive difference functions \synopsis

  \desc \b \this determines the maximum of \a x and \b y.

  \b \this (NaN, \b y) is \b y.
  \b \this (\a x, NaN) is \a x.
*/
double fmax(double __x, double __y);
#endif


/*! \brief Compute largest integer not greater than a float \ingroup Rounding and remainder functions \synopsis

 \b \this computes the largest integer value not greater than \a x.

 \b \this(\plusminus~0) is \plusminus~0.
 \b \this(\plusminus\inf) is \plusminus\inf.
*/
float floorf(float);

#ifdef __cplusplus
}
extern "C++" {
float floor(float __x);
double floor(double __x);
}
extern "C" {
#else
/*! \brief Compute largest integer not greater than a double \ingroup Rounding and remainder functions \synopsis

 \b \this computes the largest integer value not greater than \a x.

 \b \this  (\plusminus~0) is \plusminus~0.
  \b \this (\plusminus\inf) is \plusminus\inf.
*/
double floor(double);
#endif


/*! \brief Compute complex magnitude of two floats \ingroup Power functions \synopsis

  \desc \b \this computes the square root of the sum of the squares of 
  \a x and \b y, \b sqrtf(\a x*\a x + \b y*\b y), without undue overflow or underflow. 
  If \a x and \b y are the lengths of the sides of a right-angled triangle,
  then \b \this computes the length of the hypotenuse.

  If \a x or \b y is +\inf or \minus\inf, \b \this returns \inf.
  If \a x or \b y is NaN, \b \this returns NaN. 
*/
float hypotf(float __x, float __y);

#ifdef __cplusplus
}
extern "C++" {
float hypot(float __x, float __y);
double hypot(double __x, double __y);
}
extern "C" {
#else
/*! \brief Compute complex magnitude of two doubles \ingroup Power functions \synopsis

  \desc \b \this computes the square root of the sum of the squares of 
  \a x and \b y, \b sqrt(\a x*\a x + \b y*\b y), without undue overflow or underflow. 
  If \a x and \b y are the lengths of the sides of a right-angled triangle,
  then \b \this computes the length of the hypotenuse.

  If \a x or \b y is +\inf or \minus\inf, \b \this returns \inf. \n
  If \a x or \b y is NaN, \b \this returns NaN.
*/
double hypot(double __x, double __y);
#endif


/*! \brief Compute hyperbolic cosine of a float \ingroup Hyperbolic functions \synopsis

  \desc \b \this calculates the hyperbolic sine of \a x. 

  If |\a x| \gt ~88.7228, \b errno is set to \b EDOM and \b \this
  returns \b HUGE_VALF.

  If \a x is +\inf, \minus\inf, or NaN, \b \this returns |\a x|. \n
  If |\a x| \gt ~88.7228, \b \this returns +\inf or \minus\inf depending upon the sign of \a x.
*/
float coshf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float cosh(float __x);
double cosh(double __x);
}
extern "C" {
#else
/*! \brief Compute hyperbolic cosine of a double \ingroup Hyperbolic functions \synopsis

  \desc \b \this calculates the hyperbolic cosine of \a x. 

  If |\a x| \gt ~709.782, \b errno 
  is set to \b EDOM and \b cosh returns \b HUGE_VAL. 

  If \a x is +\inf, \minus\inf, or NaN, \b \this returns |\a x|.>
  If |\a x| \gt ~709.782, \b cosh returns +\inf or \minus\inf depending upon the sign of \a x. 
*/
double cosh(double __x);
#endif


/*! \brief Compute hyperbolic sine of a float \ingroup Hyperbolic functions \synopsis

  \desc \b \this calculates the hyperbolic sine of \a x. 

  If |\a x| \gt ~88.7228, \b errno is set to \b EDOM and \b \this returns \b HUGE_VALF. 

  If \a x is +\inf, \minus\inf, or NaN, \b \this returns |\a x|.
  If |\a x| \gt ~88.7228, \b \this returns +\inf or \minus\inf depending upon the sign of \a x.
*/
float sinhf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float sinh(float __x);
double sinh(double __x);
}
extern "C" {
#else
/*! \brief Compute hyperbolic sine of a double \ingroup Hyperbolic functions \synopsis

  \desc \b \this calculates the hyperbolic sine of \a x. 

  If |\a x| \gt709.782, \b errno is set to \b EDOM and \b \this returns \b HUGE_VAL. 

  If \a x is +\inf, \minus\inf, or NaN, \b \this returns |\a x|.
  If |\a x| \gt ~709.782, \b \this returns +\inf or \minus\inf depending upon the sign of \a x. 
*/
double sinh(double __x);
#endif


/*! \brief Compute hyperbolic tangent of a float \ingroup Hyperbolic functions \synopsis

  \desc \b \this calculates the hyperbolic tangent of \a x. 

  If \a x is NaN, \b \this returns NaN.
*/

float tanhf(float __x);


#ifdef __cplusplus
}
extern "C++" {
float tanh(float __x);
double tanh(double __x);
}
extern "C" {
#else
/*! \brief Compute hyperbolic tangent of a double \ingroup Hyperbolic functions \synopsis
 
  \desc \b \this calculates the hyperbolic tangent of \a x. 

  If \a x is NaN, \b \this returns NaN.
*/
double tanh(double __x);
#endif


/*! \brief Compute exponential of a float \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this computes the base-\i e exponential of \a x. 

  If |\a x| \gt ~88.722, \b errno is set to \b EDOM and \b \this returns \b HUGE_VALF. 
  
  If \a x is NaN, \b \this returns NaN. \n
  If \a x is \inf, \b \this returns \inf. \n
  If \a x is \minus\inf, \b \this returns 0. 
*/
float expf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float exp(float __x);
double exp(double __x);
}
extern "C" {
#else
/*! \brief Compute exponential of a double \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this computes the base-\i e exponential of \a x. 

  If |\a x| \gt ~709.782, \b errno is set to \b EDOM and \b exp returns \b HUGE_VAL. 

  If \a x is NaN, \b exp returns NaN. \n
  If \a x is \inf, \b \this returns \inf. \n
  If \a x is \minus\inf, \b \this returns 0. 
*/
double exp(double __x);
#endif


/*! \brief Compute inverse hyperbolic cosine of a float \ingroup Inverse hyperbolic functions \synopsis

  \desc \b \this returns the non-negative inverse hyperbolic cosine of \a x. 

  \b acosh(\i x) is defined as \b log(\i x + \b sqrt(\i x^2 \minus 1)), assuming completely accurate computation.

  If \a x \lt 1, \b errno is set to \b EDOM and \b \this returns \b HUGE_VALF.

  If \a x \lt 1, \b \this returns NaN. \n
  If \a x is NaN, \b \this returns that NaN.
*/
float acoshf(float __x);


#ifdef __cplusplus
}
extern "C++" {
float acosh(float __x);
double acosh(double __x);
}
extern "C" {
#else
/*! \brief Compute inverse hyperbolic cosine of a double \ingroup Inverse hyperbolic functions \synopsis

  \desc \b \this returns the non-negative inverse hyperbolic cosine of \a x. 

  \b acosh(\i x) is defined as \b log(\i x + \b sqrt(\i x^2 \minus 1)), assuming completely accurate computation.

  If \a x \lt 1, \b errno is set to \b EDOM and \b \this returns \b HUGE_VAL. 

  If \a x \lt 1, \b \this returns NaN. \n
  If \a x is NaN, \b \this returns NaN. 
*/
double acosh(double __x);
#endif


/*! \brief Compute inverse hyperbolic sine of a float \ingroup Inverse hyperbolic functions \synopsis

  \desc \b \this calculates the hyperbolic sine of \a x. 

  If |\a x| \gt ~88.7228, \b errnois set to \b EDOM and \b \this returns \b HUGE_VALF. 

  If \a x is +\inf, \minus\inf, or NaN, \b \this returns |\a x|.
  If |\a x| \gt ~88.7228, \b \this  returns +\inf or \minus\inf depending upon the sign of \a x.
*/
float asinhf(float __x);


#ifdef __cplusplus
}
extern "C++" {
float asinh(float __x);
double asinh(double __x);
}
extern "C" {
#else
/*! \brief Compute inverse hyperbolic sine of a double \ingroup Inverse hyperbolic functions \synopsis

  \desc \b \this calculates the hyperbolic sine of \a x. 

  If |\a x| \gt ~709.782, \b errno is set to \b EDOM and \b \this returns \b HUGE_VAL. 

  If \a x is +\inf, \minus\inf, or NaN, \b \this returns |\a x|.
  If |\a x| \gt ~709.782, \b \this returns +\inf or \minus\inf depending upon the sign of \a x. 
*/
double asinh(double __x);
#endif


/*! \brief Compute inverse hyperbolic tangent of a float \ingroup Inverse hyperbolic functions \synopsis

  \desc \b \this returns the inverse hyperbolic tangent of \a x. 

  If |\a x| \gt 1 \b \this returns NaN.
  If \a x is NaN, \b \this returns that NaN.
  If \a x is 1, \b \this returns \inf.
  If \a x is \minus~1, \b \this returns \minus\inf. 
*/
float atanhf(float __x);


#ifdef __cplusplus
}
extern "C++" {
float atanh(float __x);
double atanh(double __x);
}
extern "C" {
#else
/*! \brief Compute inverse hyperbolic tangent of a double \ingroup Inverse hyperbolic functions \synopsis

  \desc \b \this returns the inverse hyperbolic tangent of \a x. 

  If |\a x| \ge 1, \b errno is set to \b EDOM and \b \this returns \b HUGE_VAL. 

  If |\a x| \gt 1 \b \this returns NaN. \n
  If \a x is NaN, \b \this returns that NaN. \n
  If \a x is 1, \b \this returns \inf. \n
  If \a x is \minus~1, \b \this returns \minus\inf.
*/
double atanh(double __x);
#endif


/*! \brief Compute fused multiply-add of floats \ingroup Fused multiply functions \synopsis

  \desc \b \this computes \a x \times \a y + \a z with a single rounding.
*/
float fmaf(float __x, float __y, float __z);

#ifdef __cplusplus
}
extern "C++" {
float fma(float __x, float __y, float __z);
double fma(double __x, double __y, double __z);
}
extern "C" {
#else
/*! \brief Compute fused multiply-add of doubles \ingroup Fused multiply functions \synopsis

  \desc \b \this computes \a x \times \a y + \a z with a single rounding.
*/
double fma(double __x, double __y, double __z);
#endif


/*! \brief Copy magnitude and sign of a float \ingroup Floating-point manipulation functions \synopsis

  \desc \b \this returns a value with the magnitude of \a x and the sign of \a y.
*/
float copysignf(float __x, float __y);

#ifdef __cplusplus
}
extern "C++" {
float copysign(float __x, float __y);
double copysign(double __x, double __y);
}
extern "C" {
#else
/*! \brief Copy magnitude and sign of a double \ingroup Floating-point manipulation functions \synopsis

  \desc \b \this returns a value with the magnitude of \a x and the sign of \a y.
*/
double copysign(double __x, double __y);
#endif


/*! \brief Compute error function of a float \ingroup Error and Gamma functions \synopsis

  \desc \b \this returns the error function for \a x.
*/
float erff(float __x);

#ifdef __cplusplus
}
extern "C++" {
float erf(float __x);
double erf(double __x);
}
extern "C" {
#else
/*! \brief Compute error function of a double \ingroup Error and Gamma functions \synopsis

  \desc \b \this returns the error function for \a x.
*/
double erf(double __x);
#endif


/*! \brief Compute complementary error function of a float \ingroup Error and Gamma functions \synopsis

  \desc \b \this returns the complementary error function for \a x.
*/
float erfcf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float erfc(float __x);
double erfc(double __x);
}
extern "C" {
#else
/*! \brief Compute complementary error function of a double \ingroup Error and Gamma functions \synopsis

  \desc \b \this returns the complementary error function for \a x.
*/
double erfc(double __x);
#endif


/*! \brief Compute binary exponential of a float \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this returns 2 raised to the power of \a x.
*/
float exp2f(float __x);

#ifdef __cplusplus
}
extern "C++" {
float exp2(float __x);
double exp2(double __x);
}
extern "C" {
#else
/*! \brief Compute binary exponential of a double \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this returns 2 raised to the power of \a x.
*/
double exp2(double __x);
#endif


/*! \brief Compute exponential minus one of a float\ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this returns \i e raised to the power of \a x minus one.
*/
float expm1f(float __x);

#ifdef __cplusplus
}
extern "C++" {
float expm1(float __x);
double expm1(double __x);
}
extern "C" {
#else
/*! \brief Compute exponential minus one of a double\ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this returns \i e raised to the power of \a x minus one.
*/
double expm1(double __x);
#endif


/*! \brief Compute positive difference of two floats \ingroup \ingroup Maximum, minimum, and positive difference functions \synopsis

  \desc \b \this returns the positive difference between \a x and \a y.
*/
float fdimf(float __x, float __y);

#ifdef __cplusplus
}
extern "C++" {
float fdim(float __x, float __y);
double fdim(double __x, double __y);
}
extern "C" {
#else
/*! \brief Compute positive difference of two doubles \ingroup \ingroup Maximum, minimum, and positive difference functions \synopsis

  \desc \b \this returns the positive difference between \a x and \a y.
*/
double fdim(double __x, double __y);
#endif


/*! \brief Compute integer binary logarithm of a float \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this returns the integral part of the logarithm of \a x, using \b FLT_RADIX as the base for the logarithm.
*/
int ilogbf(float __x);


#ifdef __cplusplus
}
extern "C++" {
int ilogb(float __x);
int ilogb(double __x);
}
extern "C" {
#else
/*! \brief Compute integer binary logarithm of a double \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this returns the integral part of the logarithm of \a x, using \b FLT_RADIX as the base for the logarithm.
*/
int ilogb(double __x);
#endif


/*! \brief Compute log-gamma function of a float \ingroup Error and Gamma functions \synopsis

  \desc \b \this returns the natural logarithm of the gamma function for \a x.
*/
float lgammaf(float __x);

#ifdef __cplusplus
}
extern "C++" {
int lgamma(float __x);
int lgamma(double __x);
}
extern "C" {
#else
/*! \brief Compute log-gamma function of a double \ingroup Error and Gamma functions \synopsis

  \desc \b \this returns the natural logarithm of the gamma function for \a x.
*/
double lgamma(double __x);
#endif


/*! \brief Round and cast float to long long \ingroup Rounding and remainder functions \synopsis

  \desc \b \this rounds \a x to an integral value and returns it as a long long int.
*/
long long int llrintf(float __x);

#ifdef __cplusplus
}
extern "C++" {
long long int llrint(float __x);
long long int llrint(double __x);
}
extern "C" {
#else
/*! \brief Round and cast double to long long \ingroup Rounding and remainder functions \synopsis

  \desc \b \this rounds \a x to an integral value and returns it as a long long int.
*/
long long int llrint(double __x);
#endif


/*! \brief Round and cast float to long long \ingroup Rounding and remainder functions \synopsis

  \desc \b \this rounds \a x to an integral value, with halfway cases rounded away from zero, and returns it as a long long int.
*/
long long int llroundf(float __x);

#ifdef __cplusplus
}
extern "C++" {
long long int llround(float __x);
long long int llround(double __x);
}
extern "C" {
#else
/*! \brief Round and cast double to long long \ingroup Rounding and remainder functions \synopsis

  \desc \b \this rounds \a x to an integral value, with halfway cases rounded away from zero, and returns it as a long long int.
*/
long long int llround(double __x);
#endif


/*! \brief Compute natural logarithm plus one of a float\ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this computes the base-\i e logarithm of \a x plus one.
*/
float log1pf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float log1p(float __x);
double log1p(double __x);
}
extern "C" {
#else
/*! \brief Compute natural logarithm plus one of a double\ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this computes the base-\i e logarithm of \a x plus one.
*/
double log1p(double __x);
#endif


/*! \brief Compute binary logarithm of a float\ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this computes the base-\i 2 logarithm of \a x.
*/
float log2f(float __x);

#ifdef __cplusplus
}
extern "C++" {
float log2(float __x);
double log2(double __x);
}
extern "C" {
#else
/*! \brief Compute binary logarithm of a double\ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this computes the base-\i 2 logarithm of \a x.
*/
double log2(double __x);
#endif


/*! \brief Compute floating-point base logarithm of a float\ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this computes the base-\i FLT_RADIX logarithm of \a x.
*/
float logbf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float logb(float __x);
double logb(double __x);
}
extern "C" {
#else
/*! \brief Compute floating-point base logarithm of a double\ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this computes the base-\i FLT_RADIX logarithm of \a x.
*/
double logb(double __x);
#endif

/*! \brief Round and cast float to long \ingroup Rounding and remainder functions \synopsis

  \desc \b \this rounds \a x to an integral value and returns it as a long int.
*/
long int lrintf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float lrint(float __x);
double lrint(double __x);
}
extern "C" {
#else
/*! \brief Round and cast double to long \ingroup Rounding and remainder functions \synopsis

  \desc \b \this rounds \a x to an integral value and returns it as a long int.
*/
long int lrint(double __x);
#endif


/*! \brief Round and cast float to long \ingroup Rounding and remainder functions \synopsis

  \desc \b \this rounds \a x to an integral value, with halfway cases rounded away from zero, and returns it as a long int.
*/
long int lroundf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float lround(float __x);
double lround(double __x);
}
extern "C" {
#else
/*! \brief Round and cast double to long \ingroup Rounding and remainder functions \synopsis

  \desc \b \this rounds \a x to an integral value, with halfway cases rounded away from zero, and returns it as a long int.
*/
long int lround(double __x);
#endif


/*! \brief Round float to nearby integral value \ingroup Rounding and remainder functions \synopsis

  \desc \b \this Rounds \a x to an integral value.
*/
float nearbyintf(float);

#ifdef __cplusplus
}
extern "C++" {
float nearbyint(float __x);
double nearbyint(double __x);
}
extern "C" {
#else
/*! \brief Round double to nearby integral value \ingroup Rounding and remainder functions \synopsis

  \desc \b \this Rounds \a x to an integral value.
*/
double nearbyint(double);
#endif


/*! \brief Next representable float value \ingroup Floating-point manipulation functions \synopsis

  \desc \b \this Returns the next representable value after \a x in the direction of \a y.
*/
float nextafterf(float __x, float __y);

#ifdef __cplusplus
}
extern "C++" {
float nextafter(float __x, float __y);
double nextafter(double __x, double __y);
}
extern "C" {
#else
/*! \brief Next representable double value \ingroup Floating-point manipulation functions \synopsis

  \desc \b \this Returns the next representable value after \a x in the direction of \a y.
*/
double nextafter(double __x, double __y);
#endif


/*! \brief Compute remainder of a float \ingroup Rounding and remainder functions \synopsis

  \desc \b \this computes the remainder of \a numer divided by \a denom.
*/
float remainderf(float numer, float denom);

#ifdef __cplusplus
}
extern "C++" {
float remainder(float numer, float denom);
double remainder(double numer, double denom);
}
extern "C" {
#else
/*! \brief Compute remainder of a double \ingroup Rounding and remainder functions \synopsis

  \desc \b \this computes the remainder of \a numer divided by \a denom.
*/
double remainder(double numer, double denom);
#endif


/*! \brief Compute remainder and quotient of a float \ingroup Rounding and remainder functions \synopsis

  \desc \b \this computes the remainder of \a numer divided by \a denom and the quotient pointed by \a quot.
*/
float remquof(float numer, float denom, int *quot);

#ifdef __cplusplus
}
extern "C++" {
float remquo(float numer, float denom, int *quot);
double remquo(double numer, double denom, int *quot);
}
extern "C" {
#else
/*! \brief Compute remainder and quotient of a double \ingroup Rounding and remainder functions \synopsis

  \desc \b \this computes the remainder of \a numer divided by \a denom and the quotient pointed by \a quot.
*/
double remquo(double numer, double denom, int *quot);
#endif


/*! \brief Round a float to an integral value \ingroup Rounding and remainder functions \synopsis

  \desc \b \this rounds \a x to an integral value.
*/
float rintf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float rint(float numer);
double rint(double numer);
}
extern "C" {
#else
/*! \brief Round a double to an integral value \ingroup Rounding and remainder functions \synopsis

  \desc \b \this rounds \a x to an integral value.
*/
double rint(double __x);
#endif


/*! \brief Round a float to the nearest integral value \ingroup Rounding and remainder functions \synopsis

  \desc \b \this rounds \a x to an integral value, with halfway cases rounded away from zero.
*/
float roundf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float round(float numer);
double round(double numer);
}
extern "C" {
#else
/*! \brief Round a double to the nearest integral value \ingroup Rounding and remainder functions \synopsis

  \desc \b \this rounds \a x to an integral value, with halfway cases rounded away from zero.
*/
double round(double __x);
#endif


/*! \brief Scale a float \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this multiplies \a x by \b FLT_RADIX raised to the power \a exp.
*/
float scalblnf(float __x, long int __exp);

#ifdef __cplusplus
}
extern "C++" {
float scalbln(float __x, long int __exp);
double scalbln(double __x, long int __exp);
}
extern "C" {
#else
/*! \brief Scale a double \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this multiplies \a x by \b FLT_RADIX raised to the power \a exp.
*/
double scalbln(double __x, long int __exp);
#endif


/*! \brief Compute gamma function of a float \ingroup Error and Gamma functions \synopsis

  \desc \b \this returns the gamma function for \a x.
*/
float tgammaf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float tgamma(float __x);
double tgamma(double __x);
}
extern "C" {
#else
/*! \brief Compute gamma function of a double \ingroup Error and Gamma functions \synopsis

  \desc \b \this returns the gamma function for \a x.
*/
double tgamma(double __x);
#endif


/*! \brief Truncate a float value \ingroup Rounding and remainder functions \synopsis

  \desc \b \this rounds \a x to an integral value that is not larger in magnitude than \a x.
*/
float truncf(float __x);

#ifdef __cplusplus
}
extern "C++" {
float trunc(float __x);
double trunc(double __x);
}
extern "C" {
#else
/*! \brief Truncate a double value \ingroup Rounding and remainder functions \synopsis

  \desc \b \this rounds \a x to an integral value that is not larger in magnitude than \a x.
*/
double trunc(double __x);
#endif

#ifdef __cplusplus
}
#endif

#endif

#endif
