// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __float_h
#define __float_h

/*! \brief The rounding mode \ingroup Implementation \synopsis

  \description \this specifies the rounding mode of floating-point addition is round to nearest.
*/
#define FLT_ROUNDS 1

/*! \brief The evaluation format \ingroup Implementation \synopsis

  \description \this specifies that all operations and constants are evaluated to the range and precision of the type.
*/
#define FLT_EVAL_METHOD 0

/*! \brief The radix of the exponent representation \ingroup Implementation \synopsis

  \description \this specifies the radix of the exponent representation.
*/
#define FLT_RADIX                 2

/*! \brief The number of decimal digits that can be rounded without change \ingroup Implementation \synopsis

  \description \this specifies the number of decimal digits that can be rounded to a floating-point number
  without change to the value.
*/
#define DECIMAL_DIG              17

/*! \brief The number of digits in a float \ingroup Implementation \synopsis

  \description \this specifies the number of base \ref FLT_RADIX digits in the mantissa part of a \b float.
*/
#define FLT_MANT_DIG             24

/*! \brief The difference between 1 and the least value greater than 1 of a float\ingroup Float minimum and maximum values \synopsis

  \description \this the minimum positive number such that 1.0 + FLT_EPSILON != 1.0.
*/
#define FLT_EPSILON 1.19209290E-07F // decimal constant

/*! \brief The number of digits of precision of a float \ingroup Implementation \synopsis

  \description \this specifies The number of digits of precision of a \b float.
*/
#define FLT_DIG                   6

/*! \brief The minimal exponent value of a float \ingroup Float exponent minimum and maximum values \synopsis

  \description \this is the minimum value of base \ref FLT_RADIX in the exponent part of a \b float.
*/
#define FLT_MIN_EXP            -125

/*! \brief The minimal value of a float \ingroup Float minimum and maximum values \synopsis

  \description \this is the minimum value of a \b float.
*/
#define FLT_MIN     1.17549435E-38F

/*! \brief The minimal exponent value in base 10 of a float \ingroup Float exponent minimum and maximum values \synopsis

  \description \this is the minimum value in base 10 of the exponent part of a \b float.
*/
#define FLT_MIN_10_EXP          -37

/*! \brief The maximum exponent value of a float \ingroup Float exponent minimum and maximum values \synopsis

  \description \this is the maximum value of base \ref FLT_RADIX in the exponent part of a \b float.
*/
#define FLT_MAX_EXP            +128

/*! \brief The maximum value of a float \ingroup Float minimum and maximum values \synopsis

  \description \this is the maximum value of a \b float.
*/
#define FLT_MAX     3.40282347E+38F

/*! \brief The maximum exponent value in base 10 of a float \ingroup Float exponent minimum and maximum values \synopsis

  \description \this is the maximum value in base 10 of the exponent part of a \b float.
*/
#define FLT_MAX_10_EXP          +38

#ifdef __SHORT_DOUBLES

// double is 32-bit IEC 60559
#define DBL_MANT_DIG             24
#define DBL_EPSILON 1.19209290E-07F
#define DBL_DIG                   6
#define DBL_MIN_EXP            -125
#define DBL_MIN     1.17549435E-38F
#define DBL_MIN_10_EXP          -37
#define DBL_MAX_EXP            +128
#define DBL_MAX     3.40282347E+38F
#define DBL_MAX_10_EXP          +38

#else

// double is 64-bit IEC 60559

/*! \brief The number of digits in a double \ingroup Implementation \synopsis

  \description \this specifies the number of base \ref FLT_RADIX digits in the mantissa part of a \b double.
*/
#define DBL_MANT_DIG                    53

/*! \brief The difference between 1 and the least value greater than 1 of a double \ingroup Double minimum and maximum values \synopsis

  \description \this the minimum positive number such that 1.0 + DBL_EPSILON != 1.0.
*/
#define DBL_EPSILON 2.2204460492503131E-16

/*! \brief The number of digits of precision of a double \ingroup Implementation \synopsis

  \description \this specifies The number of digits of precision of a \b double.
*/
#define DBL_DIG                         15

/*! \brief The minimal exponent value of a double \ingroup Double exponent minimum and maximum values \synopsis

  \description \this is the minimum value of base \ref FLT_RADIX in the exponent part of a \b double.
*/
#define DBL_MIN_EXP                  -1021

/*! \brief The minimal value of a double \ingroup Double minimum and maximum values \synopsis

  \description \this is the minimum value of a \b double.
*/
#define DBL_MIN    2.2250738585072014E-308

/*! \brief The minimal exponent value in base 10 of a double \ingroup Double exponent minimum and maximum values \synopsis

  \description \this is the minimum value in base 10 of the exponent part of a \b double.
*/
#define DBL_MIN_10_EXP                -307

/*! \brief The maximum exponent value of a double \ingroup Double exponent minimum and maximum values \synopsis

  \description \this is the maximum value of base \ref FLT_RADIX in the exponent part of a \b double.
*/
#define DBL_MAX_EXP                  +1024

/*! \brief The maximum value of a double \ingroup Double minimum and maximum values \synopsis

  \description \this is the maximum value of a \b double.
*/
#define DBL_MAX    1.7976931348623157E+308

/*! \brief The maximum exponent value in base 10 of a double \ingroup Double exponent minimum and maximum values \synopsis

  \description \this is the maximum value in base 10 of the exponent part of a \b double.
*/
#define DBL_MAX_10_EXP                +308

#endif

#endif
