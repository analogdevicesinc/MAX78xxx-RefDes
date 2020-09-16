// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __CTYPE_H
#define __CTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Is character alphabetic? \ingroup Classification functions \synopsis

  \desc \b \this returns true if the character \a c is alphabetic.
  That is, any character for which \b isupper or \b islower returns true
  is considered alphabetic in addition to any of the locale-specific set of
  alphabetic characters for which none of \b iscntrl, \b isdigit, \b ispunct,
  or \b isspace is true.

  In the \q C locale, \b \this returns nonzero (true) if and only if
  \b isupper or \b islower return true for value of the argument \a c. 
*/
int isalpha(int __c);


/*! \brief Is character an uppercase letter? \ingroup Classification functions \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument 
  \a c is an uppercase letter.
*/
int isupper(int __c);


/*! \brief Is character a lowercase letter? \ingroup Classification functions \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument 
  \a c is an lowercase letter. 
*/
int islower(int __c);


/*! \brief Is character a decimal digit? \ingroup Classification functions \synopsis

  \desc \b \this  returns nonzero (true) if and only if the value of the argument 
  \a c is a digit.
*/
int isdigit(int __c);


/*! \brief Is character a hexadecimal digit? \ingroup Classification functions \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument 
  \a c is a hexadecimal digit. 
*/
int isxdigit(int __c);


/*! \brief Is character a whitespace character? \ingroup Classification functions \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument 
  \a c is a standard white-space character. The standard white-space characters 
  are space (\tt{' '}), form feed (\tt{'\\f'}), new-line (\tt{'\\n'}), 
  carriage return (\tt{'\\r'}), horizontal tab (\tt{'\\t'}), and 
  vertical tab (\tt{'\v'}). 
*/
int isspace(int __c);


/*! \brief Is character a punctuation mark? \ingroup Classification functions \synopsis

  \desc \b \this returns nonzero (true) for every printing character for which 
  neither \b isspace nor \b isalnum is true. 
*/
int ispunct(int __c);


/*! \brief Is character alphanumeric? \ingroup Classification functions \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument 
  \a c is an alphabetic or numeric character.
*/
int isalnum(int __c);


/*! \brief Is character printable? \ingroup Classification functions \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument 
  \a c is any printing character including space (\tt{' '}). 
*/
int isprint(int __c);


/*! \brief Is character any printing character except space? \ingroup Classification functions \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument 
  \a c is any printing character except space (\tt{' '}). 
*/
int isgraph(int __c);


/*! \brief Is character a control? \ingroup Classification functions \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument 
  \a c is a control character. Control characters have values 0 through 31 
  and the single value 127. 
*/
int iscntrl(int __c);


/*! \brief Convert lowercase character to uppercase \ingroup Conversion functions \synopsis

  \desc \b \this converts a lowercase letter to a corresponding uppercase letter.
  
  If the argument \c is a character for which \b islower is true and there are
  one or more corresponding characters, as specified by the current locale, for
  which \b isupper is true, \b toupper returns one of the corresponding
  characters (always the same one for any given locale); otherwise, the argument
  is returned unchanged.
  
  Note that even though \b islower can return true for some characters, \b toupper
  may return that lowercase character unchanged as there are no corresponding
  uppercase characters in the locale.
*/
int toupper(int __c);


/*! \brief Convert uppercase character to lowercase \ingroup Conversion functions \synopsis

  \desc \b \this converts an uppercase letter to a corresponding lowercase letter. 
  
  If the argument \a c is a character for which \b isupper is true and there
  are one or more corresponding characters, as specified by the current locale,
  for which \b islower is true, the \b tolower function returns one of the
  corresponding characters (always the same one for any given locale);
  otherwise, the argument is returned unchanged.

  Note that even though \b isupper can return true for some characters, \b tolower
  may return that uppercase character unchanged as there are no corresponding
  lowercase characters in the locale.
*/
int tolower(int __c);


/*! \brief Is character a space or horizontal tab? \ingroup Classification functions \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument 
  \a c is either a space character (\tt{' '}) or the horizontal tab 
  character (\tt{'\\t'}).
*/
int isblank(int __c);  /* C99 */


#ifndef __EXCLUDE_POSIX

#include <__crossworks.h>

#ifndef __RAL_LOCALE_T_DEFINED
#define __RAL_LOCALE_T_DEFINED
typedef struct __locale_s *locale_t;
#endif

/*! \brief Is character alphabetic? \ingroup Classification functions (extended) \synopsis

  \desc \b \this returns nonzero (true) if and only if \b isupper or \b islower 
  return true for value of the argument \a c in locale \a loc.
*/
int isalpha_l(int __c, locale_t __loc);


/*! \brief Is character an uppercase letter? \ingroup Classification functions (extended) \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument 
  \a c is an uppercase letter in locale \a loc.
*/
int isupper_l(int __c, locale_t __loc);


/*! \brief Is character a lowercase letter? \ingroup Classification functions (extended) \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument 
  \a c is an lowercase letter in locale \a loc. 
*/
int islower_l(int __c, locale_t __loc);


/*! \brief Is character a decimal digit? \ingroup Classification functions (extended) \synopsis

  \desc \b \this  returns nonzero (true) if and only if the value of the argument 
  \a c is a decimal digit in locale \a loc. 
*/
int isdigit_l(int __c, locale_t __loc);


/*! \brief Is character a hexadecimal digit? \ingroup Classification functions (extended) \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument 
  \a c is a hexadecimal digit in locale \a loc. 
*/
int isxdigit_l(int __c, locale_t __loc);


/*! \brief Is character a whitespace character? \ingroup Classification functions (extended) \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument 
  \a c is a standard white-space character in in locale \a loc.. 
*/
int isspace_l(int __c, locale_t __loc);


/*! \brief Is character a punctuation mark? \ingroup Classification functions (extended) \synopsis

  \desc \b \this returns nonzero (true) for every printing character for which 
  neither \b isspace nor \b isalnum is true in in locale \a loc.
*/
int ispunct_l(int __c, locale_t __loc);


/*! \brief Is character alphanumeric? \ingroup Classification functions (extended) \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument 
  \a c is a alphabetic or numeric character in locale \a loc.
*/
int isalnum_l(int __c, locale_t __loc);


/*! \brief Is character printable? \ingroup Classification functions (extended) \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument 
  \a c is any printing character including space (\tt{' '}) in locale \a loc. 
*/
int isprint_l(int __c, locale_t __loc);


/*! \brief Is character any printing character except space? \ingroup Classification functions (extended) \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument 
  \a c is any printing character except space (\tt{' '}) in locale \a loc.
*/
int isgraph_l(int __c, locale_t __loc);


/*! \brief Is character a control character? \ingroup Classification functions (extended) \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument 
  \a c is a control character in locale \a loc. 
*/
int iscntrl_l(int __c, locale_t __loc);


/*! \brief Convert lowercase character to uppercase \ingroup Conversion functions (extended) \synopsis

  \desc \b \this converts a lowercase letter to a corresponding uppercase letter
  in locale \a loc. If the argument \a c is a character for which \b islower is true
  in locale \a loc, \b \this  returns the corresponding uppercase letter; otherwise,
  the argument is returned unchanged.
*/
int toupper_l(int __c, locale_t __loc);


/*! \brief Convert uppercase character to lowercase \ingroup Conversion functions (extended) \synopsis

  \desc \b \this converts an uppercase letter to a corresponding lowercase letter
  in locale \a loc.  If the argument \a c is a character for which \b isupper is true
  in locale \a loc, \b \this returns the corresponding lowercase letter; otherwise,
  the argument is returned unchanged. 
*/
int tolower_l(int __c, locale_t __loc);


/*! \brief Is character a space or horizontal tab? \ingroup Classification functions (extended) \synopsis

  \desc \b \this returns nonzero (true) if and only if the value of the argument 
  \a c is either a space character (\tt{' '}) or the horizontal tab 
  character (\tt{'\\t'}) in locale \a loc.
*/
int isblank_l(int __c, locale_t __loc);  /* C99 */

#endif

#ifdef __cplusplus
}
#endif

#endif
