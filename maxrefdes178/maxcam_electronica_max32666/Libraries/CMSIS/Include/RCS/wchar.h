// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __wchar_H
#define __wchar_H

#include "__crossworks.h"

#ifdef __cplusplus
extern "C" {
#endif


// Some things are defined here, as required by the ISO standard, but
// we do not include <stddef.h> to get them as that would be in violation
// of the standard.

struct tm;

#ifndef __RAL_SIZE_T_DEFINED
#define __RAL_SIZE_T_DEFINED
typedef __RAL_SIZE_T size_t;
#endif

#ifndef __RAL_LOCALE_T_DEFINED
#define __RAL_LOCALE_T_DEFINED
typedef struct __locale_s *locale_t;
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef __RAL_FILE_DEFINED
#define __RAL_FILE_DEFINED
typedef struct __RAL_FILE FILE;
#endif


// Deal with different sizes of wchar_t.
#if __SIZEOF_WCHAR_T==2

#define WCHAR_MIN  0U
#define WCHAR_MAX  65535U

#else

#define WCHAR_MIN  (-2147483647L-1)
#define WCHAR_MAX  2147483647L

#endif


/*! \brief End of file indication \ingroup Constants \synopsis

  \desc \b \this expands to a constant value that does not correspond to any
  character in the wide character set.  It is typically used to indicate an
  end of file condition.
*/
#define WEOF ((wint_t)~0U)


/*! \brief Conversion state context \ingroup Types \synopsis

  \desc \b \this holds the conversion state information necessary to
  convert between sequences of multi-byte characters and wide
  characters.
*/
typedef struct __mbstate_s mbstate_t;


#if !defined(__RAL_WCHAR_T_DEFINED)
#define __RAL_WCHAR_T_DEFINED

/*! \brief Wide character type \ingroup Types \synopsis

  \desc \b \this holds a single wide character.

  Depending on implementation you can control whether \a wchar_t is 
  represented by a short 16-bit type or the standard 32-bit type.  
*/
typedef __RAL_WCHAR_T wchar_t;

#endif


#ifndef __RAL_WINT_T_DEFINED
#define __RAL_WINT_T_DEFINED

/*! \brief Wide integer type \ingroup Types \synopsis

  \desc \b \this is an integer type that is unchanged by default argument promotions
  that can hold any value corresponding to members of the extended character set,
  as well as at least one value that does not correspond to any member of the extended
  character set (WEOF).
*/
typedef long wint_t;

#endif


/*! \brief Copy memory \ingroup Copying functions \synopsis

  \description \b \this copies \a n wide characters from the object pointed to by \a s2
  into the object pointed to by \a s1. The behavior of \b \this is undefined 
  if copying takes place between objects that overlap.

  \b \this returns the value of \a s1.
*/
wchar_t *wmemcpy(wchar_t *__s1, const wchar_t *__s2, size_t __n);


/*! \brief Copy memory with specified terminator (POSIX extension) \ingroup Copying functions \synopsis

  \description \b \this copies at most \a n wide characters from the object pointed to
  by \a s2 into the object pointed to by \a s1. The copying stops as soon as
  \a n wide characters are copied or the wide character \a c is copied into the
  destination object pointed to by \a s1.
  
  The behavior of \b \this is undefined if copying takes place between objects
  that overlap.

  \b \this returns a pointer to the wide character immediately following \a c in
  \a s1, or \b NULL if \a c was not found in the first \a n wide characters of \a s2.

  \note \b \this conforms to POSIX.1-2008.
*/
wchar_t *wmemccpy(wchar_t *__s1, const wchar_t *__s2, wchar_t __c, size_t __n);


/*! \brief Copy memory (GNU extension) \ingroup Copying functions \synopsis

  \description \b \this copies \a n wide characters from the object pointed to by \a s2
  into the object pointed to by \a s1. The behavior of \b \this is undefined 
  if copying takes place between objects that overlap.

  \b \this returns it returns a pointer to the wide character following the
  last written wide character.

  \note This is an extension found in GNU libc.
*/
wchar_t *wmempcpy(wchar_t *__s1, const wchar_t *__s2, size_t __n);


/*! \brief Safely copy overlapping memory \ingroup Copying functions \synopsis

  \description \b \this copies \a n wide characters from the object pointed to by \a s2
  into the object pointed to by \a s1 ensuring that if \a s1 and \a s2
  overlap, the copy works correctly. Copying takes place as if the \a n wide characters 
  from the object pointed to by \a s2 are first copied into a temporary array 
  of \a n wide characters that does not overlap the objects pointed to by \a s1 
  and \a s2, and then the \a n wide characters from the temporary array are 
  copied into the object pointed to by \a s1.

  \b \this returns the value of \a s1.
*/
wchar_t *wmemmove(wchar_t *__s1, const wchar_t *__s2, size_t __n);


/*! \brief Compare memory \ingroup Comparison functions \synopsis

  \description \b \this compares the first \a n wide characters of the object pointed 
  to by \a s1 to the first \a n wide characters of the object pointed to by 
  \a s2. \b \this returns an integer greater than, equal to, or less 
  than zero as the object pointed to by \a s1 is greater than, equal to, or 
  less than the object pointed to by \a s2.
*/
int wmemcmp(const wchar_t *__s1, const wchar_t *__s2, size_t __n);


/*! \brief Search memory for a wide character \ingroup Search functions \synopsis

  \description \b \this locates the first occurrence of \a c in the initial
  \a n characters of the object pointed to by \a s.  Unlike \b wcschr, \b \this
  does \em not terminate a search when a null wide character is found in the object 
  pointed to by \a s.

  \b \this returns a pointer to the located wide character, or a null pointer 
  if \a c does not occur in the object.
*/
wchar_t *wmemchr(const wchar_t *__s, wchar_t __c, size_t __n);


/*! \brief Set memory to wide character \ingroup Miscellaneous functions \synopsis

  \desc \b \this copies the value of \a c into each of the first \b n
  wide characters of the object pointed to by \a s. 

  \b \this returns the value of \a s.
*/
wchar_t *wmemset(wchar_t *__s, wchar_t __c, size_t __n);


/*! \brief Copy string \ingroup Copying functions \synopsis

  \desc \b \this copies the wide string pointed to by \a s2 (including the terminating 
  null wide character) into the array pointed to by \a s1. The behavior of \b \this
  is undefined if copying takes place between objects that overlap.

  \b \this returns the value of \a s1.
*/
wchar_t *wcscpy(wchar_t *__s1, const wchar_t *__s2);


/*! \brief Copy string up to a maximum length \ingroup Copying functions \synopsis

  \desc \b \this copies not more than \a n wide characters from the array pointed 
  to by \a s2 to the array pointed to by \a s1. Wide characters that follow 
  a null wide character in \a s2 are not copied. The behavior of \b \this
  is undefined if copying takes place between objects that overlap. If the array 
  pointed to by \a s2 is a wide string that is shorter than \a n wide characters, 
  null wide characters are appended to the copy in the array pointed to by \a s1, 
  until \a n characters in all have been written.

  \b \this returns the value of \a s1.
*/
wchar_t *wcsncpy(wchar_t *__s1, const wchar_t *__s2, size_t __n);


/*! \brief Concatenate strings \ingroup Copying functions \synopsis

  \desc \b \this appends a copy of the wide string pointed to by \a s2 (including 
  the terminating null wide character) to the end of the wide string pointed to by \a s1.
  The initial character of \a s2 overwrites the null wide character at the end 
  of \a s1. The behavior of \b \this is undefined if copying takes place 
  between objects that overlap.

  \b \this returns the value of \a s1.
*/
wchar_t *wcscat(wchar_t *__s1, const wchar_t *__s2);


/*! \brief Concatenate strings up to maximum length \ingroup Copying functions \synopsis

  \desc \b \this appends not more than \a n wude characters from the array pointed 
  to by \a s2 to the end of the wide string pointed to by \a s1. A null wide character 
  in \a s1 and wide characters that follow it are not appended. The initial wide character 
  of \a s2 overwrites the null wide character at the end of \a s1. A terminating 
  wide null character is always appended to the result. The behavior of \b \this
  is undefined if copying takes place between objects that overlap.

  \b \this returns the value of \a s1.
*/
wchar_t *wcsncat(wchar_t *__s1, const wchar_t *__s2, size_t __n);


/*! \brief Compare strings \ingroup Comparison functions \synopsis

  \desc \b \this compares the wide string pointed to by \a s1 to the wide string pointed 
  to by \a s2. \b \this returns an integer greater than, equal to, or 
  less than zero if the wide string pointed to by \a s1 is greater than, equal 
  to, or less than the wide string pointed to by \a s2.
*/
int wcscmp(const wchar_t *__s1, const wchar_t *__s2);


/*! \brief Compare strings up to a maximum length \ingroup Comparison functions \synopsis

  \desc \b \this compares not more than \a n wide characters from the array pointed 
  to by \a s1 to the array pointed to by \a s2. Characters that follow 
  a null wide character are not compared.

  \b \this returns an integer greater than, equal to, or less than zero, 
  if the possibly null-terminated array pointed to by \a s1 is greater than, 
  equal to, or less than the possibly null-terminated array pointed to by \a s2.
*/
int wcsncmp(const wchar_t *__s1, const wchar_t *__s2, size_t __n);


/*! \brief Find character within string \ingroup Search functions \synopsis

  \desc \b \this locates the first occurrence of \a c in the wide
  string pointed to by \a s. The terminating wide null character 
  is considered to be part of the string.

  \b \this returns a pointer to the located wide character, or a null pointer 
  if \a c does not occur in the string.
*/
wchar_t *wcschr(const wchar_t *__s, wchar_t __c);


/*! \brief Find character in a length-limited string \ingroup Search functions \synopsis

  \desc \b \this searches not more than \a n wide characters to locate the
  first occurrence of \a c in the wide string pointed to by \a s. The terminating
  wide null character is considered to be part of the wide string.

  \b \this returns a pointer to the located wide character, or a null pointer 
  if \a c does not occur in the string.
*/
wchar_t *wcsnchr(const wchar_t *__str, size_t __n, wchar_t __ch);


/*! \brief Compute size of string not prefixed by a set of characters \ingroup Search functions \synopsis

  \desc \b \this computes the length of the maximum initial segment of the wide string
  pointed to by \a s1 which consists entirely of wide characters not from the wide string
  pointed to by \a s2.

  \b \this returns the length of the segment.
*/
size_t wcscspn(const wchar_t *__s1, const wchar_t *__s2);


/*! \brief Find first occurrence of characters within string \ingroup Search functions \synopsis

  \desc \b \this locates the first occurrence in the wide string pointed to by \a s1
  of any wide character from the wide string pointed to by \a s2.

  \b \this returns a pointer to the wide character, or a null pointer if no wide character 
  from \a s2 occurs in \a s1.
*/
wchar_t *wcspbrk(const wchar_t *__s1, const wchar_t *__s2);


/*! \brief Find last occurrence of character within string \ingroup Search functions \synopsis

  \desc \b \this locates the last occurrence of \a c in the wide string pointed to
  by \a s. The terminating wide null character is considered to be part of the string.

  \b \this returns a pointer to the wide character, or a null pointer if \a c
  does not occur in the wide string.
*/
wchar_t *wcsrchr(const wchar_t *__s, wchar_t __c);


/*! \brief Compute size of string prefixed by a set of characters \ingroup Search functions \synopsis

  \desc \b \this computes the length of the maximum initial segment of the wide string 
  pointed to by \a s1 which consists entirely of wide characters from the wide string
  pointed to by \a s2.

  \b \this returns the length of the segment.
*/
size_t wcsspn(const wchar_t *__s1, const wchar_t *__s2);


/*! \brief Find first occurrence of a string within string \ingroup Search functions \synopsis

  \desc \b \this locates the first occurrence in the wide string pointed to by \a s1
  of the sequence of wide characters (excluding the terminating null wide character) in 
  the wide string pointed to by \a s2.

  \b \this returns a pointer to the located wide string, or a null pointer if 
  the wide string is not found. If \a s2 points to a wide string with zero length, \b \this
  returns \a s1.
*/
wchar_t *wcsstr(const wchar_t *__s1, const wchar_t *__s2);


/*! \brief Find first occurrence of a string within length-limited string \ingroup Search functions \synopsis

  \desc \b \this searches at most \a n wide characters to locate the first occurrence
  in the wide string pointed to by \a s1 of the sequence of wide characters (excluding the
  terminating null wide character) in the wide string pointed to by \a s2.

  \b \this returns a pointer to the located string, or a null pointer if 
  the string is not found. If \a s2 points to a string with zero length, \b \this
  returns \a s1.

  \note \b \this is an extension commonly found in Linux and BSD C libraries.
*/
wchar_t *wcsnstr(const wchar_t *__s1, const wchar_t *__s2, size_t __n);


/*! \brief Calculate length of string \ingroup Miscellaneous functions \synopsis

  \desc \b \this returns the length of the wide string pointed to by \a s, that 
  is the number of wide characters that precede the terminating null wide character.
*/
size_t wcslen(const wchar_t *__s);


/*! \brief Calculate length of length-limited string \ingroup Search functions \synopsis

  \desc \b this returns the length of the wide string pointed to by \a s, up
  to a maximum of \a n wide characters.  \b \this only examines the first \a n wide characters
  of the string \a s.

  \note \b \this is an extension commonly found in Linux and BSD C libraries.
*/
size_t wcsnlen(const wchar_t *__s, size_t __n);


/*! \brief Break string into tokens \ingroup Search functions \synopsis

  \desc \b \this A sequence of calls to \b \this breaks the wide string pointed to by \a s1 
  into a sequence of tokens, each of which is delimited by a wide character from the 
  wide string pointed to by \a s2. The first call in the sequence has a non-null 
  first argument; subsequent calls in the sequence have a null first argument. 
  The separator wide string pointed to by \a s2 may be different from call to call. 

  The first call in the sequence searches the wide string pointed to by \a s1
  for the first wide character that is not contained in the current separator wide string
  pointed to by \a s2. If no such wide character is found, then there are no tokens 
  in the wide string pointed to by \a s1 and \b \this returns a null pointer. 
  If such a wide character is found, it is the start of the first token.

  \b \this then searches from there for a wide character that is contained in 
  the current wide separator string. If no such wide character is found, the current token 
  extends to the end of the wide string pointed to by \a s1, and subsequent searches 
  for a token will return a null pointer. If such a wude character is found, it is 
  overwritten by a wide null character, which terminates the current token. \b \this
  saves a pointer to the following wide character, from which the next search for a 
  token will start.

  Each subsequent call, with a null pointer as the value of the first argument, 
  starts searching from the saved pointer and behaves as described above.

  \note \b \this maintains static state and is therefore not reentrant and
  not thread safe.  See \ref wcstok_r for a thread-safe and reentrant
  variant.
*/
wchar_t *wcstok(wchar_t *__s1, const wchar_t *__s2);


/*! \brief Break string into tokens \ingroup Search functions \synopsis

  \desc \b \this locates, in the wide string referenced by \a{*stringp}, the first
  occurrence of any wide character in the wide string \a delim (or the terminating wide null
  character) and replaces it with a wide null character.  The location of the next character
  after the delimiter wide character (or NULL, if the end of the string was reached) is stored
  in \a{*stringp}.  The original value of \a{*stringp} is returned. 

  An empty field (that is, a wide character in the string \a delim occurs as the
  first wide character of \a{*stringp} can be detected by comparing the location
  referenced by the returned pointer to a wide null character. 

  If \a{*stringp} is initially null, \b \this returns null.
  
  \note \b \this is not an ISO C function, but appears in BSD4.4 and Linux.
*/
wchar_t *wstrsep(wchar_t **__stringp, const wchar_t *__delim);


/*! \brief Break string into tokens (reentrant version) \ingroup Search functions \synopsis

  \desc \b \this is a reentrant version of the function \b wcstok where the state is
  maintained in the object of type \b{wchar_t *} pointed to by \a s3.

  \note \b \this is an extension commonly found in Linux and BSD C libraries.

  \sa \ref wcstok.
*/
wchar_t *wcstok_r(wchar_t *__s1, const wchar_t *__s2, wchar_t **__s3);


/*! \brief Duplicate string \ingroup Miscellaneous functions \synopsis

  \desc \b \this duplicates the wide string pointed to by \a s1 by using \b malloc to
  allocate memory for a copy of \a s and then copying \a s, including the
  terminating wide null character, to that memory. The returned pointer can be
  passed to \b free.
  
  \b \this returns a pointer to the new wide string or a null pointer if the
  new string cannot be created.

  \note \b \this is an extension commonly found in Linux and BSD C libraries.
*/
wchar_t *wcsdup(const wchar_t *__s1);


/*! \brief Query conversion state
  \ingroup Multi-byte/wide string conversion functions \synopsis

  \desc \b \this function returns nonzero if \a ps is a null pointer or
  if the pointed-to object describes an initial conversion state; otherwise,
  \b \this returns zero.
*/
int msbinit(const mbstate_t *__ps);


/*! \brief Convert wide character to single-byte character
  \ingroup Multi-byte/wide string conversion functions \synopsis

  \desc \b \this determines whether \a c corresponds to a member of the
  extended character set whose multi-byte character representation is
  a single byte when in the initial shift state in the current locale.

  \desc \b this returns \b EOF if \a c does not correspond to a
  multi-byte character with length one in the initial shift state.
  Otherwise, it returns the single-byte representation of that
  character as an \b{unsigned char} converted to an \b int.
*/
int wctob(wint_t __c);


/*! \brief Convert wide character to single-byte character
  \ingroup Multi-byte/wide string conversion functions \synopsis

  \desc \b \this determines whether \a c corresponds to a member of the
  extended character set whose multi-byte character representation is
  a single byte when in the initial shift state in locale \a loc.

  \desc \b \this returns \b EOF if \a c does not correspond to a
  multi-byte character with length one in the initial shift state.
  Otherwise, it returns the single-byte representation of that
  character as an \b{unsigned char} converted to an \b int.
*/
int wctob_l(wint_t c, locale_t __loc);


/*! \brief Determine number of bytes in a multi-byte character
  \ingroup Multi-byte to wide character conversions \synopsis

  \note \b \this function is equivalent to the call:

  \code mbrtowc(NULL, s, n, ps != NULL ? ps : &internal); \endcode

  where \b internal is the \b mbstate_t object for the \b mbrlen function,
  except that the expression designated by \a ps is evaluated only once.
*/
size_t mbrlen(const char *__s, size_t __n, mbstate_t *__ps);


/*! \brief Determine number of bytes in a multi-byte character
  \ingroup Multi-byte to wide character conversions \synopsis

  \note \b \this function is equivalent to the call:

  \code mbrtowc_l(NULL, s, n, ps != NULL ? ps : &internal, loc); \endcode

  where \b internal is the \b mbstate_t object for the \b mbrlen function,
  except that the expression designated by \a ps is evaluated only once.
*/
size_t mbrlen_l(const char *__s, size_t __n, mbstate_t *__ps, locale_t __loc);


/*! \brief Convert multi-byte string to wide character string
  \ingroup Multi-byte to wide character conversions \synopsis

  \desc \b \this converts a sequence of multi-byte characters that begins
  in the conversion state described by the object pointed to by \a ps,
  from the array indirectly pointed to by \a src into a sequence of
  corresponding wide characters
  
  If \a dst is not a null pointer, the converted characters are stored
  into the array pointed to by \a dst.  Conversion continues up to and
  including a terminating null character, which is also stored.

  Conversion stops earlier in two cases: when a sequence of bytes is
  encountered that does not form a valid multi-byte character, or
  (if \a dst is not a null pointer) when \a len wide characters have been
  stored into the array pointed to by \a dst.  Each conversion takes place
  as if by a call to the \b mbrtowc function.

  If \a dst is not a null pointer, the pointer object pointed to by
  \a src is assigned either a null pointer (if conversion stopped due to
  reaching a terminating null character) or the address just past the
  last multi-byte character converted (if any). If conversion stopped
  due to reaching a terminating null character and if \a dst is not a null
  pointer, the resulting state described is the initial conversion state.

  \sa \ref mbsrtowcs_l, \ref mbrtowc
*/
size_t mbsrtowcs(wchar_t *__dst, const char **__src, size_t __len, mbstate_t *__ps);


/*! \brief Convert multi-byte string to wide character string
  \ingroup Multi-byte to wide character conversions \synopsis

  \desc \b \this converts a sequence of multi-byte characters that begins
  in the conversion state described by the object pointed to by \a ps,
  from the array indirectly pointed to by \a src into a sequence of
  corresponding wide characters
  
  If \a dst is not a null pointer, the converted characters are stored
  into the array pointed to by \a dst.  Conversion continues up to and
  including a terminating null character, which is also stored.

  Conversion stops earlier in two cases: when a sequence of bytes is
  encountered that does not form a valid multi-byte character, or
  (if \a dst is not a null pointer) when \a len wide characters have been
  stored into the array pointed to by \a dst.  Each conversion takes place
  as if by a call to the \b mbrtowc function.

  If \a dst is not a null pointer, the pointer object pointed to by
  \a src is assigned either a null pointer (if conversion stopped due to
  reaching a terminating null character) or the address just past the
  last multi-byte character converted (if any). If conversion stopped
  due to reaching a terminating null character and if \a dst is not a null
  pointer, the resulting state described is the initial conversion state.

  \sa \ref mbsrtowcs_l, \ref mbrtowc
*/
size_t mbsrtowcs_l(wchar_t *__dst, const char **__src, size_t __len, mbstate_t *__ps, locale_t __loc);


/*! \brief Convert single-byte character to wide character
  \ingroup Single-byte to wide character conversions \synopsis

  \desc \b \this function determines whether \a c constitutes a valid
  single-byte character.  If \a c is a valid single-byte character,
  \b \this  returns the wide character representation of that
  character

  \b \this returns WEOF if \a c has the value \a EOF or if
  \tt{(unsigned char)c} does not constitute a valid single-byte
  character in the initial shift state.
*/
wint_t btowc(int __c);


/*! \brief Convert single-byte character to wide character
  \ingroup Single-byte to wide character conversions \synopsis

  \desc \b \this function determines whether \a c constitutes a valid
  single-byte character in the locale \a loc.  If \a c is a valid
  single-byte character, \b \this  returns the wide character
  representation of that character

  \b \this returns WEOF if \a c has the value \a EOF or if
  \tt{(unsigned char)c} does not constitute a valid single-byte
  character in the initial shift state.
*/
wint_t btowc_l(int __c, locale_t __loc);


/*! \brief Convert multi-byte character to wide character
  \ingroup Multi-byte/wide string conversion functions \synopsis

  \desc \b \this converts a single multi-byte character to a wide
  character in the current locale.

  If \a s is a null pointer, \b \this is equivalent to
  \tt{mbrtowc(NULL, "", 1, ps)}, ignoring \a pwc and \a n.

  If \a s is not null and the object that \a s points to is a
  wide-character null character, \b \this returns 0.

  If \a s is not null and the object that \s points to forms a valid
  multi-byte character with a most \a n bytes, \b \this returns the
  length in bytes of the multi-byte character and stores that wide
  character to the object pointed to by \a pwc (if \a pwc is not null).

  If the object that \s points to forms an incomplete, but possibly
  valid, multi-byte character, \b \this returns \minus~2.
  
  If the object that \s points to does not form a partial multi-byte
  character, \b \this returns \minus~1.

  \sa \ref mbtowc, \ref mbrtowc_l
*/
size_t mbrtowc(wchar_t *__pwc, const char *__s, size_t __n, mbstate_t *__ps);


/*! \brief Convert multi-byte character to wide character
  \ingroup Multi-byte/wide string conversion functions \synopsis

  \desc \b \this converts a single multi-byte character to a wide
  character in the locale \a loc.

  If \a s is a null pointer, \b \this is equivalent to
  \tt{mbrtowc(NULL, "", 1, ps)}, ignoring \a pwc and \a n.

  If \a s is not null and the object that \a s points to is a
  wide-character null character, \b \this returns 0.

  If \a s is not null and the object that \s points to forms a valid
  multi-byte character with a most \a n bytes, \b \this returns the
  length in bytes of the multi-byte character and stores that wide
  character to the object pointed to by \a pwc (if \a pwc is not null).

  If the object that \s points to forms an incomplete, but possibly
  valid, multi-byte character, \b \this returns \minus~2.
  
  If the object that \s points to does not form a partial multi-byte
  character, \b \this returns \minus~1.

  \sa \ref mbrtowc, \ref mbtowc_l
*/
size_t mbrtowc_l(wchar_t *__pwc, const char *__s, size_t __n, mbstate_t *__ps, locale_t __loc);


/*! \brief Convert wide character to multi-byte character (restartable)
  \ingroup Multi-byte/wide string conversion functions \synopsis

  If \a s is a null pointer, \b \this function is equivalent to the call
  \tt{wcrtomb(buf, L'\0', ps)} where \a buf is an internal buffer.

  If \a s is not a null pointer, \b \this determines the number of bytes needed
  to represent the multibyte character that corresponds to the wide character given
  by \a wc, and stores the multibyte character representation in the array whose first
  element is pointed to by \a s. At most \b MB_CUR_MAX bytes are stored.  If \a wc is
  a null wide character, a null byte is stored; the resulting state described is
  the initial conversion state.

  \b \this returns the number of bytes stored in the array object. When \a wc is
  not a valid wide character, an encoding error occurs: \b \this stores the value
  of the macro \b EILSEQ in \b errno and returns \tt{(size_t)(-1)}; the conversion
  state is unspecified.
*/
size_t wcrtomb(char *__s, wchar_t __wc, mbstate_t *__ps);


/*! \brief Convert wide character to multi-byte character (restartable)
  \ingroup Multi-byte/wide string conversion functions \synopsis

  If \a s is a null pointer, \b \this function is equivalent to the call
  \tt{wcrtomb_l(buf, L'\0', ps, loc)} where \a buf is an internal buffer.

  If \a s is not a null pointer, \b \this determines the number of bytes needed
  to represent the multibyte character that corresponds to the wide character given
  by \a wc, and stores the multibyte character representation in the array whose first
  element is pointed to by \a s. At most \b MB_CUR_MAX bytes are stored.  If \a wc is
  a null wide character, a null byte is stored; the resulting state described is
  the initial conversion state.

  \b \this returns the number of bytes stored in the array object. When \a wc is
  not a valid wide character, an encoding error occurs: \b \this stores the value
  of the macro \b EILSEQ in \b errno and returns \tt{(size_t)(-1)}; the conversion
  state is unspecified.
*/
size_t wcrtomb_l(char *__s, wchar_t __wc, mbstate_t *__ps, locale_t __loc);


size_t mbsrtowcs(wchar_t *__dst, const char **__src, size_t __len, mbstate_t *__ps);
size_t mbsrtowcs_l(wchar_t *__dst, const char **__src, size_t __len, mbstate_t *__ps, locale_t __loc);

size_t wcsrtombs(char *__dst, const wchar_t **__src, size_t __len, mbstate_t *__ps);
size_t wcsrtombs_l(char *__dst, const wchar_t **__src, size_t __len, mbstate_t *__ps, locale_t __loc);

int swprintf(wchar_t *ws, size_t len, const wchar_t *format, ...);
int swscanf(const wchar_t *ws, const wchar_t *format, ...);
int vswprintf(wchar_t *ws, size_t len, const wchar_t *format, __va_list arg);
int vswscanf(const wchar_t *ws, const wchar_t *format, __va_list arg);

// These are not implemented, but keep some clients happy enough.
wint_t fgetwc(FILE *stream);
wchar_t *fgetws(wchar_t *ws, int num, FILE *stream);
wint_t fputwc(wchar_t wc, FILE *stream);
int fputws(const wchar_t *ws, FILE *stream);
int fwide(FILE *stream, int mode);
int fwprintf(FILE *stream, const wchar_t *format, ...);
int fwscanf(FILE *stream, const wchar_t *format, ...);
wint_t getwc(FILE *stream);
wint_t getwchar(void);
wint_t putwc(wchar_t wc, FILE *stream);
wint_t putwchar(wchar_t wc);
wint_t ungetwc(wint_t wc, FILE *stream);
int wcscoll(const wchar_t *wcs1, const wchar_t *wcs2);
int vcwscanf(const wchar_t *ws, const wchar_t *format, __va_list arg);
int vfwprintf(FILE *stream, const wchar_t *format, __va_list arg);
int vfwscanf(FILE *stream, const wchar_t *format, __va_list arg);
int vwprintf(const wchar_t *format, __va_list arg);
int vwscanf(const wchar_t *format, __va_list arg);
size_t wcsftime(wchar_t *ptr, size_t maxsize, const wchar_t *format,
                  const struct tm *timeptr);
size_t wcsxfrm(wchar_t *destination, const wchar_t *source, size_t num);
double wcstod(const wchar_t *str, wchar_t **endptr);
long int wcstol(const wchar_t *str, wchar_t **endptr, int base);
int wprintf(const wchar_t *format, ...);
int wscanf(const wchar_t *format, ...);


#ifdef __CROSSWORKS_DOCUMENTATION

/*! \brief Minimum value of a wide character
    \ingroup Character minimum and maximum values \synopsis

  \desc \b \this is the minimum value for an object of type \b wchar_t.
*/
#define WCHAR_MIN  ...


/*! \brief Maximum value of a wide character 
    \ingroup Character minimum and maximum values \synopsis

  \desc \b \this is the maximum value for an object of type \b wchar_t.
  Although capable of storing larger values, the maximum value
  implemented by the conversion functions in the library is the
  value 0x10FFFF defined by ISO 10646.
*/
#define WCHAR_MAX  ...

#endif


#ifdef __cplusplus
}
#endif

#endif
