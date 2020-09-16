// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __stdlib_H
#define __stdlib_H

#include "__crossworks.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __RAL_SIZE_T_DEFINED
#define __RAL_SIZE_T_DEFINED
typedef __RAL_SIZE_T size_t;
#endif

#ifndef __cplusplus
#ifndef __RAL_WCHAR_T_DEFINED
#define __RAL_WCHAR_T_DEFINED
typedef __RAL_WCHAR_T wchar_t;
#endif
#endif

#ifndef NULL
#define NULL 0
#endif


/*! \brief EXIT_SUCCESS \ingroup Macros \synopsis

  \desc \b \this pass to \ref exit on successful termination.
*/
#define EXIT_SUCCESS    0


/*! \brief EXIT_FAILURE \ingroup Macros \synopsis

  \desc \b \this pass to \ref exit on unsuccessful termination.
*/
#define EXIT_FAILURE    1


/*! \brief RAND_MAX \ingroup Macros \synopsis

  \desc \b \this expands to an integer constant expression that is the maximum
  value returned by \ref rand.
*/
#define RAND_MAX 32767


/*! \brief Maximum number of bytes in a multi-byte character in the current locale \ingroup Macros \synopsis

  \desc \b \this expands to a positive integer expression with type \b size_t
  that is the maximum number of bytes in a multi-byte character for the
  extended character set specified by the current locale (category LC_CTYPE).
  \b \this is never greater than \b MB_LEN_MAX.
*/
#define MB_CUR_MAX  __RAL_mb_max(&__RAL_global_locale)


/*! \brief Structure containing quotient and remainder after division of an int \ingroup Types

  \desc \b \this stores the quotient and remainder returned by \ref div.
*/
typedef struct
{
  int quot;
  int rem;
} div_t;

/*! \brief Structure containing quotient and remainder after division of a long \ingroup Types

  \desc \b \this stores the quotient and remainder returned by \ref ldiv.
*/
typedef struct
{
  long quot;
  long rem;
} ldiv_t;


/*! \brief Structure containing quotient and remainder after division of a long long \ingroup Types

  \desc \b \this stores the quotient and remainder returned by \ref lldiv.
*/
typedef struct
{
  long long quot;
  long rem;
} lldiv_t;


/*! \brief Return an integer absolute value \ingroup Integer arithmetic functions \synopsis

  \desc \b \this returns the absolute value of the integer argument \a j.
*/
int abs(int __j);


/*! \brief Return a long integer absolute value \ingroup Integer arithmetic functions \synopsis

  \desc \b \this returns the absolute value of the long integer argument \a j.
*/
long int labs(long int __j);


/*! \brief Return a long long integer absolute value \ingroup Integer arithmetic functions \synopsis

  \desc \b \this returns the absolute value of the long long integer argument \a j.
*/
long long int llabs(long long int __j);


/*! \brief Divide two ints returning quotient and remainder \ingroup Integer arithmetic functions \synopsis

  \desc \b \this computes \a numer / \a denom and \a numer % \a denom
  in a single operation.

  \b \this returns a structure of type \ref div_t comprising both the
  quotient and the remainder. The structures contain the members
  \b quot (the quotient) and \b rem (the remainder), each of which has
  the same type as the arguments \a numer and \a denom. If either part
  of the result cannot be represented, the behavior is undefined.

  \sa \ref div_t
*/
div_t div(int __numer, int __denom);


/*! \brief Divide two longs returning quotient and remainder \ingroup Integer arithmetic functions \synopsis

  \desc \b \this computes \a numer / \a denom and \a numer % \a denom
  in a single operation.

  \b \this returns a structure of type \ref ldiv_t
  comprising both the quotient and the remainder. The structures contain the members
  \b quot (the quotient) and \b rem (the remainder), each of which has
  the same type as the arguments \b numer and \b denom. If either part
  of the result cannot be represented, the behavior is undefined.

  \sa \ref ldiv_t
*/
ldiv_t ldiv(long int __numer, long int __denom);


/*! \brief Divide two long longs returning quotient and remainder \ingroup Integer arithmetic functions \synopsis

  \b \this computes \b numer / \b denom and \b numer % \b denom
  in a single operation.

  \b \this returns a structure of type \ref lldiv_t
  comprising both the quotient and the remainder. The structures contain the members
  \b quot (the quotient) and \b rem (the remainder), each of which has
  the same type as the arguments \b numer and \b denom. If either part
  of the result cannot be represented, the behavior is undefined.

 \sa \ref lldiv_t

*/
lldiv_t lldiv(long long int __numer, long long int __denom);


/*! \brief Allocate space for a single object \ingroup Memory allocation functions \synopsis

  \desc \b \this allocates space for an object whose size is specified by 'b size
  and whose value is indeterminate.

  \b \this returns a null pointer if the space for the object cannot be
  allocated from free memory; if space for the object can be allocated, \b \this
  returns a pointer to the start of the allocated space.
*/
void *malloc(size_t __size);


/*! \brief Allocate space for an array of objects and initialize them to zero \ingroup Memory allocation functions \synopsis

  \desc \b \this allocates space for an array of \b nmemb objects, each of
  whose size is \b size. The space is initialized to all zero bits.

  \b \this returns a null pointer if the space for the array of object cannot
  be allocated from free memory; if space for the array can be allocated, \b calloc
  returns a pointer to the start of the allocated space.
*/
void *calloc(size_t __nobj, size_t __size);


/*! \brief Resizes allocated memory space or allocates memory space \ingroup Memory allocation functions \synopsis

  \desc \b \this deallocates the old object pointed to by \b ptr and returns
  a pointer to a new object that has the size specified by \b size. The contents
  of the new object is identical to that of the old object prior to deallocation, up to the
  lesser of the new and old sizes. Any bytes in the new object beyond the size
  of the old object have indeterminate values.

  If \b ptr is a null pointer, \b \this behaves like \this
  for the specified size. If memory for the new object cannot be allocated, the
  old object is not deallocated and its value is unchanged.

  \b \this returns a pointer to the new object (which may have
  the same value as a pointer to the old object), or a null pointer if the new
  object could not be allocated.

  If \b ptr does not match a pointer earlier returned by \b calloc, \b malloc,
  or \b \this, or if the space has been deallocated by a call to \b free
  or \b \this, the behavior is undefined.
*/
void *realloc(void *p, size_t __size);


/*! \brief Frees allocated memory for reuse \ingroup Memory allocation functions \synopsis

  \desc \b \this causes the space pointed to by \b ptr to be deallocated, that
  is, made available for further allocation. If \b ptr is a null pointer,
  no action occurs.

  If \b ptr does not match a pointer earlier returned by \b calloc, \b malloc,
  or \b realloc, or if the space has been deallocated by a call to \b \this
  or \b realloc, the behavior is undefined.
*/
void free(void *__p);


/*! \brief Convert string to double \ingroup String to number conversions \synopsis

  \desc \b \this converts the initial portion of the string pointed to by \b nptr to
  a \b double representation.

  \b \this does not affect the value of \b errno on an error. If the
  value of the result cannot be represented, the behavior is undefined.

  Except for the behavior on error, \b \this is equivalent to
  \tt{strtod(nptr, (char **)NULL)}.

  \b \this returns the converted value.

  \sa \ref strtod
*/
double atof(const char *__nptr);


/*! \brief Convert string to double \ingroup String to number conversions \synopsis

  \desc \b \this converts the initial portion of the string pointed to by \b nptr
  to a \b double representation.

  First, \b \this decomposes the input string into three parts: an initial,
  possibly empty, sequence of white-space characters (as specified by \ref isspace),
  a subject sequence resembling a floating-point constant, and a final string
  of one or more unrecognized characters, including the terminating null character
  of the input string. \b \this then attempts to convert the subject sequence
  to a floating-point number, and return the result.

  The subject sequence is defined as the longest initial subsequence of the input
  string, starting with the first non-white-space character, that is of the expected
  form. The subject sequence contains no characters if the input string is empty
  or consists entirely of white space, or if the first non-white-space character
  is other than a sign or a permissible letter or digit.

  The expected form of the subject sequence is an optional plus or minus sign
  followed by a nonempty sequence of decimal digits optionally containing a decimal-point
  character, then an optional exponent part.

  If the subject sequence begins with a minus sign, the value resulting from
  the conversion is negated.

  A pointer to the final string is stored in the object pointed to by \b \this,
  provided that \b endptr is not a null pointer.

  If the subject sequence is empty or does not have the expected form, no conversion
  is performed, the value of \b nptr is stored in the object pointed to by
  \b endptr, provided that \b endptr is not a null pointer.

  \b \this returns the converted value, if any. If no conversion could be
  performed, zero is returned. If the correct value is outside the range of representable
  values, \b HUGE_VAL is returned according to the sign of the value, if any,
  and the value of the macro \ref errno is stored in \ref errno.
*/
double strtod(const char *__nptr, char **__endptr);


/*! \brief Convert string to float \ingroup String to number conversions \synopsis

  \desc \b \this  converts the initial portion of the string pointed to by \b nptr
  to a \b double representation.

  First, \b \this decomposes the input string into three parts: an initial,
  possibly empty, sequence of white-space characters (as specified by \ref isspace),
  a subject sequence resembling a floating-point constant, and a final string
  of one or more unrecognized characters, including the terminating null character
  of the input string. \b \this then attempts to convert the subject sequence
  to a floating-point number, and return the result.

  The subject sequence is defined as the longest initial subsequence of the input
  string, starting with the first non-white-space character, that is of the expected
  form. The subject sequence contains no characters if the input string is empty
  or consists entirely of white space, or if the first non-white-space character
  is other than a sign or a permissible letter or digit.

  The expected form of the subject sequence is an optional plus or minus sign
  followed by a nonempty sequence of decimal digits optionally containing a decimal-point
  character, then an optional exponent part.

  If the subject sequence begins with a minus sign, the value resulting from
  the conversion is negated.
  A pointer to the final string is stored in the object pointed to by \b endptr,
  provided that \b endptr is not a null pointer.

  If the subject sequence is empty or does not have the expected form, no conversion
  is performed, the value of \b nptr is stored in the object pointed to by
  \b endptr, provided that \b endptr is not a null pointer.

  \b \this returns the converted value, if any. If no conversion could be
  performed, zero is returned. If the correct value is outside the range of representable
  values, \b HUGE_VALF is returned according to the sign of the value, if
  any, and the value of the macro \ref errno
  is stored in \ref errno.
*/
float strtof(const char *__nptr, char **__endptr);


/*! \brief Convert string to int \ingroup String to number conversions \synopsis

  \desc \b \this converts the initial portion of the string pointed to by \b nptr
  to an \b int representation.

  \b \this does not affect the value of \b errno on an error. If the value
  of the result cannot be represented, the behavior is undefined.

  Except for the behavior on error, \b \this is equivalent to
  \tt{(int)strtol(nptr, (char **)NULL, 10)}.

  \b \this returns the converted value.

  \sa \ref strtol
*/
int atoi(const char *__nptr);


/*! \brief Convert string to long \ingroup String to number conversions \synopsis

  \desc \b \this converts the initial portion of the string pointed to by \b nptr
  \b to a \b{long int} representation.

 \b atol does not affect the value of \b errno on an error. If the value
  of the result cannot be represented, the behavior is undefined.

  Except for the behavior on error, \b \this is equivalent to
  \tt{strtol(nptr, (char **)NULL, 10)}.

  \b \this returns the converted value.

  \sa \ref strtol
*/
long int atol(const char *__nptr);


/*! \brief Convert string to long long \ingroup String to number conversions \synopsis

  \desc \b \this converts the initial portion of the string pointed to by \b nptr
  to a \b{long long int} representation.

  \b \this does not affect the value of \b errno
  on an error. If the value of the result cannot be represented, the behavior
  is undefined.

  Except for the behavior on error, \b \this is equivalent to
  \tt{strtoll(nptr, (char **)NULL, 10)}.

  \b \this returns the converted value.

\sa \ref strtoll
*/
long long int atoll(const char *__nptr);


/*! \brief Convert string to long \ingroup String to number conversions \synopsis

  \desc \b \this converts the initial portion of the string pointed to by \b nptr
  to a \b{long int} representation.

  First, \b strtol decomposes the input string into three parts: an initial,
  possibly empty, sequence of white-space characters (as specified by \ref isspace),
  a subject sequence resembling an integer represented in some radix determined
  by the value of \b base, and a final string of one or more unrecognized
  characters, including the terminating null character of the input string. \b \this
  then attempts to convert the subject sequence to an integer, and return the
  result.

  When converting, no integer suffix (such as U, L, UL, LL, ULL) is allowed.

  If the value of \b base is zero, the expected form of the subject sequence
  is an optional plus or minus sign followed by an integer constant.

  If the value of \b base is between 2 and 36 (inclusive), the expected form
  of the subject sequence is an optional plus or minus sign followed by a sequence
  of letters and digits representing an integer with the radix specified by \b base.
  The letters from a (or A) through z (or Z) represent the values 10 through 35;
  only letters and digits whose ascribed values are less than that of \b base
  are permitted.

  If the value of \b base is 16, the characters \q{0x} or \q{0X} may optionally
  precede the sequence of letters and digits, following the optional sign.

  The subject sequence is defined as the longest initial subsequence of the input
  string, starting with the first non-white-space character, that is of the expected
  form. The subject sequence contains no characters if the input string is empty
  or consists entirely of white space, or if the first non-white-space character
  is other than a sign or a permissible letter or digit.

  If the subject sequence has the expected form and the value of \b base
  is zero, the sequence of characters starting with the first digit is interpreted
  as an integer constant. If the subject sequence has the expected form and the
  value of \b base is between 2 and 36, it is used as the base for conversion.

  If the subject sequence begins with a minus sign, the value resulting from
  the conversion is negated.

  A pointer to the final string is stored in the object pointed to by \b endptr,
  provided that \b endptr is not a null pointer.

  If the subject sequence is empty or does not have the expected form, no conversion
  is performed, the value of \b nptr is stored in the object pointed to by
  \b endptr, provided that \b endptr is not a null pointer.

  \b \this returns the converted value, if any. If no conversion could be
  performed, zero is returned. If the correct value is outside the range of representable
  values, \ref LONG_MIN or \ref LONG_MAX
  is returned according to the sign of the value, if any, and the value of the
  macro \ref errno is stored in \ref errno.
*/
long int strtol(const char *__nptr, char **__endptr, int __base);


/*! \brief Convert string to long long \ingroup String to number conversions \synopsis

  \desc \b \this converts the initial portion of the string pointed to by \b nptr
  to a \b{long int} representation.

  First, \b \this decomposes the input string into three parts: an initial,
  possibly empty, sequence of white-space characters (as specified by \ref isspace),
  a subject sequence resembling an integer represented in some radix determined
  by the value of \b base, and a final string of one or more unrecognized
  characters, including the terminating null character of the input string. \b \this
  then attempts to convert the subject sequence to an integer, and return the
  result.

  When converting, no integer suffix (such as U, L, UL, LL, ULL) is allowed.

  If the value of \b base is zero, the expected form of the subject sequence
  is an optional plus or minus sign followed by an integer constant.

  If the value of \b base is between 2 and 36 (inclusive), the expected form
  of the subject sequence is an optional plus or minus sign followed by a sequence
  of letters and digits representing an integer with the radix specified by \b base.
  The letters from a (or A) through z (or Z) represent the values 10 through 35;
  only letters and digits whose ascribed values are less than that of \b base
  are permitted.

  If the value of \b base is 16, the characters \q{0x} or \q{0X} may optionally
  precede the sequence of letters and digits, following the optional sign.

  The subject sequence is defined as the longest initial subsequence of the input
  string, starting with the first non-white-space character, that is of the expected
  form. The subject sequence contains no characters if the input string is empty
  or consists entirely of white space, or if the first non-white-space character
  is other than a sign or a permissible letter or digit.

  If the subject sequence has the expected form and the value of \b base
  is zero, the sequence of characters starting with the first digit is interpreted
  as an integer constant. If the subject sequence has the expected form and the
  value of \b base is between 2 and 36, it is used as the base for conversion.

  If the subject sequence begins with a minus sign, the value resulting from
  the conversion is negated.

  A pointer to the final string is stored in the object pointed to by \b endptr,
  provided that \b endptr is not a null pointer.

  If the subject sequence is empty or does not have the expected form, no conversion
  is performed, the value of \b nptr is stored in the object pointed to by
  \b endptr, provided that \b endptr is not a null pointer.

  \b \this returns the converted value, if any. If no conversion could
  be performed, zero is returned. If the correct value is outside the range of
  representable values, \ref LLONG_MIN or \ref LLONG_MAX is returned according
  to the sign of the value, if any, and the value of the macro \b ERANGE is
  stored in \ref errno.
*/
long long int strtoll(const char *__nptr, char **__endptr, int __base);


/*! \brief Convert string to unsigned long \ingroup String to number conversions \synopsis

  \desc \b \this converts the initial portion of the string pointed to by \b nptr
  to a \b{long int} representation.

  First, \b \this decomposes the input string into three parts: an initial,
  possibly empty, sequence of white-space characters (as specified by \ref isspace),
  a subject sequence resembling an integer represented in some radix determined
  by the value of \b base, and a final string of one or more unrecognized
  characters, including the terminating null character of the input string. \b strtoul
  then attempts to convert the subject sequence to an integer, and return the
  result.

  When converting, no integer suffix (such as U, L, UL, LL, ULL) is allowed.

  If the value of \b base is zero, the expected form of the subject sequence
  is an optional plus or minus sign followed by an integer constant.

  If the value of \b base is between 2 and 36 (inclusive), the expected form
  of the subject sequence is an optional plus or minus sign followed by a sequence
  of letters and digits representing an integer with the radix specified by \b base.
  The letters from a (or A) through z (or Z) represent the values 10 through 35;
  only letters and digits whose ascribed values are less than that of \b base
  are permitted.

  If the value of \b base is 16, the characters \q{0x} or \q{0X} may optionally
  precede the sequence of letters and digits, following the optional sign.

  The subject sequence is defined as the longest initial subsequence of the input
  string, starting with the first non-white-space character, that is of the expected
  form. The subject sequence contains no characters if the input string is empty
  or consists entirely of white space, or if the first non-white-space character
  is other than a sign or a permissible letter or digit.

  If the subject sequence has the expected form and the value of \b base
  is zero, the sequence of characters starting with the first digit is interpreted
  as an integer constant. If the subject sequence has the expected form and the
  value of \b base is between 2 and 36, it is used as the base for conversion.

  If the subject sequence begins with a minus sign, the value resulting from
  the conversion is negated.

  A pointer to the final string is stored in the object pointed to by \b endptr,
  provided that \b endptr is not a null pointer.

  If the subject sequence is empty or does not have the expected form, no conversion
  is performed, the value of \b nptr is stored in the object pointed to by
  \b endptr, provided that \b endptr is not a null pointer.

  \b \this returns the converted value, if any. If no conversion could
  be performed, zero is returned. If the correct value is outside the range of
  representable values, \ref LONG_MAX or \ref ULONG_MAX is returned according
  to the sign of the value, if any, and the value of the macro \b ERANGE is
  stored in \ref errno.
  */
unsigned long int strtoul(const char *__nptr, char **__endptr, int __base);


/*! \brief Convert string to unsigned long long \ingroup String to number conversions \synopsis

  \desc \b \this converts the initial portion of the string pointed to by \b nptr
  to a \b{long int} representation.

  First, \b strtoull decomposes the input string into three parts: an initial,
  possibly empty, sequence of white-space characters (as specified by \ref isspace),
  a subject sequence resembling an integer represented in some radix determined
  by the value of \b base, and a final string of one or more unrecognized
  characters, including the terminating null character of the input string. \b \this
  then attempts to convert the subject sequence to an integer, and return the
  result.

  When converting, no integer suffix (such as U, L, UL, LL, ULL) is allowed.

  If the value of \b base is zero, the expected form of the subject sequence
  is an optional plus or minus sign followed by an integer constant.

  If the value of \b base is between 2 and 36 (inclusive), the expected form
  of the subject sequence is an optional plus or minus sign followed by a sequence
  of letters and digits representing an integer with the radix specified by \b base.
  The letters from a (or A) through z (or Z) represent the values 10 through 35;
  only letters and digits whose ascribed values are less than that of \b base
  are permitted.

  If the value of \b base is 16, the characters \q{0x} or \q{0X} may optionally
  precede the sequence of letters and digits, following the optional sign.

  The subject sequence is defined as the longest initial subsequence of the input
  string, starting with the first non-white-space character, that is of the expected
  form. The subject sequence contains no characters if the input string is empty
  or consists entirely of white space, or if the first non-white-space character
  is other than a sign or a permissible letter or digit.

  If the subject sequence has the expected form and the value of \b base
  is zero, the sequence of characters starting with the first digit is interpreted
  as an integer constant. If the subject sequence has the expected form and the
  value of \b base is between 2 and 36, it is used as the base for conversion.

  If the subject sequence begins with a minus sign, the value resulting from
  the conversion is negated.

  A pointer to the final string is stored in the object pointed to by \b endptr,
  provided that \b endptr is not a null pointer.

  If the subject sequence is empty or does not have the expected form, no conversion
  is performed, the value of \b nptr is stored in the object pointed to by
  \b endptr, provided that \b endptr is not a null pointer.

  \b \this returns the converted value, if any. If no conversion could
  be performed, zero is returned. If the correct value is outside the range of
  representable values, \ref LLONG_MAX or \ref ULLONG_MAX is returned according
  to the sign of the value, if any, and the value of the macro \b ERANGE is
  stored in \ref errno.
*/
unsigned long long int strtoull(const char *__nptr, char **__endptr, int __base);


/*! \brief Return next random number in sequence \ingroup Pseudo-random sequence generation functions \synopsis

  \desc \b \this computes a sequence of pseudo-random integers in the range 0 to
  \b RAND_MAX.

  \b \this returns the computed pseudo-random integer.
*/
int rand(void);


/*! \brief Set seed of random number sequence \ingroup Pseudo-random sequence generation functions \synopsis

  \desc \b \this uses the argument \b seed as a seed for a new sequence of
  pseudo-random numbers to be returned by subsequent calls to \b rand. If
  \b \this is called with the same seed value, the same sequence of pseudo-random
  numbers is generated.

  If \b rand is called before any calls to \b \this have been made, a
  sequence is generated as if \b \this is first called with a seed value of
  1.

  \sa \ref rand
*/
void srand(unsigned int __seed);


/*! \brief Search a sorted array \ingroup Search and sort functions \synopsis

  \desc \b \this searches the array \a *base for the specified \a *key and returns a
  pointer to the first entry that matches or null if no match. The array should
  have \a num elements of \a size bytes and be sorted by the same algorithm as
  the \a compare function.

  The \a compare function should return a negative value if the first parameter
  is less than second parameter, zero if the parameters are equal, and a positive
  value if the first parameter is greater than the second parameter.
*/
void *bsearch(const void *__key,
              const void *__buf,
              size_t __num,
              size_t __size,
              int (*__compare)(const void *, const void *));


/*! \brief Sort an array \ingroup Search and sort functions \synopsis

  \b qsort sorts the array \a *base using the \a compare function. The
  array should have \a num elements of \a size bytes. The \a compare function
  should return a negative value if the first parameter is less than second
  parameter, zero if the parameters are equal and a positive value if the first
  parameter is greater than the second parameter.
*/
void qsort(void *__buf,
           size_t __num,
           size_t __size,
           int (*__compare)(const void *, const void *));

void abort(void);


/*! \brief Terminates the calling process \ingroup Environment \synopsis

  \desc \b \this returns to the startup code and performs the appropriate cleanup process.
*/
void exit(int __exit_code);


/*! \brief Set function to be execute on exit \ingroup Environment \synopsis

  \desc \b \this registers \b function to be called when the application has
  exited. The functions registered with \b \this are executed in reverse
  order of their registration. \b \this returns 0 on success and non-zero
  on failure.
*/
int atexit(void (*__func)(void));

char *getenv(const char *__name);
int system(const char *__command);

// Extensions

/*! \brief Convert int to string \ingroup Number to string conversions \synopsis

  \desc \b \this converts \b val to a string in base \b radix and places
  the result in \b buf.

  \b \this returns \b buf as the result.

  If \b radix is greater than 36, the result is undefined.

  If \b val is negative and \b radix is 10, the string has a leading minus
  sign (-); for all other values of \b radix, \b value is considered unsigned
  and never has a leading minus sign.

 \sa \ref ltoa, \ref lltoa, \ref ultoa, \ref ulltoa, \ref utoa
*/
char *itoa(int __val, char *__buf, int __radix);


/*! \brief Convert unsigned to string \ingroup Number to string conversions \synopsis

  \desc \b \this converts \b val to a string in base \b radix and places
  the result in \b buf.

  \b \this returns \b buf as the result.

  If \b radix is greater than 36, the result is undefined.

  \sa \ref itoa, \ref ltoa, \ref lltoa, \ref ultoa, \ref ulltoa
*/
char *utoa(unsigned val, char *buf, int radix);


/*! \brief Convert long to string \ingroup Number to string conversions \synopsis

  \desc \b \this converts \b val to a string in base \b radix and places
  the result in \b buf.

  \b \this returns \b buf as the result.

  If \b radix is greater than 36, the result is undefined.

  If \b val is negative and radix is 10, the string has a leading minus
  sign (-); for all other values of \b radix, \b value is considered unsigned
  and never has a leading minus sign.

  \sa \ref itoa, \ref lltoa, \ref ultoa, \ref ulltoa, \ref utoa
*/
char *ltoa(long __val, char *__buf, int __radix);


/*! \brief Convert unsigned long to string \ingroup Number to string conversions \synopsis

  \desc \b \this converts \b val to a string in base \b radix and places
  the result in \b buf.

  \b \this returns \b buf as the result.

  If \b radix is greater than 36, the result is undefined.

  \sa \ref itoa, \ref ltoa, \ref lltoa, \ref ulltoa, \ref utoa
*/
char *ultoa(unsigned long __val, char *__buf, int __radix);


/*! \brief Convert long long to string \ingroup Number to string conversions \synopsis

  \desc \b \this converts \b val to a string in base \b radix and places
  the result in \b buf.

  \b \this returns \b buf as the result.

 If \b radix is greater than 36, the result is undefined.

 If \b val is negative and radix is 10, the string has a leading minus
 sign (-); for all other values of \b radix, \b value is considered unsigned
 and never has a leading minus sign.

  \sa \ref itoa, \ref ltoa, \ref ultoa, \ref ulltoa, \ref utoa
*/
char *lltoa(long long __val, char *__buf, int __radix);


/*! \brief Convert unsigned long long to string \ingroup Number to string conversions \synopsis

  \desc \b \this converts \b val to a string in base \b radix and places
  the result in \b buf.

  \b \this returns \b buf as the result.

  If \b radix is greater than 36, the result is undefined.

  \sa \ref itoa, \ref ltoa, \ref lltoa, \ref ultoa, \ref utoa
*/
char *ulltoa(unsigned long long __val, char *__buf, int __radix);


/*! \brief Determine number of bytes in a multi-byte character
  \ingroup Multi-byte/wide character conversion functions \synopsis

  \desc \b \this determines the number of bytes contained in the multi-byte
  character pointed to by \a s in the current locale.

  If \a s is a null pointer, \b \this returns a nonzero or zero value, if multi-byte
  character encodings, respectively, do or do not have state-dependent encodings

  If \a s is not a null pointer, \b \this either returns 0 (if \a s points to the
  null character), or returns the number of bytes that are contained in the
  multi-byte character (if the next \a n or fewer bytes form a valid multi-byte
  character), or returns \minus~1 (if they do not form a valid multi-byte
  character).

  \note Except that the conversion state of the \a mbtowc function is not
  affected, it is equivalent to

  \code mbtowc((wchar_t *)0, s, n); \endcode

  \note It is guaranteed that no library function in the Standard C library
  calls \b \this.

  \sa \ref mblen_l, \ref mbtowc
*/
int mblen(const char *__s, size_t __n);


/*! \brief Determine number of bytes in a multi-byte character
  \ingroup Multi-byte/wide character conversion functions \synopsis

  \desc \b \this determines the number of bytes contained in the multi-byte
  character pointed to by \a s in the locale \a loc.

  If \a s is a null pointer, \b \this returns a nonzero or zero value, if multi-byte
  character encodings, respectively, do or do not have state-dependent encodings

  \a If s is not a null pointer, \b \this either returns 0 (if \a s points to the
  null character), or returns the number of bytes that are contained in the
  multi-byte character (if the next \a n or fewer bytes form a valid multi-byte
  character), or returns \minus~1 (if they do not form a valid multi-byte
  character).

  \note Except that the conversion state of the \a mbtowc_l function is not
  affected, it is equivalent to

  \code mbtowc((wchar_t *)0, s, n, loc); \endcode

  \note It is guaranteed that no library function in the Standard C library
  calls \b \this.

  \sa \ref mblen_l, \ref mbtowc_l
*/
int mblen_l(const char *__s, size_t __n, struct __locale_s *__loc);


/*! \brief Convert multi-byte string to wide string
  \ingroup Multi-byte/wide string conversion functions \synopsis

  \desc \b \this converts a sequence of multi-byte characters that begins
  in the initial shift state from the array pointed to by \a s into a
  sequence of corresponding wide characters and stores not more than
  \a n wide characters into the array pointed to by \a pwcs.

  No multi-byte characters that follow a null character (which is converted
  into a null wide character) will be examined or converted. Each multi-byte
  character is converted as if by a call to the \b mbtowc function, except
  that the conversion state of the \b mbtowc function is not affected.

  No more than \a n elements will be modified in the array pointed to by
  \a pwcs. If copying takes place between objects that overlap, the behavior
  is undefined.

  \b \this returns \minus~1 if an invalid multi-byte character is
  encountered, otherwise \b \this returns the number of array
  elements modified (if any), not including a terminating null wide
  character.
*/
size_t mbstowcs(wchar_t *__pwcs, const char *__s, size_t __n);


/*! \brief Convert multi-byte string to wide string using specified locale
  \ingroup Multi-byte/wide string conversion functions \synopsis

  \desc \b \this is as \a mbstowcs except that the local \a loc is used
  for the conversion as opposed to the current locale.

  \sa \ref mbstowcs.
*/
size_t mbstowcs_l(wchar_t *__pwcs, const char *__s, size_t __n, struct __locale_s *__loc);


/*! \brief Convert multi-byte character to wide character
  \ingroup Multi-byte/wide string conversion functions \synopsis

  \desc \b \this converts a single multi-byte character to a wide
  character in the current locale.

  If \a s is a null pointer, \b \this returns a nonzero value if multi-byte
  character encodings are state-dependent in the current locale, and zero otherwise.

  If \a s is not null and the object that \a s points to is a
  wide-character null character, \b \this returns 0.

  If \a s is not null and the object that \s points to forms a valid
  multi-byte character, \b \this returns the length in bytes of the
  multi-byte character.

  If the object that \s points to does not form a valid multi-byte
  character within the first \a n characters, it returns \minus~1.

  \sa \ref mbtowc_l
*/
int mbtowc(wchar_t *__pwc, const char *__s, size_t __n);


/*! \brief Convert multi-byte character to wide character
  \ingroup Multi-byte/wide string conversion functions \synopsis

  \desc \b \this converts a single multi-byte character to a wide
  character in locale \a loc.

  If \a s is a null pointer, \b \this returns a nonzero value if multi-byte
  character encodings are state-dependent in the locale \a loc,
  and zero otherwise.

  If \a s is not null and the object that \a s points to is a
  wide-character null character, \b \this returns 0.

  If \a s is not null and the object that \s points to forms a valid
  multi-byte character, \b \this returns the length in bytes of the
  multi-byte character.

  If the object that \a s points to does not form a valid multi-byte
  character within the first \a n characters, it returns \minus~1.

  \sa \ref mbtowc
*/
int mbtowc_l(wchar_t *__pwc, const char *__s, size_t __n, struct __locale_s *__loc);


int wctomb(char *__s, wchar_t __wc);
int wctomb_l(char *__s, wchar_t __wc, struct __locale_s *__loc);

size_t wcstombs(char *__s, const wchar_t *__pwcs, size_t __n);
size_t wcstombs_l(char *__s, const wchar_t *__pwcs, size_t __n, struct __locale_s *__loc);

#ifdef __cplusplus
}
#endif

#endif
