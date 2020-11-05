// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __debug_stdio_H
#define __debug_stdio_H

#include <debugio.h>

#define FILE     DEBUG_FILE

#ifndef SEEK_CUR
#define SEEK_CUR DEBUG_SEEK_CUR
#define SEEK_END DEBUG_SEEK_END
#define SEEK_SET DEBUG_SEEK_SET
#endif

#define stdin    DEBUG_STDIN
#define stdout   DEBUG_STDOUT
#define stderr   DEBUG_STDERR

#define fopen    debug_fopen
#define fflush   debug_fflush
#define fclose   debug_fclose
#define fprintf  debug_fprintf
#define vfprintf debug_vfprintf
#define printf   debug_printf
#define vprintf  debug_vprintf
#define fscanf   debug_fscanf
#define scanf    debug_scanf
#define vscanf   debug_vscanf
#define vfscanf  debug_vfscanf
#define getchar  debug_getchar
#define getc     debug_fgetc
#define gets     debug_gets
#define fgetc    debug_fgetc
#define fgets    debug_fgets
#define fputc    debug_fputc
#define fputs    debug_fputs
#ifndef putchar
#define putchar  debug_putchar
#endif
#define puts     debug_puts
#define fread    debug_fread
#define fwrite   debug_fwrite
#define fseek    debug_fseek
#define ftell    debug_ftell
#define rewind   debug_rewind
#define filesize debug_filesize
#define clearerr debug_clearerr
#define feof     debug_feof
#define ferror   debug_ferror
#define exit     debug_exit
#define time     debug_time

#endif

