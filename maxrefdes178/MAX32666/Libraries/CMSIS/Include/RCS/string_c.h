// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __string_c_H
#define __string_c_H

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

/*! \brief Copy code memory to data memory\ingroup Copying functions \synopsis

  \description see \ref memcpy
*/
void *memcpy_c(void *s1, const __code void *s2, size_t n);

/*! \brief Compare code memory with data memory\ingroup Comparison functions \synopsis

  \description see \ref memcmp
*/
int memcmp_c(const void *s1, const __code void *s2, size_t n);

/*! \brief Copy code string to data string\ingroup Copying functions \synopsis

  \description see \ref strcpy
*/
char *strcpy_c(char *s1, const __code char *s2);

/*! \brief Copy code string to data string up to a maximum length \ingroup Copying functions \synopsis

  \description see \ref strncpy
*/
char *strncpy_c(char *s1, const __code char *s, size_t n);

/*! \brief Concatenate a code string to a data string \ingroup Copying functions \synopsis

  \description see \ref strcat
*/
char *strcat_c(char *s1, const __code char *s2);

/*! \brief Compare a code string to a data string \ingroup Comparison functions \synopsis

  \description see \ref strcmp
*/
int strcmp_c(const char *s1, const __code char *s2);

/*! \brief Compare a code string to a data string up to a maximum length \ingroup Comparison functions \synopsis

  \description see \ref strncmp
*/
int strncmp_c(const char *s1, const __code char *s2, size_t n);

/*! \brief Calculate length of a code string \ingroup Miscellaneous functions \synopsis

  \description see \ref strlen
*/
size_t strlen_c(const __code char *s);

#ifdef __cplusplus
}
#endif

#endif

