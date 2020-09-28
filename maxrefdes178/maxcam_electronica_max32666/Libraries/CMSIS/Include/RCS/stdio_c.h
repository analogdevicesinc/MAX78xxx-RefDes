// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __stdio_c_H
#define __stdio_c_H

// Again, can't just #include <stddef.h> to get a tranche of common
// definitions, nor #include <stdarg.h> to get va_list, so #include
// "__crossworks.h" to get the private versions of all.
#include "__crossworks.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef EOF
#define EOF (-1)
#endif

#ifndef __RAL_SIZE_T_DEFINED
#define __RAL_SIZE_T_DEFINED
typedef __RAL_SIZE_T size_t;
#endif

/*! \brief Write a code string to standard output \ingroup Character and string I/O functions \synopsis

  \desc See \ref puts
*/
int puts_c(const __code char *__s);

/*! \brief Write code string formatted text to a string \ingroup Formatted output functions \synopsis

  \desc See \ref sprintf
*/
int sprintf_c(char *__s, const __code char *__format, ...);

/*! \brief Write code string formatted text to a string with truncation \ingroup Formatted output functions \synopsis

  \desc See \ref snprintf
*/
int snprintf_c(char *__s, size_t __n, const __code char *__format, ...);

/*! \brief Write code string formatted text to a string with truncation using variable argument context \ingroup Formatted output functions \synopsis

  \desc See \ref vsnprintf
*/
int vsnprintf_c(char *__s, size_t __n, const __code char *__format, __va_list __arg);

/*! \brief Write code string formatted text to standard output \ingroup Formatted output functions \synopsis

  \desc See \ref printf
*/
int printf_c(const __code char *__format, ...);

/*! \brief Write code string formatted text to standard output using variable argument context \ingroup Formatted output functions \synopsis

  \desc See \ref vprintf
*/
int vprintf_c(const __code char *__format, __va_list __arg);

/*! \brief Write code string formatted text to a string using variable argument context \ingroup Formatted output functions \synopsis

   \desc See \ref vsprintf
*/
int vsprintf_c(char *__s, const __code char *__format, __va_list __arg);

/*! \brief Read code string formatted text from standard input \ingroup Formatted input functions \synopsis

  \desc See \ref scanf
*/
int scanf_c(const __code char *__format, ...);

/*! \brief Read code string formatted text from code string \ingroup Formatted input functions \synopsis

  \desc See \ref sscanf
*/
int sscanf_c(const char *__s, const __code char *__format, ...);

/*! \brief Read code string formatted text from standard using variable argument context \ingroup Formatted input functions \synopsis

  \desc See \ref vscanf
*/
int vscanf_c(const __code char *__format, __va_list __arg);

/*! \brief Read code string formatted text from a string using variable argument context \ingroup Formatted input functions \synopsis

  \desc See \ref vsscanf
*/
int vsscanf_c(const char *__s, const __code char *__format, __va_list __arg);

#ifdef __cplusplus
}
#endif

#endif
