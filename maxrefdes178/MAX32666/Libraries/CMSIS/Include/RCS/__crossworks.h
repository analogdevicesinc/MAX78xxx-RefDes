// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __crossworks_H
#define __crossworks_H

#ifdef __cplusplus
extern "C" {
#endif

// Melting pot for the CrossWorks C Library.  This is sparsely documented
// as it is internal and subject to change.

#if defined(__CROSSWORKS)

#define __RAL_SIZE_T unsigned
#define __RAL_SIZE_MAX 65535U
#define __RAL_PTRDIFF_T int

typedef unsigned char *__va_list;
#pragma intrinsic(__va_base)

#elif defined(__CROSSWORKS_ARM) || defined(__SES_ARM)

#define __THREAD __thread

#undef __RAL_SIZE_T
#ifdef __ARM_EABI__
  #define __RAL_SIZE_T unsigned
#else
  #define __RAL_SIZE_T unsigned long
#endif
#define __RAL_SIZE_MAX 4294967295UL

#define __RAL_PTRDIFF_T int
typedef __builtin_va_list __va_list;

#else

#include <stddef.h>

#define __RAL_SIZE_T size_t
#define __RAL_SIZE_MAX 4294967295UL

#endif

// Harvard machines need code-space access.
#ifndef __CODE
#if defined(__CROSSWORKS_AVR) || defined(__CROSSWORKS_MAXQ) || defined(__CROSSWORKS_MAXQ30)
#define __CODE __code
#else
#define __CODE
#endif

// MAXQ does not lend itself to threading in the slightest.
#if defined(__CROSSWORKS_MAXQ) || defined(__CROSSWORKS_MAXQ30)
#define __THREAD
#else
#define __THREAD __thread
#endif

#endif

/* Base classifications. */
#define __CTYPE_UPPER          0x01    /* upper case letter */
#define __CTYPE_LOWER          0x02    /* lower case letter */
#define __CTYPE_DIGIT          0x04    /* digit */
#define __CTYPE_SPACE          0x08    /* whitespace */
#define __CTYPE_PUNCT          0x10    /* punctuation character */
#define __CTYPE_CNTRL          0x20    /* control character */
#define __CTYPE_BLANK          0x40    /* space char */
#define __CTYPE_XDIGIT         0x80    /* hexadecimal digit */

/* Derived classifications. */
#define __CTYPE_ALPHA   (__CTYPE_UPPER | __CTYPE_LOWER)
#define __CTYPE_ALNUM   (__CTYPE_UPPER | __CTYPE_LOWER | __CTYPE_DIGIT)
#define __CTYPE_GRAPH   (__CTYPE_PUNCT | __CTYPE_UPPER | __CTYPE_LOWER | __CTYPE_DIGIT)
#define __CTYPE_PRINT   (__CTYPE_BLANK | __CTYPE_PUNCT | __CTYPE_UPPER | __CTYPE_LOWER | __CTYPE_DIGIT)

#if defined(__SHORT_WCHAR) || __SIZEOF_WCHAR_T == 2
#define __RAL_WCHAR_T unsigned short
#else
#define __RAL_WCHAR_T unsigned
#endif

struct __mbstate_s {
  int __state;
  long __wchar;
};

typedef int (__RAL_mb_encode_t)(char *s, __RAL_WCHAR_T wc, struct __mbstate_s *codec);
typedef int (__RAL_mb_decode_t)(__RAL_WCHAR_T *pwc, const char *s, __RAL_SIZE_T len, struct __mbstate_s *codec);

typedef struct {
  // Defines for this locale, follow struct lconv.
  const char *decimal_point;
  const char *thousands_sep;
  const char *grouping;

  const char *int_curr_symbol;
  const char *currency_symbol;
  const char *mon_decimal_point;
  const char *mon_thousands_sep;
  const char *mon_grouping;
  const char *positive_sign;
  const char *negative_sign;

  char int_frac_digits;
  char frac_digits;
  char p_cs_precedes;
  char p_sep_by_space;
  char n_cs_precedes;
  char n_sep_by_space;
  char p_sign_posn;
  char n_sign_posn;
  char int_p_cs_precedes;
  char int_n_cs_precedes;
  char int_p_sep_by_space;
  char int_n_sep_by_space;
  char int_p_sign_posn;
  char int_n_sign_posn;

  // Pointer to null-terminated list of full day names, e.g. using string concatenation:
  //  "Sun\0" "Mon\0" "Tue\0" "Wed\0" "Thu\0" "Fri\0" "Sat\0" "\0"  /*final terminator*/

  const char *day_names;
  const char *abbrev_day_names;  // Pointer to null-terminated list of full day names.
  const char *month_names;
  const char *abbrev_month_names;  // Pointer to null-terminated list of full day names.
  const char *am_pm_indicator;     // For %p in strftime, taken from am_pm in FDCC specification 
  const char *date_format;         // For %x in strftime, taken from d_fmt in FDCC specification
  const char *time_format;         // For %X in strftime, taken from t_fmt in FDCC specification
  const char *date_time_format;    // For %c in strftime, taken from d_t_fmt in FDCC specification
} __RAL_locale_data_t;

enum {
  __RAL_WC_ALNUM = 1,
  __RAL_WC_ALPHA,
  __RAL_WC_CNTRL,
  __RAL_WC_DIGIT,
  __RAL_WC_GRAPH,
  __RAL_WC_LOWER,
  __RAL_WC_UPPER,
  __RAL_WC_SPACE,
  __RAL_WC_PRINT,
  __RAL_WC_PUNCT,
  __RAL_WC_BLANK,
  __RAL_WC_XDIGIT
};

enum {
  __RAL_WT_TOLOWER = 1,
  __RAL_WT_TOUPPER
};

typedef struct {
  // Narrow character functions
  int (*__isctype)(int, int);
  int (*__toupper)(int);
  int (*__tolower)(int);

  // Wide character functions
  int (*__iswctype)(long, int);
  long (*__towupper)(long);
  long (*__towlower)(long);

  // Conversion between multi-byte and wide characters.
  int (*__wctomb)(char *s, __RAL_WCHAR_T wc, struct __mbstate_s *state);
  int (*__mbtowc)(__RAL_WCHAR_T *pwc, const char *s, __RAL_SIZE_T len, struct __mbstate_s *state);
} __RAL_locale_codeset_t;

typedef struct {
  const char *name;
  const __CODE __RAL_locale_data_t *data;
  const __CODE __RAL_locale_codeset_t *codeset;
} __RAL_locale_t;

#define __MAX_CATEGORY 5  // Categories 1-5

typedef struct __locale_s {
  const __CODE __RAL_locale_t *__category[__MAX_CATEGORY];  // This corresponds directly to the LC_* categories minus LC_ALL!
} *__locale_t;

// This is what the user provides; a weak implementation returns zero.
const __CODE __RAL_locale_t *__user_find_locale(const char *locale);


/*! \brief Find a named locale \ingroup Locale functions \synopsis

  \desc \b \this finds the locale named \a locale.  The names \q C and \q POSIX
  are reserved and take preference over any user-defined locale.  If \a locale
  is neither \b C nor \b POSIX, \b \this calls the function \b __user_find_locale
  with \a locale as a parameter so the user can provide their own locales.

  The default implementation of \b __user_find_locale in the library returns
  a null pointer.  Any implementation of \b __user_find_locale in the main
  application will replace the implementation in the library.
*/
const __CODE __RAL_locale_t *__RAL_find_locale(const char *locale);


/*! \brief Retrieve a global locale category \ingroup Locale functions \synopsis

  \desc \b \this retrieves the locale associated with the category \a category
  from the global locale.
*/
const __CODE __RAL_locale_t *__RAL_global_locale_category(int __category);


/*! \brief Retrieve a locale category \ingroup Locale functions \synopsis

  \desc \b \this retrieves the locale associated with the category \a category
  from the locale \a locale.
*/
const __CODE __RAL_locale_t *__RAL_locale_category(__locale_t __locale, int __category);


/*! \brief Global locale for the application \ingroup Static data \synopsis

  \desc \b \this holds the global locale for the application.
*/
extern struct __locale_s __RAL_global_locale;


/*! \brief The standard C locale \ingroup Static data \synopsis

  \desc \b \this holds the definition of the C and POSIX locales which are equivalent.
*/
extern const __CODE __RAL_locale_t __RAL_c_locale;

// Built-in codesets.
extern const __CODE __RAL_locale_codeset_t __RAL_codeset_ascii;
extern const __CODE __RAL_locale_codeset_t __RAL_codeset_utf8;

extern const __CODE unsigned char __RAL_ascii_ctype_map[128];

extern const char __RAL_c_locale_day_names[];
extern const char __RAL_c_locale_abbrev_day_names[];
extern const char __RAL_c_locale_month_names[];
extern const char __RAL_c_locale_abbrev_month_names[];

extern const char __RAL_data_utf8_period[];
extern const char __RAL_data_utf8_comma[];
extern const char __RAL_data_utf8_space[];
extern const char __RAL_data_utf8_plus[];
extern const char __RAL_data_utf8_minus[];
extern const char __RAL_data_empty_string[];

const char *__RAL_string_list_decode(const char *str, int index);
int __RAL_string_list_encode(const char *list, const char *str);

// UTF-8 codec.
void __RAL_init_mbstate(struct __mbstate_s *state);

int __RAL_ascii_wctomb(char *s, __RAL_WCHAR_T wc, struct __mbstate_s *state);
int __RAL_ascii_mbtowc(__RAL_WCHAR_T *pwc, const char *s, __RAL_SIZE_T len, struct __mbstate_s *state);

int __RAL_utf8_wctomb(char *s, __RAL_WCHAR_T wc, struct __mbstate_s *state);
int __RAL_utf8_mbtowc(__RAL_WCHAR_T *pwc, const char *s, __RAL_SIZE_T len, struct __mbstate_s *state);

int __RAL_mb_max(const struct __locale_s *loc);

struct timeval;

// Match locale names str0 and str1; if match, return zero (rather like strcmp).
int __RAL_compare_locale_name(const char *str0, const char *str1);

// C runtime drivers to get and set the time of day from a RTC.
extern int (*__user_set_time_of_day)(const struct timeval *tp);
extern int (*__user_get_time_of_day)(struct timeval *tp);

// Internal structures used for case mapping.
typedef struct { unsigned short min, max, map; } __RAL_unicode_map_bmp_range_t;
typedef struct { unsigned short cp, map; } __RAL_unicode_map_bmp_singleton_t;

// Internal structures for predicate testing.
typedef struct { unsigned short min, max; } __RAL_unicode_set_bmp_range_t;
typedef struct { long min, max; } __RAL_unicode_set_nonbmp_range_t;

// Unicode wide character support.
int __RAL_unicode_iswctype(long ch, int ty);
long __RAL_unicode_towupper(long ch);
long __RAL_unicode_towlower(long ch);

// Internal functions for case mapping.
int __RAL_unicode_map_range_search(const void *k0, const void *k1);
int __RAL_unicode_map_singleton_search(const void *k0, const void *k1);

// Internal functions for predicate testing.
int __RAL_unicode_set_bmp_range_search(const void *k0, const void *k1);
int __RAL_unicode_set_nonbmp_range_search(const void *k0, const void *k1);

typedef const char * (*__RAL_error_decoder_fn_t)(int error);

typedef struct __RAL_error_decoder_s
{
  __RAL_error_decoder_fn_t decode;
  struct __RAL_error_decoder_s *next;
} __RAL_error_decoder_t;

void __RAL_register_error_decoder(__RAL_error_decoder_t *decoder);

extern __RAL_error_decoder_t *__RAL_error_decoder_head;

const char *__RAL_decode_error(int num);

#ifdef __cplusplus
}
#endif

#endif
