// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __wctype_H
#define __wctype_H

#ifdef __cplusplus
extern "C" {
#endif

#define WEOF ((wint_t)~0U)

#ifndef __RAL_WINT_T_DEFINED
#define __RAL_WINT_T_DEFINED
typedef long wint_t;
#endif

typedef int wctrans_t;
typedef int wctype_t;


/*! \brief Is character alphabetic? \ingroup Classification functions \synopsis

  \desc \b \this returns true if the wide character \a c is alphabetic.
  Any character for which \b iswupper or \b iswlower returns true
  is considered alphabetic in addition to any of the locale-specific set of
  alphabetic characters for which none of \b iswcntrl, \b iswdigit, \b iswpunct,
  or \b iswspace is true.

  In the \q C locale, \b \this returns nonzero (true) if and only if
  \b iswupper or \b iswlower return true for the value of the argument \a c.
*/
int iswalpha(wint_t __c);


/*! \brief Is character alphanumeric? \ingroup Classification functions \synopsis

  \desc \b \this tests for any wide character for which \b iswalpha or
  \b iswdigit is true.
*/
int iswalnum(wint_t __c);


/*! \brief Is character blank? \ingroup Classification functions \synopsis

  \desc \b \this tests for any wide character that is a standard
  blank wide character or is one of a locale-specific set of wide characters
  for which \a iswspace is true and that is used to separate words within
  a line of text. The standard blank wide are space and horizontal tab.

  In the \q C locale, \b \this returns true only for the standard blank
  characters.
*/
int iswblank(wint_t __c);


/*! \brief Is character a control? \ingroup Classification functions \synopsis

  \desc \b \this tests for any wide character that is a control
  character.
*/
int iswcntrl(wint_t __c);


/*! \brief Is character a decimal digit? \ingroup Classification functions \synopsis

  \desc \b \this tests for any wide character that corresponds to a decimal-digit
  character.
*/
int iswdigit(wint_t __c);


/*! \brief Is character a control? \ingroup Classification functions \synopsis

  \desc \b \this tests for any wide character for which \b iswprint is true and
  \b iswspace is false.
*/
int iswgraph(wint_t __c);


/*! \brief Is character a lowercase letter? \ingroup Classification functions \synopsis

  \desc \b \this tests for any wide character that corresponds to a lowercase
  letter or is one of a locale-specific set of wide characters for which none of
  \b iswcntrl, \b iswdigit, \b iswpunct, or \b iswspace is true.
*/
int iswlower(wint_t __c);


/*! \brief Is character printable? \ingroup Classification functions \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument
  \a c is any printing character.
*/
int iswprint(wint_t __c);


/*! \brief Is character punctuation? \ingroup Classification functions \synopsis

  \desc \b \this tests for any printing wide character that is one of a
  locale-specific set of punctuation wide characters for which neither
  \b iswspace nor \b iswalnum is true.
*/
int iswpunct(wint_t __c);


/*! \brief Is character a whitespace character? \ingroup Classification functions \synopsis

  \desc \b \this tests for any wide character that corresponds to a locale-specific
  set of white-space wide characters for which none of \b iswalnum, \b iswgraph,
  or \b iswpunct is true.
*/
int iswspace(wint_t __c);


/*! \brief Is character an uppercase letter? \ingroup Classification functions \synopsis

  \desc \b \this tests for any wide character that corresponds to an uppercase
  letter or is one of a locale-specific set of wide characters for which none of
  \b iswcntrl, \b iswdigit, \b iswpunct, or \b iswspace is true.
*/
int iswupper(wint_t __c);


/*! \brief Is character a hexadecimal digit? \ingroup Classification functions \synopsis

  \desc \b \this tests for any wide character that corresponds to a
  hexadecimal digit.
*/
int iswxdigit(wint_t __c);


/*! \brief Determine character type \ingroup Classification functions \synopsis

  \desc \b \this determines whether the wide character \a c has the
  property described by \a t in the current locale.
*/
int iswctype(wint_t __c, wctype_t __t);


/*! \brief Construct character mapping \ingroup Conversion functions \synopsis

  \desc \b \this constructs a value of type \b wctrans_t that describes a
  mapping between wide characters identified by the string argument \a property.

  If \a property identifies a valid mapping of wide characters in the current locale,
  \b \this returns a nonzero value that is valid as the second argument to \a towctrans;
  otherwise, it returns zero.

  \task Note
  
  The only mappings supported are \tt{"tolower"} and \tt{"toupper"}.
*/
wctrans_t wctrans(const char *property);


/*! \brief Construct character class \ingroup Classification functions \synopsis

  \desc \b \this constructs a value of type \b wctype_t that describes a
  class of wide characters identified by the string argument \a property.

  If \a property identifies a valid class of wide characters in the current locale,
  \b \this returns a nonzero value that is valid as the second argument to \b iswctype;
  otherwise, it returns zero.

  \task Note
  
  The only mappings supported are \tt{"alnum"}, \tt{"alpha"},
  \tt{"blank"}, \tt{"cntrl"}, \tt{"digit"}, \tt{"graph"}, \tt{"lower"}, \tt{"print"},
  \tt{"punct"}, \tt{"space"}, \tt{"upper"}, and \tt{"xdigit"}.
*/
wctype_t wctype(const char *property);


/*! \brief Convert uppercase character to lowercase \ingroup Conversion functions \synopsis

  \desc \b \this converts an uppercase letter to a corresponding lowercase letter.

  If the argument \a c is a wide character for which \b iswupper is true and there
  are one or more corresponding wide characters, in the current locale, for
  which \b iswlower is true, \b \this returns one (and always the same one for any
  given locale) of the corresponding wide characters; otherwise, \a c is
  returned unchanged.
*/
wint_t towlower(wint_t __c);


/*! \brief Convert lowercase character to uppercase \ingroup Conversion functions \synopsis

  \desc \b \this converts a lowercase letter to a corresponding uppercase letter.

  If the argument \a c is a wide character for which \b iswlower is true and there
  are one or more corresponding wide characters, in the current current locale, for
  which \b iswupper is true, \b \this returns one (and always the same one for any
  given locale) of the corresponding wide characters; otherwise, \a c is
  returned unchanged.
*/
wint_t towupper(wint_t __c);


/*! \brief Translate character \ingroup Conversion functions \synopsis

  \desc \b \this maps the wide character \a c using the mapping described by
  \a t in the current locale.
*/
wint_t towctrans(wint_t __c, wctrans_t __t);

#ifndef __EXCLUDE_POSIX

#include <__crossworks.h>

#ifndef __RAL_LOCALE_T_DEFINED
#define __RAL_LOCALE_T_DEFINED
typedef struct __locale_s *locale_t;
#endif


/*! \brief Is character alphabetic? \ingroup Classification functions (extended) \synopsis

  \desc \b \this returns true if the wide character \a c is alphabetic
  in the locale \a loc.  Any character for which \b iswupper_l or \b iswlower_l
  returns true is considered alphabetic in addition to any of the
  locale-specific set of alphabetic characters for which none of
  \b iswcntrl_l, \b iswdigit_l, \b iswpunct_l, or \b iswspace_l is true.
*/
int iswalpha_l(wint_t __c, locale_t __loc);


/*! \brief Is character alphanumeric? \ingroup Classification functions (extended) \synopsis

  \desc \b \this tests for any wide character for which \b iswalpha_l or
  \b iswdigit_l is true in the locale \a loc.
*/
int iswalnum_l(wint_t __c, locale_t __loc);


/*! \brief Is character blank? \ingroup Classification functions (extended) \synopsis

  \desc \b \this tests for any wide character that is a standard
  blank wide character in the locale \a loc or is one of a locale-specific
  set of wide characters for which \a iswspace_l is true and that is used
  to separate words within a line of text. The standard blank wide are
  space and horizontal tab.
*/
int iswblank_l(wint_t __c, locale_t __loc);


/*! \brief Is character a control? \ingroup Classification functions (extended) \synopsis

  \desc \b \this tests for any wide character that is a control
  character in the locale \a loc.
*/
int iswcntrl_l(wint_t __c, locale_t __loc);


/*! \brief Is character a decimal digit? \ingroup Classification functions (extended) \synopsis

  \desc \b \this tests for any wide character that corresponds to a decimal-digit
  character in the locale \a loc.
*/
int iswdigit_l(wint_t __c, locale_t __loc);


/*! \brief Is character a control? \ingroup Classification functions (extended) \synopsis

  \desc \b \this tests for any wide character for which \b iswprint is true and
  \b iswspace is false in the locale \a loc.
*/
int iswgraph_l(wint_t __c, locale_t __loc);


/*! \brief Is character a lowercase letter? \ingroup Classification functions (extended) \synopsis

  \desc \b \this tests for any wide character that corresponds to a lowercase
  letter in the locale \a loc or is one of a locale-specific set of wide characters for
  which none of \b iswcntrl_l, \b iswdigit_l, \b iswpunct_l, or \b iswspace_l is true.
*/
int iswlower_l(wint_t __c, locale_t __loc);


/*! \brief Is character printable? \ingroup Classification functions (extended) \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument
  \a c is any printing character in the locale \a loc.
*/
int iswprint_l(wint_t __c, locale_t __loc);


/*! \brief Is character punctuation? \ingroup Classification functions (extended) \synopsis

  \desc \b \this tests for any printing wide character that is one of a
  locale-specific set of punctuation wide characters in locale \a loc for
  which neither \b iswspace_l nor \b iswalnum_l is true.
*/
int iswpunct_l(wint_t __c, locale_t __loc);


/*! \brief Is character a whitespace character? \ingroup Classification functions (extended) \synopsis

  \desc \b \this tests for any wide character that corresponds to a locale-specific
  set of white-space wide characters in the locale \a loc for which none of \b iswalnum,
  \b iswgraph_l, or \b iswpunct_l is true.
*/
int iswspace_l(wint_t __c, locale_t __loc);


/*! \brief Is character an uppercase letter? \ingroup Classification functions (extended) \synopsis

  \desc \b \this tests for any wide character that corresponds to an uppercase
  letter or is one of a locale-specific set of wide characters in the locale \a loc
  for which none of \b iswcntrl_l, \b iswdigit_l, \b iswpunct_l, or \b iswspace_l
  is true.
*/
int iswupper_l(wint_t __c, locale_t __loc);


/*! \brief Is character a hexadecimal digit? \ingroup Classification functions (extended) \synopsis

  \desc \b \this tests for any wide character that corresponds to a
  hexadecimal digit in the locale \a loc.
*/
int iswxdigit_l(wint_t __c, locale_t __loc);


/*! \brief Determine character type \ingroup Classification functions (extended) \synopsis

  \desc \b \this determines whether the wide character \a c has the
  property described by \a t in the locale \a loc.
*/
int iswctype_l(wint_t __c, wctype_t __t, locale_t __loc);


/*! \brief Convert uppercase character to lowercase \ingroup Conversion functions (extended) \synopsis

  \desc \b \this converts an uppercase letter to a corresponding lowercase letter
  in locale \a loc.

  If the argument \a c is a wide character for which \b iswupper_l is true and there
  are one or more corresponding wide characters, in the locale \a loc, for
  which \b iswlower_l is true, \b \this returns one (and always the same one for any
  given locale) of the corresponding wide characters; otherwise, \a c is
  returned unchanged.
*/
wint_t towlower_l(wint_t __c, locale_t __loc);


/*! \brief Convert lowercase character to uppercase \ingroup Conversion functions (extended) \synopsis

  \desc \b \this converts a lowercase letter to a corresponding uppercase letter
  in locale \a loc.

  If the argument \a c is a wide character for which \b iswlower_l is true and there
  are one or more corresponding wide characters, in the locale \a loc, for
  which \b iswupper_l is true, \b \this returns one (and always the same one for any
  given locale) of the corresponding wide characters; otherwise, \a c is
  returned unchanged.
*/
wint_t towupper_l(wint_t __c, locale_t __loc);


/*! \brief Construct character mapping \ingroup Conversion functions (extended) \synopsis

  \desc \b \this constructs a value of type \b wctrans_t that describes a
  mapping between wide characters identified by the string argument \a property
  in locale \a loc.

  If \a property identifies a valid mapping of wide characters in the locale \a loc,
  \b \this returns a nonzero value that is valid as the second argument to \a towctrans_l;
  otherwise, it returns zero.

  \task Note
  
  The only mappings supported are \tt{"tolower"} and \tt{"toupper"}.
*/
wctrans_t wctrans_l(const char *property, locale_t loc);


/*! \brief Translate character \ingroup Conversion functions (extended) \synopsis

  \desc \b \this maps the wide character \a c using the mapping described by
  \a t in the current locale.
*/
wint_t towctrans_l(wint_t __c, wctrans_t __t, locale_t __loc);


#endif

#ifdef __cplusplus
}
#endif

#endif
