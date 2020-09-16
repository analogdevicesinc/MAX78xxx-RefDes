// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __libc_H
#define __libc_H

#include "__crossworks.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __printf_tag *__printf_tag_ptr;

int __getchar(void);
int __putchar(int, __printf_tag_ptr __ctx);
int __stdin_ungetc(int);

char *__xtoa(unsigned __val, char *__buf, unsigned __radix, int __neg);
char *__xltoa(unsigned long __val, char *__buf, unsigned __radix, int __neg);
char *__xlltoa(unsigned long long __val, char *__buf, unsigned __radix, int __neg);
int __digit(int __ch, int __radix);
unsigned long __strtoul(const char *__nsptr, char **__endptr, int __base);
unsigned long long __strtoull(const char *__nsptr, char **__endptr, int __base);
double __RAL_pow10(int __e);

void __heap_lock(void);
void __heap_unlock(void);
void __printf_lock(void);
void __printf_unlock(void);
void __scanf_lock(void);
void __scanf_unlock(void);
void __debug_io_lock(void);
void __debug_io_unlock(void);

int __open(const char *__filename, const char *__mode);
int __close(int __fildes);
int __flush(int __fildes);
int __write(int __fildes, const unsigned char *__buf, unsigned __len);
int __read(int __fildes, unsigned char *__buf, unsigned __len);
long __seek(int __fildes, long __offset, int __whence);

#ifdef __cplusplus
}
#endif

#endif

