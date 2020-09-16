// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __vfprintf_H
#define __vfprintf_H

#if defined(__CROSSWORKS) || defined(__CROSSWORKS_ARM) || defined(__SES_ARM)
#include "__argtype.h"
#else
#include "../../lib/hosted__argtype.h"
#endif

#ifndef __MEMSPACE
#define __MEMSPACE
#endif

#ifndef __CODE
#define __CODE
#endif

#ifdef __cplusplus
extern "C" {
#endif

#undef __VFPRINTF_CHAR
#undef __VFSCANF_CHAR
#if defined(__VFxxxxx_WCHAR)
#include <wchar.h>
#define __VFPRINTF_CHAR wchar_t
#define __VFSCANF_CHAR wchar_t
#define __VFPRINTF_USER_FORMAT_EXTENDER __user_format_extender_wchar
#define __VFPRINTF_PUTC __RAL_putc_wchar
#define __VFPRINTF_GETC __RAL_getc_wchar
#define __VFPRINTF_PRINT_PADDING __RAL_print_padding_wchar
#define __VFPRINTF_PRE_PADDING __RAL_pre_padding_wchar
#define __VFPRINTF_PRINT_WIDE_STRING __RAL_print_wide_string_wchar
#if __SIZEOF_WCHAR_T==2
#define __VFPRINTF_CHAR_PTR_ARG ARGTYPE_UnsignedShortPtrArg
#define __VFPRINTF_WRITE_PTR_ARG_CHAR(a, ptr, v) *(unsigned short *)ptr = v
#else
#define __VFPRINTF_CHAR_PTR_ARG ARGTYPE_UnsignedIntPtrArg
#define __VFPRINTF_WRITE_PTR_ARG_CHAR(a, ptr, v) *(unsigned int *)ptr = v
#endif
#else
#define __VFPRINTF_CHAR char
#define __VFSCANF_CHAR unsigned char
#define __VFPRINTF_USER_FORMAT_EXTENDER __user_format_extender
#define __VFPRINTF_PUTC __putc
#define __VFPRINTF_GETC __getc
#define __VFPRINTF_PRINT_PADDING __RAL_print_padding
#define __VFPRINTF_PRINT_WIDE_STRING __RAL_print_wide_string
#define __VFPRINTF_PRE_PADDING __RAL_pre_padding
#define __VFPRINTF_CHAR_PTR_ARG ARGTYPE_CharPtrArg
#define __VFPRINTF_WRITE_PTR_ARG_CHAR(a, ptr, v) *(unsigned char *)ptr = v
#endif

typedef struct __printf_tag
{
  size_t charcount;
  size_t maxchars;
  __VFPRINTF_CHAR *string;
  int (*output_fn)(int, struct __printf_tag *ctx);
} __printf_t;

typedef struct
{
  __VFPRINTF_CHAR is_string;
} __scanf_t;

typedef struct
{
  __VFPRINTF_CHAR is_string;
  __VFPRINTF_CHAR *string;
}  __string_scanf_t;

typedef struct
{
  __VFPRINTF_CHAR is_string;
  int (*getc_fn)(void);
  int (*ungetc_fn)(int);
} __stream_scanf_t;

int __putc_to_string(int c, __printf_t *ctx);

// Printer formatting flags.
#define FORMAT_LONG                 (1<<0)
#define FORMAT_LONG_LONG            (1<<1)
#define FORMAT_SHORT                (1<<2)
#define FORMAT_CHAR                 (1<<3)
#define FORMAT_LEFT_JUSTIFY         (1<<4)
#define FORMAT_SIGNED               (1<<5)
#define FORMAT_SPACE                (1<<6)
#define FORMAT_ALTERNATIVE          (1<<7)
#define FORMAT_HAVE_PRECISION       (1<<8)
#define FORMAT_ZERO_FILL            (1<<9)
#define FORMAT_FLOAT_E              (1<<10)
#define FORMAT_FLOAT_F              (1<<11)
#define FORMAT_ENGINEERING          (1<<12)
#define FORMAT_CAPITALS             (1<<13)
#define FORMAT_INPUT_SIGNED         (1<<14)
#define FORMAT_TICK                 (1<<15)   // POSIX.1 extension

// Combinations of flags used internally...
#define FORMAT_NEGATIVE             (FORMAT_SIGNED | FORMAT_SPACE)
#define FORMAT_FLOAT_G              (FORMAT_FLOAT_E | FORMAT_FLOAT_F)

// Helper variables.
extern __CODE const char __RAL_hex_uc[16];
extern __CODE const char __RAL_hex_lc[16];

// Standard formatting routine.
#if defined(__VFxxxxx_WCHAR)
int __vfprintf_wchar(__printf_t *p, __MEMSPACE const wchar_t *fmt, ARGTYPE args);
int __vfscanf_wchar(__scanf_t *p, __MEMSPACE const wchar_t *fmt, ARGTYPE argv);
#elif defined(__VFxxxxx_C)
int __vfprintf_c(__printf_t *p, __MEMSPACE const char *fmt, ARGTYPE args);
int __vfscanf_c(__scanf_t *p, __MEMSPACE const unsigned char *fmt, ARGTYPE argv);
#else
int __vfprintf(__printf_t *p, __MEMSPACE const char *fmt, ARGTYPE args);
int __vfscanf(__scanf_t *p, __MEMSPACE const unsigned char *fmt, ARGTYPE argv);
#endif

// Should be somewhere else...
extern void (*__VFPRINTF_USER_FORMAT_EXTENDER)(__VFPRINTF_CHAR ch, __printf_t *ctx, int flags, int width, int precision, ARGTYPE *args);

void __VFPRINTF_PRINT_PADDING(int ch, int n, __printf_t *ctx);
void __VFPRINTF_PRE_PADDING(int flags, int width, __printf_t *ctx);
void __VFPRINTF_PUTC(__printf_t *ctx, __VFPRINTF_CHAR ch);

#ifdef __cplusplus
}
#endif

#endif
