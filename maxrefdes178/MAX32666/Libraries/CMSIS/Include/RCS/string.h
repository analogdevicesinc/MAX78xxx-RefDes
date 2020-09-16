// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __string_H
#define __string_H

#include "__crossworks.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __RAL_SIZE_T_DEFINED
#define __RAL_SIZE_T_DEFINED
typedef __RAL_SIZE_T size_t;
#endif

#ifndef NULL
#define NULL 0
#endif

/*! \overview
  The header file \<string.h\> defines functions that operate on 
  arrays that are interpreted as null-terminated strings.

  Various methods are used for determining the lengths of the arrays, but in 
  all cases a \b{char *} or \b{void *} argument points to the initial (lowest 
  addressed) character of the array. If an array is accessed beyond the end of 
  an object, the behavior is undefined.

  Where an argument declared as \b size_t \em n specifies the length of an array 
  for a function, \em n can have the value zero on a call to that function. 
  Unless explicitly stated otherwise in the description of a particular function, 
  pointer arguments must have valid values on a call with a zero size. On such 
  a call, a function that locates a character finds no occurrence, a function 
  that compares two character sequences returns zero, and a function that copies 
  characters copies zero characters.
*/



/*! \brief Copy memory \ingroup Copying functions \synopsis

  \description \b \this copies \a n characters from the object pointed to by \a s2
  into the object pointed to by \a s1. The behavior of \b \this is undefined 
  if copying takes place between objects that overlap.

  \b \this returns the value of \a s1.
*/
void *memcpy(void *__s1, const void *__s2, size_t __n);

#if defined(__CROSSWORKS_ARM) || defined(__SES_ARM)

/*! \brief Copy memory \ingroup Copying functions \synopsis

  \description \b \this copies \a n characters from the object pointed to by \a s2
  into the object pointed to by \a s1. The behavior of \b \this is undefined 
  if copying takes place between objects that overlap. The implementation of \b \this
  is optimized for speed for all cases of memcpy and as such has a large code memory
  requirement. This function is implemented for little-endian ARM and 32-bit Thumb-2 
  instruction sets only.

  \b \this returns the value of \a s1.
*/
void *memcpy_fast(void *__s1, const void *__s2, size_t __n);

#endif

/*! \brief Copy memory with specified terminator (POSIX extension) \ingroup Copying functions \synopsis

  \description \b \this copies at most \a n characters from the object pointed to
  by \a s2 into the object pointed to by \a s1. The copying stops as soon as
  \a n characters are copied or the character \a c is copied into the destination
  object pointed to by \a s1.
  
  The behavior of \b \this is undefined if copying takes place between objects
  that overlap.

  \b \this returns a pointer to the character immediately following \a c in
  \a s1, or \b NULL if \a c was not found in the first \a n characters of \a s2.

  \note \b \this conforms to POSIX.1-2008.
*/
void *memccpy(void *__s1, const void *__s2, int __c, size_t __n);


/*! \brief Copy memory (GNU extension) \ingroup Copying functions \synopsis

  \description \b \this copies \a n characters from the object pointed to by \a s2
  into the object pointed to by \a s1. The behavior of \b \this is undefined 
  if copying takes place between objects that overlap.

  \b \this returns a pointer to the byte following the last written
  byte.
  
  \note This is an extension found in GNU libc.
*/
void *mempcpy(void *__s1, const void *__s2, size_t __n);


/*! \brief Safely copy overlapping memory \ingroup Copying functions \synopsis

  \description \b \this copies \a n characters from the object pointed to by \a s2
  into the object pointed to by \a s1 ensuring that if \a s1 and \a s2
  overlap, the copy works correctly. Copying takes place as if the \a n characters 
  from the object pointed to by \a s2 are first copied into a temporary array 
  of \a n characters that does not overlap the objects pointed to by \a s1 
  and \a s2, and then the \a n characters from the temporary array are 
  copied into the object pointed to by \a s1.

  \b \this returns the value of \a s1.
*/
void *memmove(void *__s1, const void *__s2, size_t __n);


/*! \brief Compare memory \ingroup Comparison functions \synopsis

  \description \b \this compares the first \a n characters of the object pointed 
  to by \a s1 to the first \a n characters of the object pointed to by 
  \a s2. \b \this returns an integer greater than, equal to, or less 
  than zero as the object pointed to by \a s1 is greater than, equal to, or 
  less than the object pointed to by \a s2.
*/
int memcmp(const void *__s1, const void *__s2, size_t __n);


/*! \brief Search memory for a character \ingroup Search functions \synopsis

  \description \b \this locates the first occurrence of \a c (converted to an
  \b unsigned \b char) in the initial \a n characters (each interpreted as \b unsigned 
  \b char) of the object pointed to by \a s.  Unlike \b strchr, \b \this
  does \em not terminate a search when a null character is found in the object 
  pointed to by \a s.

  \b \this returns a pointer to the located character, or a null pointer 
  if \a c does not occur in the object.
*/
void *memchr(const void *__s, int __c, size_t __n);


/*! \brief Set memory to character \ingroup Miscellaneous functions \synopsis

  \desc \b \this copies the value of \a c (converted to an \b unsigned \b char) 
  into each of the first \b n characters of the object pointed to by \a s. 

  \b \this returns the value of \a s.
*/
void *memset(void *__s, int __c, size_t __n);


/*! \brief Copy string \ingroup Copying functions \synopsis

  \desc \b \this copies the string pointed to by \a s2 (including the terminating 
  null character) into the array pointed to by \a s1. The behavior of \b \this
  is undefined if copying takes place between objects that overlap.

  \b \this returns the value of \a s1.
*/
char *strcpy(char *__s1, const char *__s2);


/*! \brief Copy string up to a maximum length \ingroup Copying functions \synopsis

  \desc \b \this copies not more than \a n characters from the array pointed 
  to by \a s2 to the array pointed to by \a s1. Characters that follow 
  a null character in \a s2 are not copied. The behavior of \b \this
  is undefined if copying takes place between objects that overlap. If the array 
  pointed to by \a s2 is a string that is shorter than \a n characters, 
  null characters are appended to the copy in the array pointed to by \a s1, 
  until \a n characters in all have been written.

  \b \this returns the value of \a s1.

  \note No null character is implicitly appended to the end of \a s1, so \a s1 will only be
  terminated by a null character if the length of the string pointed to by \a s2 is
  less than \a n.

*/
char *strncpy(char *__s1, const char *__s2, size_t __n);


/*! \brief Copy string up to a maximum length with terminator (BSD extension) \ingroup Copying functions \synopsis

  \desc \b \this copies up to \a n\minus~1 characters from the string
  pointed to by \a s2 into the array pointed to by \a s1 and always
  terminates the result with a null character.  The behavior of \b \this
  is undefined if copying takes place between objects that overlap.

  \b \this returns the number of characters it tried to copy, which is
  the length of the string \a s2 or \a n, whichever is smaller.

  \note \b \this is commonly found in OpenBSD libraries and contrasts with
  \b strncpy in that the resulting string is always terminated with a
  null character.
*/
size_t strlcpy(char *__s1, const char *__s2, size_t __n);


/*! \brief Concatenate strings \ingroup Copying functions \synopsis

  \desc \b \this appends a copy of the string pointed to by \a s2 (including 
  the terminating null character) to the end of the string pointed to by \a s1.
  The initial character of \a s2 overwrites the null character at the end 
  of \a s1. The behavior of \b \this is undefined if copying takes place 
  between objects that overlap.

  \b \this returns the value of \a s1.
*/
char *strcat(char *__s1, const char *__s2);


/*! \brief Concatenate strings up to maximum length \ingroup Copying functions \synopsis

  \desc \b \this appends not more than \a n characters from the array pointed 
  to by \a s2 to the end of the string pointed to by \a s1. A null character 
  in \a s1 and characters that follow it are not appended. The initial character 
  of \a s2 overwrites the null character at the end of \a s1. A terminating 
  null character is always appended to the result. The behavior of \b \this
  is undefined if copying takes place between objects that overlap.

  \b \this returns the value of \a s1.
*/
char *strncat(char *__s1, const char *__s2, size_t __n);


/*! \brief Copy string up to a maximum length with terminator (BSD extension) \ingroup Copying functions \synopsis

  \desc \b \this appends no more than \a n\minus\b{strlen}(\a dst)\minus~1
  characters pointed to by \a s2 into the array pointed to by \a s1 and always
  terminates the result with a null character if \a n is greater than zero.
  Both the strings \a s1 and \a s2 must be terminated with a null character on
  entry to \b \this and a byte for the terminating null should be included in
  \a n.
  
  The behavior of \b \this is undefined if copying takes place between objects
  that overlap.

  \b \this returns the number of characters it tried to copy, which is
  the sum of the lengths of the strings \a s1 and \a s2 or \a n,
  whichever is smaller.

  \note \b \this is commonly found in OpenBSD libraries.
*/
size_t strlcat(char *__s1, const char *__s2, size_t __n);


/*! \brief Compare strings \ingroup Comparison functions \synopsis

  \desc \b \this compares the string pointed to by \a s1 to the string pointed 
  to by \a s2. \b \this returns an integer greater than, equal to, or 
  less than zero if the string pointed to by \a s1 is greater than, equal 
  to, or less than the string pointed to by \a s2.
*/
int strcmp(const char *__s1, const char *__s2);


/*! \brief Compare strings up to a maximum length \ingroup Comparison functions \synopsis

  \desc \b \this compares not more than \a n characters from the array pointed 
  to by \a s1 to the array pointed to by \a s2. Characters that follow 
  a null character are not compared.

  \b \this returns an integer greater than, equal to, or less than zero, 
  if the possibly null-terminated array pointed to by \a s1 is greater than, 
  equal to, or less than the possibly null-terminated array pointed to by \a s2.
*/
int strncmp(const char *__s1, const char *__s2, size_t __n);


/*! \brief Compare strings ignoring case (POSIX extension) \ingroup Comparison functions \synopsis

  \desc \b \this compares the string pointed to by \a s1 to the string pointed 
  to by \a s2 ignoring differences in case. \b \this returns an integer greater
  than, equal to, or less than zero if the string pointed to by \a s1 is greater
  than, equal to, or less than the string pointed to by \a s2.

  \note \b \this conforms to POSIX.1-2008.
*/
int strcasecmp(const char *__s1, const char *__s2);


/*! \brief Compare strings up to a maximum length ignoring case (POSIX extension) \ingroup Comparison functions \synopsis

  \desc \b \this compares not more than \a n characters from the array pointed 
  to by \a s1 to the array pointed to by \a s2 ignoring differences in case.
  Characters that follow a null character are not compared.

  \b \this returns an integer greater than, equal to, or less than zero, 
  if the possibly null-terminated array pointed to by \a s1 is greater than, 
  equal to, or less than the possibly null-terminated array pointed to by \a s2.

  \note \b \this conforms to POSIX.1-2008.
*/
int strncasecmp(const char *__s1, const char *__s2, size_t __n);


/*! \brief Find character within string \ingroup Search functions \synopsis

  \desc \b \this locates the first occurrence of \a c (converted to 
  a \b char) in the string pointed to by \a s. The terminating null character 
  is considered to be part of the string.

  \b \this returns a pointer to the located character, or a null pointer 
  if \a c does not occur in the string.
*/
char *strchr(const char *__s, int __c);


/*! \brief Find character in a length-limited string \ingroup Search functions \synopsis

  \desc \b \this searches not more than \a n characters to locate the first occurrence
  of \a c (converted to a \b char) in the string pointed to by \a s.
  The terminating null character is considered to be part of the string.

  \b \this returns a pointer to the located character, or a null pointer 
  if \a c does not occur in the string.
*/
char *strnchr(const char *__str, size_t __n, int __ch);


/*! \brief Compute size of string not prefixed by a set of characters \ingroup Search functions \synopsis

  \desc \b \this computes the length of the maximum initial segment of the string 
  pointed to by \a s1 which consists entirely of characters not from the string 
  pointed to by \a s2.

  \b \this returns the length of the segment.
*/
size_t strcspn(const char *__s1, const char *__s2);


/*! \brief Find first occurrence of characters within string \ingroup Search functions \synopsis

  \desc \b \this locates the first occurrence in the string pointed to by \a s1
  of any character from the string pointed to by \a s2.

  \b \this returns a pointer to the character, or a null pointer if no character 
  from \a s2 occurs in \a s1.
*/
char *strpbrk(const char *__s1, const char *__s2);


/*! \brief Find last occurrence of character within string \ingroup Search functions \synopsis

  \desc \b \this locates the last occurrence of \a c (converted to a \b char) 
  in the string pointed to by \a s. The terminating null character is considered 
  to be part of the string.

  \b \this returns a pointer to the character, or a null pointer if \a c
  does not occur in the string.
*/
char *strrchr(const char *__s, int __c);


/*! \brief Compute size of string prefixed by a set of characters \ingroup Search functions \synopsis

  \desc \b \this computes the length of the maximum initial segment of the string 
  pointed to by \a s1 which consists entirely of characters from the string 
  pointed to by \a s2.

  \b \this returns the length of the segment.
*/
size_t strspn(const char *__s1, const char *__s2);


/*! \brief Find first occurrence of a string within string \ingroup Search functions \synopsis

  \desc \b \this locates the first occurrence in the string pointed to by \a s1
  of the sequence of characters (excluding the terminating null character) in 
  the string pointed to by \a s2.

  \b \this returns a pointer to the located string, or a null pointer if 
  the string is not found. If \a s2 points to a string with zero length, \b \this
  returns \a s1.
*/
char *strstr(const char *__s1, const char *__s2);


/*! \brief Find first occurrence of a string within length-limited string \ingroup Search functions \synopsis

  \desc \b \this searches at most \a n characters to locate the first occurrence
  in the string pointed to by \a s1 of the sequence of characters (excluding the
  terminating null character) in the string pointed to by \a s2.

  \b \this returns a pointer to the located string, or a null pointer if 
  the string is not found. If \a s2 points to a string with zero length, \b \this
  returns \a s1.

  \note \b \this is an extension commonly found in Linux and BSD C libraries.
*/
char *strnstr(const char *__s1, const char *__s2, size_t __n);


/*! \brief Find first case-insensitive occurrence of a string within string \ingroup Search functions \synopsis

  \desc \b \this locates the first occurrence in the string pointed to by \a s1
  of the sequence of characters (excluding the terminating null character) in 
  the string pointed to by \a s2 without regard to character case.

  \b \this returns a pointer to the located string, or a null pointer if 
  the string is not found. If \a s2 points to a string with zero length, \b \this
  returns \a s1.

  \note \b \this is an extension commonly found in Linux and BSD C libraries.
*/
char *strcasestr(const char *__s1, const char *__s2);


/*! \brief Find first case-insensitive occurrence of a string within length-limited string \ingroup Search functions \synopsis

  \desc \b \this searches at most \a n characters to locate the first occurrence
  in the string pointed to by \a s1 of the sequence of characters (excluding the
  terminating null character) in the string pointed to by \a s2 without regard
  to character case.

  \b \this returns a pointer to the located string, or a null pointer if 
  the string is not found. If \a s2 points to a string with zero length, \b \this
  returns \a s1.

  \note \b \this is an extension commonly found in Linux and BSD C libraries.
*/
char *strncasestr(const char *__s1, const char *__s2, size_t __n);


/*! \brief Calculate length of string \ingroup Miscellaneous functions \synopsis

  \desc \b \this returns the length of the string pointed to by \a s, that 
  is the number of characters that precede the terminating null character.
*/
size_t strlen(const char *__s);


/*! \brief Calculate length of length-limited string (POSIX extension) \ingroup Search functions \synopsis

  \desc \b \this returns the length of the string pointed to by \a s, up
  to a maximum of \a n characters.  \b \this only examines the first \a n characters
  of the string \a s.

  \note \b \this conforms to POSIX.1-2008.
*/
size_t strnlen(const char *__s, size_t __n);


/*! \brief Break string into tokens \ingroup Search functions \synopsis

  \desc \b \this A sequence of calls to \b \this breaks the string pointed to by \a s1 
  into a sequence of tokens, each of which is delimited by a character from the 
  string pointed to by \a s2. The first call in the sequence has a non-null 
  first argument; subsequent calls in the sequence have a null first argument. 
  The separator string pointed to by \a s2 may be different from call to call. 

  The first call in the sequence searches the string pointed to by \a s1
  for the first character that is not contained in the current separator string 
  pointed to by \a s2. If no such character is found, then there are no tokens 
  in the string pointed to by \a s1 and \b \this returns a null pointer. 
  If such a character is found, it is the start of the first token.

  \b \this then searches from there for a character that is contained in 
  the current separator string. If no such character is found, the current token 
  extends to the end of the string pointed to by \a s1, and subsequent searches 
  for a token will return a null pointer. If such a character is found, it is 
  overwritten by a null character, which terminates the current token. \b \this
  saves a pointer to the following character, from which the next search for a 
  token will start.

  Each subsequent call, with a null pointer as the value of the first argument, 
  starts searching from the saved pointer and behaves as described above.

  \note \b \this maintains static state and is therefore not reentrant and
  not thread safe.  See \ref strtok_r for a thread-safe and reentrant
  variant.
  
  \sa \ref strsep, \ref strtok_r.
*/
char *strtok(char *__s1, const char *__s2);


/*! \brief Break string into tokens (4.4BSD extension) \ingroup Search functions \synopsis

  \desc \b \this locates, in the string referenced by \a{*stringp}, the first
  occurrence of any character in the string \a delim (or the terminating null character)
  and replaces it with a null character.  The location of the next character after the
  delimiter character (or NULL, if the end of the string was reached) is stored
  in \a{*stringp}.  The original value of \a{*stringp} is returned. 

  An empty field (that is, a character in the string \a delim occurs as the
  first character of \a{*stringp} can be detected by comparing the location
  referenced by the returned pointer to the null character.

  If \a{*stringp} is initially null, \b \this returns null.
  
  \note \b \this is an extension commonly found in Linux and BSD C libraries.
*/
char *strsep(char **__stringp, const char *__delim);


/*! \brief Break string into tokens, reentrant version (POSIX extension) \ingroup Search functions \synopsis

  \desc \b \this is a reentrant version of the function \b strtok where the state is
  maintained in the object of type \b{char *} pointed to by \a s3.

  \note \b \this conforms to POSIX.1-2008 and is commonly found in
  Linux and BSD C libraries.

  \sa \ref strtok.
*/
char *strtok_r(char *__s1, const char *__s2, char **__s3);


/*! \brief Duplicate string (POSIX extension) \ingroup Copying functions \synopsis

  \desc \b \this duplicates the string pointed to by \a s1 by using \b malloc to
  allocate memory for a copy of \a s and then copying \a s, including the
  terminating null, to that memory
  
  \b \this returns a pointer to the new string or a null pointer if the
  new string cannot be created.  The returned pointer can be passed to \b free.

  \note \b \this conforms to POSIX.1-2008 and SC22 TR 24731-2.
*/
char *strdup(const char *__s1);


/*! \brief Duplicate string (POSIX extension) \ingroup Copying functions \synopsis

  \desc \b \this duplicates at most \a n characters from the the string pointed to
  by \a s1 by using \b malloc to allocate memory for a copy of \a s1.

  If the length of string pointed to by \a s1 is greater than \a n characters,
  only \a n characters will be duplicated.  If \a n is greater than the length of
  string pointed to by \a s1, all characters in the string are copied into the
  allocated array including the terminating null character.

  \b \this returns a pointer to the new string or a null pointer if the
  new string cannot be created.  The returned pointer can be passed to \b free.

  \note \b \this conforms to POSIX.1-2008 and SC22 TR 24731-2.
*/
char *strndup(const char *__s1, size_t __n);


/*! \brief Decode error code \ingroup Miscellaneous functions \synopsis

  \desc \b \this maps the number in \a num to a message string. Typically, 
  the values for \a num come from \b errno, but \b \this can 
  map any value of type \b int to a message.

  \b \this returns a pointer to the message string.
  The program must not modify the returned message string.
  The message may be overwritten by a subsequent call to \b \this.
*/
char *strerror(int __num);


// These aren't implemented but are here for STLport
int strcoll(const char *__str1, const char *__str2);
size_t strxfrm( char *__str1, const char *__str2, size_t __num);


#ifdef __cplusplus
}
#endif

#endif
