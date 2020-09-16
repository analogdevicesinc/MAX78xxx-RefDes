// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __stdint_H
#define __stdint_H

typedef signed    char     int8_t;
typedef unsigned  char     uint8_t;

#if defined(__CROSSWORKS_ARM) || defined(__CROSSWORKS_MAXQ30) || defined(__SES_ARM)

// 32-bit compiler
typedef signed   short     int16_t;
typedef unsigned short     uint16_t;
typedef signed   int       int32_t;
typedef unsigned int       uint32_t;

#else

// 16-bit compiler
typedef signed   int       int16_t;
typedef unsigned int       uint16_t;
typedef signed   long      int32_t;
typedef unsigned long      uint32_t;

#endif

typedef signed   long long int64_t;
typedef unsigned long long uint64_t;


typedef int8_t int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;

typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;

#if defined(__CROSSWORKS_ARM) || defined(__CROSSWORKS_MAXQ30) || defined(__SES_ARM)

typedef int32_t int_fast8_t;
typedef int32_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef int64_t int_fast64_t;
    
typedef uint32_t uint_fast8_t;
typedef uint32_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
typedef uint64_t uint_fast64_t;

typedef int32_t intptr_t;
typedef uint32_t uintptr_t;

#elif defined(__CROSSWORKS_AVR) || defined(__CROSSWORKS_MSP430) || defined(__CROSSWORKS_MAXQ)

typedef int16_t int_fast8_t;
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef int64_t int_fast64_t;
    
typedef uint16_t uint_fast8_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
typedef uint64_t uint_fast64_t;

typedef int16_t intptr_t;
typedef uint16_t uintptr_t;

#else

#error unknown target

#endif

typedef int64_t intmax_t;
typedef uint64_t uintmax_t;

#define UINT8_MAX   255

#define INT8_MAX    127
#define INT8_MIN    (-128)

#define UINT16_MAX  65535

#define INT16_MIN   (-32767-1)
#define INT16_MAX   32767

#define UINT32_MAX  4294967295UL
#define INT32_MAX   2147483647L
#define INT32_MIN   (-2147483647L-1)

#define INT64_MIN   (-9223372036854775807LL-1)
#define INT64_MAX   9223372036854775807LL
#define UINT64_MAX  18446744073709551615ULL

#define INTMAX_MIN  (-9223372036854775807LL-1)
#define INTMAX_MAX  9223372036854775807LL
#define UINTMAX_MAX 18446744073709551615ULL


#define INT_LEAST8_MIN   INT8_MIN
#define INT_LEAST16_MIN  INT16_MIN
#define INT_LEAST32_MIN  INT32_MIN
#define INT_LEAST64_MIN  INT64_MIN
#define INT_LEAST8_MAX   INT8_MAX
#define INT_LEAST16_MAX  INT16_MAX
#define INT_LEAST32_MAX  INT32_MAX
#define INT_LEAST64_MAX  INT64_MAX
#define UINT_LEAST8_MAX  UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

#define INT_FAST8_MIN    INT8_MIN
#define INT_FAST16_MIN   INT32_MIN
#define INT_FAST32_MIN   INT32_MIN
#define INT_FAST64_MIN   INT64_MIN
#define INT_FAST8_MAX    INT8_MAX
#define INT_FAST16_MAX   INT32_MAX
#define INT_FAST32_MAX   INT32_MAX
#define INT_FAST64_MAX   INT64_MAX
#define UINT_FAST8_MAX   UINT8_MAX
#define UINT_FAST16_MAX  UINT32_MAX
#define UINT_FAST32_MAX  UINT32_MAX
#define UINT_FAST64_MAX  UINT64_MAX

#if defined(__CROSSWORKS_ARM) || defined(__CROSSWORKS_MAXQ30) || defined(__SES_ARM)

// 32-bit compilers, half the address space...
#define PTRDIFF_MIN      INT32_MIN
#define PTRDIFF_MAX      INT32_MAX
#define SIZE_MAX         INT32_MAX

#define INTPTR_MIN      INT32_MIN
#define INTPTR_MAX      INT32_MAX
#define UINTPTR_MAX     UINT32_MAX

#else

// 16-bit compilers will generate a 16-bit difference, and I
// really don't care what the ISO standard spews about it.
#define PTRDIFF_MIN      INT16_MIN
#define PTRDIFF_MAX      INT16_MAX
#define SIZE_MAX         INT16_MAX

#define INTPTR_MIN      INT16_MIN
#define INTPTR_MAX      INT16_MAX
#define UINTPTR_MAX     UINT16_MAX

#endif

#define INT8_C(x)   (x)
#define UINT8_C(x)  (x##U)
#define INT16_C(x)  (x)
#define UINT16_C(x) (x##U)
#define INT32_C(x)  (x##L)
#define UINT32_C(x) (x##UL)
#define INT64_C(x)  (x##LL)
#define UINT64_C(x) (x##ULL)

#define INTMAX_C(x) (x##LL)
#define UINTMAX_C(x) (x##ULL)

// Deal with different sizes of wchar_t.
#if __SIZEOF_WCHAR_T==2

#define WCHAR_MIN  0U
#define WCHAR_MAX  65535U

#else

#define WCHAR_MIN  (-2147483647L-1)
#define WCHAR_MAX  2147483647L

#endif

#define WINT_MIN   (-2147483647L-1)
#define WINT_MAX   2147483647L

#endif
