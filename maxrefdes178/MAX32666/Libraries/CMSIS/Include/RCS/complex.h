// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef	_complex_h
#define	_complex_h

#define complex _Complex
#define _Complex_I 1.0fi
#define I _Complex_I

/*! \brief Compute inverse cosine of a complex float \ingroup Trigonometric functions \synopsis

  \desc \b \this returns the principal value the inverse cosine of \a z with branch cuts outside the interval [-1,+1] on the real axis. 
  The principal value lies in the interval [0, \pi] on the real axis and in the range of a strip mathematically unbounded on the imaginary axis. 
*/
double complex cacos(double complex __z);
float complex cacosf(float complex __z);

/*! \brief Compute inverse sine of a complex float \ingroup Trigonometric functions \synopsis

  \desc \b \this returns the principal value the inverse sine of \a z with branch cuts outside the interval [-1,+1] on the real axis. 
  The principal value lies in the interval [\minus\half\pi, \half\pi] on the real axis and in the range of a strip mathematically unbounded on the imaginary axis. 
*/
double complex casin(double complex __z);
float complex casinf(float complex __z);

/*! \brief Compute inverse tangent of a complex float \ingroup Trigonometric functions \synopsis

  \desc \b \this returns the principal value the inverse sine of \a z with branch cuts outside the interval [-1,+1] on the real axis. 
  The principal value lies in the interval [\minus\half\pi, \half\pi] on the real axis and in the range of a strip mathematically unbounded on the imaginary axis. 
*/
double complex catan(double complex __z);
float complex catanf(float complex __z);

/*! \brief Compute cosine of a complex float \ingroup Trigonometric functions \synopsis

  \desc \b \this returns the complex cosine of \a z. 
*/
double complex ccos(double complex __z);
float complex ccosf(float complex __z);

/*! \brief Compute sine of a complex float \ingroup Trigonometric functions \synopsis

  \desc \b \this returns the complex sine of \a z. 
*/
double complex csin(double complex __z);
float complex csinf(float complex __z);

/*! \brief Compute tangent of a complex float \ingroup Trigonometric functions \synopsis

  \desc \b \this returns the complex tangent of \a z. 
*/
double complex ctan(double complex __z);
float complex ctanf(float complex __z);

/*! \brief Compute inverse hyperbolic cosine of a complex float \ingroup Hyperbolic trigonometric functions \synopsis

  \desc \b \this returns the principal value the inverse hyperbolic cosine of \a z with branch cuts of values less than 1 on the real axis. 
  The principal value lies in the range of a half-strip of non-negative values on the real axis and in the interval [-i\pi,+i\pi] on the imaginary axis.
*/
double complex cacosh(double complex __z);
float complex cacoshf(float complex _z);

/*! \brief Compute inverse hyperbolic sine of a complex float \ingroup Hyperbolic trigonometric functions \synopsis

  \desc \b \this returns the principal value the inverse hyperbolic sine of \a z with branch cuts outside the inteval [-i,+i] on the imaginary axis. 
  The principal value lies in the range of a strip mathematically unbounded on the real axis and in the interval [-i\half\pi,+i\half\pi] on the imaginary axis.
*/
double complex casinh(double complex __z);
float complex casinhf(float complex __z);

/*! \brief Compute inverse hyperbolic tangent of a complex float \ingroup Hyperbolic trigonometric functions \synopsis

  \desc \b \this returns the principal value the inverse hyperbolic sine of \a z with branch cuts outside the inteval [-1,+1] on the real axis. 
  The principal value lies in the range of a strip mathematically unbounded on the real axis and in the interval [-i\half\pi,+i\half\pi] on the imaginary axis.
*/
double complex catanh(double complex __z);
float complex catanhf(float complex __z);

/*! \brief Compute hyperbolic cosine of a complex float \ingroup Hyperbolic trigonometric functions \synopsis

  \desc \b \this returns the complex hyperbolic cosine of \a z. 
*/
double complex ccosh(double complex __z);
float complex ccoshf(float complex __z);

/*! \brief Compute hyperbolic sine of a complex float \ingroup Hyperbolic trigonometric functions \synopsis

  \desc \b \this returns the complex hyperbolic sine of \a z. 
*/
double complex csinh(double complex __z);
float complex csinhf(float complex __z);

/*! \brief Compute hyperbolic tangent of a complex float \ingroup Hyperbolic trigonometric functions \synopsis

  \desc \b \this returns the complex hyperbolic tangent of \a z. 
*/
double complex ctanh(double complex __z);
float complex ctanhf(float complex __z);

/*! \brief Computes the base-e exponential of a complex float  \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this returns the complex base-e exponential value of \a z. 
*/
double complex cexp(double complex __z);
float complex cexpf(float complex __z);

/*! \brief Computes the base-e logarithm of a complex float  \ingroup Exponential and logarithmic functions \synopsis

  \desc \b \this returns the complex base-e logarithm value of \a z. 
*/
double complex clog(double complex __z);
float complex clogf(float complex __z);

/*! \brief Computes the absolute value of a complex float  \ingroup Power and absolute value functions \synopsis

  \desc \b \this returns the absolute value of \a z. 
*/
double cabs(double complex __z);
float cabsf(float complex __z);

/*! \brief Compute a complex float raised to a power  \ingroup Power and absolute value functions \synopsis

  \desc \b \this computes \a x raised to the power \b y with a branch cut for the \a x along the negative real axis. 
*/
double complex cpow(double complex __x, double complex __y);
float complex cpowf(float complex __x, float complex __y);

/*! \brief Compute square root of a complex float \ingroup Power and absolute value functions \synopsis

  \desc \b \this computes the complex square root of \b z with a branch cut along the negative real axis. 
*/
double complex csqrt(double complex __z);
float complex csqrtf(float complex __z);

/*! \brief Compute argument of a complex float \ingroup Manipulation functions \synopsis

  \desc \b \this computes the argument of \b z with a branch cut along the negative real axis. 
*/
double carg(double complex __z);
float cargf(float complex __z);

/*! \brief Compute imaginary part of a complex float \ingroup Manipulation functions \synopsis

  \desc \b \this computes the imaginary part of \b z. 
*/
double cimag(double complex);
float cimagf(float complex);

/*! \brief Compute conjugate of a complex float \ingroup Manipulation functions \synopsis

  \desc \b \this computes the conjugate of \b z by reversing the sign of the imaginary part.
*/
double complex conj(double complex);
float complex conjf(float complex);

/*! \brief Compute projection on the Riemann sphere \ingroup Manipulation functions \synopsis

  \desc \b \this computes the projection of \b z on the Riemann sphere.
*/
double complex cproj(double complex);
float complex cprojf(float complex);

/*! \brief Compute real part of a complex float \ingroup Manipulation functions \synopsis

  \desc \b \this computes the real part of \b z. 
*/
double creal(double complex);
float crealf(float complex);

#endif
