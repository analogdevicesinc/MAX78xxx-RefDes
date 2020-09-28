// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __locale_H
#define __locale_H

#include "__crossworks.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL 0
#endif

#define LC_COLLATE    0
#define LC_CTYPE      1
#define LC_MONETARY   2
#define LC_NUMERIC    3
#define LC_TIME       4
#define LC_ALL        5

/* Check */
#if LC_ALL != __MAX_CATEGORY
#error Inconsistent categories
#endif

/*! \brief Formatting info for numeric values \ingroup Structures \synopsis

  \description \b \this structure holds formatting information on how numeric values
  are to be written.  Note that the order of fields in this structure is not consistent
  between implementations, nor is it consistent between C89 and C99 standards.

  The members \a decimal_point, \a grouping, and \a thousands_sep are controlled
  by \a LC_NUMERIC, the remainder by \a LC_MONETARY.

  The members \a int_n_cs_precedes, \a int_n_sep_by_space, \a int_n_sign_posn,
  \a int_p_cs_precedes, \a int_p_sep_by_space. and \a int_p_sign_posn are added
  by the C99 standard.

  We have standardized on the ordering specified by the ARM EABI for the
  base of this structure.  This ordering is neither that of C89 nor C99.

  \table{1.5in,*}
    || Member             | Description
    || currency_symbol    | Local currency symbol.
    || decimal_point      | Decimal point separator.
    || frac_digits        | Amount of fractional digits to the right of the decimal point for monetary quantities in the local format.
    || grouping           | Specifies the amount of digits that form each of the groups to be separated by thousands_sep separator for non-monetary quantities.
    || int_curr_symbol    | International currency symbol.
    || int_frac_digits    | Amount of fractional digits to the right of the decimal point for monetary quantities in the international format.
    || mon_decimal_point  | Decimal-point separator used for monetary quantities.
    || mon_grouping       | Specifies the amount of digits that form each of the groups to be separated by mon_thousands_sep separator for monetary quantities.
    || mon_thousands_sep  | Separators used to delimit groups of digits to the left of the decimal point for monetary quantities.
    || negative_sign      | Sign to be used for negative monetary quantities.
    || n_cs_precedes      | Whether the currency symbol should precede negative monetary quantities.
    || n_sep_by_space     | Whether a space should appear between the currency symbol and negative monetary quantities.
    || n_sign_posn        | Position of the sign for negative monetary quantities.
    || positive_sign      | Sign to be used for nonnegative (positive or zero) monetary quantities.
    || p_cs_precedes      | Whether the currency symbol should precede nonnegative (positive or zero) monetary quantities.
    || p_sep_by_space     | Whether a space should appear between the currency symbol and nonnegative (positive or zero) monetary quantities.
    || p_sign_posn        | Position of the sign for nonnegative (positive or zero) monetary quantities.
    || thousands_sep      | Separators used to delimit groups of digits to the left of the decimal point for non-monetary quantities.
  \endtable
*/
struct lconv
{
  /* Numeric, non-monetary */
  char *decimal_point;
  char *thousands_sep;
  char *grouping;

  /* Monetary */
  char *int_curr_symbol;
  char *currency_symbol;
  char *mon_decimal_point;
  char *mon_thousands_sep;
  char *mon_grouping;
  char *positive_sign;
  char *negative_sign;
  char int_frac_digits;
  char frac_digits;
  char p_cs_precedes;
  char p_sep_by_space;
  char n_cs_precedes;
  char n_sep_by_space;
  char p_sign_posn;
  char n_sign_posn;

  /* Extra monetary fields added by C99 */
  char int_p_cs_precedes;
  char int_n_cs_precedes;
  char int_p_sep_by_space;
  char int_n_sep_by_space;
  char int_p_sign_posn;
  char int_n_sign_posn;
};


/*! \brief Get current locale data \ingroup Functions \synopsis

  \description \b \this returns a pointer to a structure of type \b lconv with the
  corresponding values for the current locale filled in.
*/
struct lconv *localeconv(void);


/*! \brief Set Locale \ingroup Functions \synopsis

  \description \b \this sets the current locale. The \a category parameter can have the following values:

  \table{1.5in,*}
  || Name             | Locale affected
  || LC_ALL           | Entire locale
  || LC_COLLATE       | Affects \b strcoll and \b strxfrm
  || LC_CTYPE         | Affects character handling
  || LC_MONETARY      | Affects monetary formatting information
  || LC_NUMERIC       | Affects decimal-point character in I/O and string formatting operations
  || LC_TIME          | Affects \b strftime
  \endtable

  The \a locale parameter contains the name of a C locale to set or if \b NULL is passed the current locale
  is not changed.

  \rv \b \this returns the name of the current locale.
*/
char *setlocale(int __category, const char *__locale);

#ifdef __cplusplus
}
#endif

#endif
