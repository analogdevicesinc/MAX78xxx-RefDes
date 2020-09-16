// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __debugio_H
#define __debugio_H

#include "__crossworks.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_FILE void

#define DEBUG_SEEK_CUR 1
#define DEBUG_SEEK_END 2
#define DEBUG_SEEK_SET 0

#define DEBUG_STDIN  ((DEBUG_FILE *)0)
#define DEBUG_STDOUT ((DEBUG_FILE *)1)
#define DEBUG_STDERR ((DEBUG_FILE *)2)


/*! \brief Opens a file on the host PC \ingroup File Functions \synopsis

  \desc \b \this opens the \a filename on the host PC and returns a stream or \b 0 if the open fails.
  The \a filename is a host PC filename which is opened relative to the debugger working directory.
  The \a mode is a string containing one of:

  \item \b r open file for reading.
  \item \b w create file for writing.
  \item \b a open or create file for writing and position at the end of the file.
  \item \b r+ open file for reading and writing.
  \item \b w+ create file for reading and writing.
  \item \b a+ open or create text file for reading and writing and position at the end of the file.

  followed by one of:

  \item \b t for a text file.
  \item \b b for a binary file.

  \b \this returns a stream that can be used to access the file or \b 0 if the open fails.
*/
DEBUG_FILE *debug_fopen(const char *filename, const char *mode); 


/*! \brief Flushes buffered output \ingroup File Functions \synopsis

  \desc \b \this flushes any buffered output of the \a stream.

  \b \this returns 0 on success or -1 if there was an error.
*/
int debug_fflush(DEBUG_FILE *stream);


/*! \brief Closes an open stream \ingroup File Functions \synopsis

  \desc \b \this flushes any buffered output of the \a stream and then closes the stream.

  \b \this returns 0 on success or -1 if there was an error.
*/
int debug_fclose(DEBUG_FILE *stream);


/*! \brief Formatted write \ingroup File Functions \synopsis

  \desc \b \this writes to \a stream, under control of the 
  string pointed to by \a format that specifies how subsequent arguments are 
  converted for output. The \a format string is a standard C printf format string. 
  The actual formatting is performed on the host by the debugger 
  and therefore \b \this consumes only a very small amount of
  code and data space, only the overhead to call the function. 

  If there are insufficient arguments for the format, the behavior is undefined. 
  If the format is exhausted while arguments remain, the excess arguments are 
  evaluated but are otherwise ignored.

  \b \this returns the number of characters transmitted, or a negative 
  value if an output or encoding error occurred.
*/
int debug_fprintf(DEBUG_FILE *stream, const char *format, ...);


/*! \brief Formatted write \ingroup Debug Terminal Output Functions \synopsis

  \desc \b \this writes to the \b Debug \b Terminal, under control 
  of the string pointed to by \b format that specifies how subsequent arguments 
  are converted for output. The \a format string is a standard C printf format string. 
  The actual formatting is performed on the host by 
  the debugger and therefore \b \this consumes only a very small amount of
  code and data space, only the overhead to call the function.

  If there are insufficient arguments for the format, the behavior is undefined. 
  If the format is exhausted while arguments remain, the excess arguments are 
  evaluated but are otherwise ignored.

  \b \this returns the number of characters transmitted, or a negative 
  value if an output or encoding error occurred.
*/
int debug_printf(const char *format, ...);  


/*! \brief Formatted read \ingroup File Functions \synopsis

  \desc \b \this reads from the input \a stream, under control of the string 
  pointed to by \a format, that specifies how subsequent arguments are converted 
  for input. The \a format string is a standard C scanf format string.
  The actual formatting is performed on the host by the debugger and 
  therefore \b \this consumes only a very small amount of
  code and data space, only the overhead to call the function.

  If there are insufficient arguments for the format, the behavior is undefined. 
  If the format is exhausted while arguments remain, the excess arguments are 
  evaluated but are otherwise ignored.

  \b \this returns number of characters read, or a negative value 
  if an output or encoding error occurred.
*/
int debug_fscanf(DEBUG_FILE *stream, const char *format, ...);


/*! \brief Formatted read \ingroup Debug Terminal Input Functions \synopsis

  \desc \b \this reads from the \b{Debug Terminal},
  under control of the string pointed to by \a format that specifies how subsequent 
  arguments are converted for input. The \a format string is a standard C scanf format string.
  The actual formatting is performed on the host by the debugger and 
  therefore \b \this consumes only a very small amount of
  code and data space, only the overhead to call the function.

  If there are insufficient arguments for the format, the behavior is undefined. 
  If the format is exhausted while arguments remain, the excess arguments are 
  evaluated but are otherwise ignored.

  \b \this returns number of characters read, or a negative value if 
  an output or encoding error occurred.
*/
int debug_scanf(const char *format, ...);  


/*! \brief Line-buffered character read \ingroup Debug Terminal Input Functions\synopsis

  \desc \b \this reads one character from the \b Debug \b Terminal.
  This function uses line input and will therefore block until characters are
  available and ENTER has been pressed.

  \b \this returns the character that has been read.
*/
int debug_getchar(void);


/*! \brief String read \ingroup Debug Terminal Input Functions\synopsis

  \desc \b \this reads a string from the Debug Terminal in memory pointed at by \a s.
  This function will block until ENTER has been pressed.

  \b \this returns the value of \a s.
*/
char *debug_gets(char *s);    


/*! \brief Line-buffered integer read \ingroup Debug Terminal Input Functions \synopsis

  \desc \b \this reads an integer from the \b Debug \b Terminal. If the number 
  starts with \b{0x} it is interpreted as a hexadecimal number, if 
  it starts with \b{0} it is interpreted as an octal number, if it 
  starts with \b{0b} it is interpreted as a binary number, otherwise 
  it is interpreted as a decimal number. The number is written to the integer
  object pointed to by \a i.

  \b \this returns zero on success or -1 on error.
*/
int debug_geti(int *i);


/*! \brief Line-buffered unsigned integer \ingroup Debug Terminal Input Functions \synopsis

  \desc \b \this reads an unsigned integer from the \b Debug \b Terminal. If the number 
  starts with \b 0x it is interpreted as a hexadecimal number, if 
  it starts with \b 0 it is interpreted as an octal number, if it 
  starts with \b 0b it is interpreted as a binary number, otherwise 
  it is interpreted as a decimal number. The number is written to the unsigned integer
  object pointed to by \a u.

  \b \this returns zero on success or -1 on error.
*/
int debug_getu(unsigned *u);


/*! \brief Line-buffered long read \ingroup Debug Terminal Input Functions \synopsis

  \desc \b \this reads a long from the \b Debug \b Terminal. If the number 
  starts with \b 0x it is interpreted as a hexadecimal number, if 
  it starts with \b 0 it is interpreted as an octal number, if it 
  starts with \b0b it is interpreted as a binary number, otherwise 
  it is interpreted as a decimal number. The number is written to the long
  object pointed to by \a l.

  \b \this returns zero on success or -1 on error.
*/
int debug_getl(long *l);


/*! \brief Line-buffered unsigned long read \ingroup Debug Terminal Input Functions \synopsis

  \desc \b \this reads an unsigned long from the \b Debug \b Terminal. If the number 
  starts with \b 0x it is interpreted as a hexadecimal number, if 
  it starts with \b 0 it is interpreted as an octal number, if it 
  starts with \b 0b it is interpreted as a binary number, otherwise 
  it is interpreted as a decimal number. The number is written to the long
  object pointed to by \a ul.

  \b \this returns zero on success or -1 on error.
*/
int debug_getul(unsigned long *ul);


/*! \brief Line-buffered float read \ingroup Debug Terminal Input Functions \synopsis

  \desc \b \this reads an float from the \b Debug \b Terminal. The number is written to the float
  object pointed to by \a f.

  \b \this returns zero on success or -1 on error.
*/
int debug_getf(float *f);


/*! \brief Line-buffered double read \ingroup Debug Terminal Input Functions \synopsis

  \desc \b \this reads a double from the \b Debug \b Terminal. The number is written to the double
  object pointed to by \a d.

  \b \this returns zero on success or -1 on error.
*/
int debug_getd(double *);


/*! \brief Line-buffered long long read \ingroup Debug Terminal Input Functions \synopsis

  \desc \b \this reads a long long from the \b Debug \b Terminal. If the number 
  starts with \b 0x it is interpreted as a hexadecimal number, if 
  it starts with \b 0 it is interpreted as an octal number, if it 
  starts with \b 0b it is interpreted as a binary number, otherwise 
  it is interpreted as a decimal number. The number is written to the long long
  object pointed to by \a ll.

  \b \this returns zero on success or -1 on error.
*/
int debug_getll(long long *ll);


/*! \brief Line-buffered unsigned long long read \ingroup Debug Terminal Input Functions \synopsis

  \desc \b \this reads an unsigned long long from the \b Debug \b Terminal. If the number 
  starts with \b 0x it is interpreted as a hexadecimal number, if 
  it starts with \b 0 it is interpreted as an octal number, if it 
  starts with \b 0b it is interpreted as a binary number, otherwise 
  it is interpreted as a decimal number. The number is written to the long long
  object pointed to by \a ull.

  \b \this returns zero on success or -1 on error.
*/
int debug_getull(unsigned long long *ull);
   

/*! \brief Read a character from a stream \ingroup File Functions\synopsis

  \desc \b \this reads and returns the next character on \a stream or -1 if no character is available.
*/
int debug_fgetc(DEBUG_FILE *stream);


/*! \brief Read a string \ingroup File Functions \synopsis

  \desc \b \this reads at most \a n-1 characters or the characters up to (and including) 
  a newline from the input \a stream into the array pointed to by \a s. A null character is written to the
  array after the input characters.

  \b \this returns \a s on success, or 0 on error or end of file.
*/
char *debug_fgets(char *s, int n, DEBUG_FILE *stream);


/*! \brief Write a character \ingroup File Functions\synopsis

  \desc \b \this writes the character \a c to the output \a stream.

  \b \this returns the character written or -1 if an error occurred.
*/
int debug_fputc(int c, DEBUG_FILE *stream);  
                      

/*! \brief Write a string \ingroup File Functions \synopsis

  \desc \b \this writes the string pointed to by \a s to the output
  \a stream and appends a new-line character. The terminating null character is not written.

  \b \this returns -1 if a write error occurs; otherwise it returns a nonnegative value.
*/
int debug_fputs(const char *s, DEBUG_FILE *stream);


/*! \brief Write a character \ingroup Debug Terminal Output Functions \synopsis

  \desc \b \this write the character \a c to the Debug Terminal.

  \b \this returns the character written or -1 if a write error occurs.  
*/
int debug_putchar(int c);


/*! \brief Write a string \ingroup Debug Terminal Output Functions \synopsis

  \desc \b \this writes the string \a s to the Debug Terminal followed by a new-line character.

  \b \this returns -1 if a write error occurs, otherwise it returns a nonnegative value.
*/
int debug_puts(const char *);


/*! \brief Read data \ingroup File Functions \synopsis

  \desc \b \this reads from the input \a stream into the array \a ptr at most \a nobj
  objects of size \a size.

  \b \this returns the number of objects read. If this number is different from \a nobj then
  \b debug_feof and \b debug_ferror can be used to determine status.
*/
int debug_fread(void *ptr, int size, int nobj, DEBUG_FILE *stream);


/*! \brief Write data \ingroup File Functions \synopsis

  \desc \b \this write to the output \a stream from the array \a ptr at most \a nobj
  objects of size \a size.

  \b \this returns the number of objects written. If this number is different from \a nobj then
  \b debug_feof and \b debug_ferror can be used to determine status.
*/
int debug_fwrite(const void *ptr, int size, int nobj, DEBUG_FILE *stream);


/*! \brief Set file position \ingroup File Functions \synopsis

  \desc \b \this sets the file position for the \a stream. A subsequent 
  read or write will access data at that position. The \a origin can be one 
  of:

  \item \b 0 sets the position to \b offset bytes from the beginning of the file.
  \item \b 1 sets the position to \b offset bytes relative to the current position.
  \item \b 2 sets the position to \b offset bytes from the end of the file.

  Note that for text files \b offset must be zero. 
  
  \b \this returns zero on success, non-zero on error.
*/
int debug_fseek(DEBUG_FILE *stream, long offset, int origin);


/*! \brief Return file position \ingroup File Functions \synopsis

  \desc \b \this returns the current file position of the \a stream.

  \b \this returns -1 on error.
*/
long debug_ftell(DEBUG_FILE *stream);


/*! \brief Set file position to the beginning \ingroup File Functions \synopsis

  \desc \b \this sets the current file position of the \a stream to
  the beginning of the file and clears any error and end of file conditions.
*/
void debug_rewind(DEBUG_FILE *stream);


/*! \brief Return the size of a file \ingroup File Functions \synopsis
  
  \desc \b \this returns the size of the file associated with the \a stream in bytes.

  \b \this returns -1 on error.
*/
int debug_filesize(DEBUG_FILE *stream);


/*! \brief Clear error indicator \ingroup File Functions \synopsis

  \desc \b \this clears any error indicator or end of file condition for the \a stream.
*/
void debug_clearerr(DEBUG_FILE *stream);


/*! \brief Check end of file condition \ingroup File Functions \synopsis

  \desc \b \this returns non-zero if the end of file condition is set for the \a stream.
*/
int debug_feof(DEBUG_FILE *stream);


/*! \brief Check error indicator \ingroup File Functions \synopsis

  \desc \b \this returns non-zero if the error indicator is set for the \a stream.
*/
int debug_ferror(DEBUG_FILE *stream);


/*! \brief Polled character read \ingroup Debug Terminal Input Functions \synopsis

  \desc \b \this polls the Debug Terminal for a character and returns a non-zero value if 
  a character is available or 0 if not.
*/
int debug_kbhit(void);


/*! \brief Blocking character read \ingroup Debug Terminal Input Functions \synopsis

  \desc \b \this reads one character from the Debug Terminal.
  This function will block until a character is available.
*/
int debug_getch(void);


/*! \brief Stop and report error \ingroup Debugger Functions \synopsis

  \desc \b \this causes the debugger to stop the target, 
  position the cursor at the line that called \this, 
  and display the null-terminated string pointed to by \a error.
*/
void debug_runtime_error(const char *error);


/*! \brief Stop target \ingroup Debugger Functions \synopsis

  \desc \b \this causes the debugger to stop the target and
  position the cursor at the line that called \this.
*/
void debug_break(void);

/*! \brief Stop debugging \ingroup Debugger Functions \synopsis

  \desc \b \this causes the debugger to exit and \a result is returned to the user.
*/
void debug_exit(int result);

/*! \brief Stop debugging \ingroup Debugger Functions \synopsis

  \desc \b \this causes the debugger to exit and a failure result is returned to the user.
*/
void debug_abort(void);

/*! \brief get time \ingroup Misc Functions\synopsis
  \desc \b \this returns the number of seconds elapsed since midnight (00:00:00), 
  January 1, 1970, coordinated universal time (UTC), according to the system clock 
  of the host computer. The return value is stored in \b *ptr if \b ptr is not NULL.
*/
long debug_time(long *ptr);


#if defined(__CROSSWORKS_AVR) || defined(__CROSSWORKS_MAXQ) || defined(__CROSSWORKS_MAXQ30)


/*! \brief Formatted write \ingroup File Functions \synopsis
    
    \desc \b \this is equivalent to \b debug_fprintf with the format string in code memory.
*/
int debug_fprintf_c(DEBUG_FILE *stream, __code const char *format, ...);


/*! \brief Formatted write \ingroup Debug Terminal Output Functions \synopsis

  \desc \b \this is equivalent to \b debug_printf with the format string in code memory.
*/
int debug_printf_c(__code const char *format, ...);  


/*! \brief Formatted read \ingroup File Functions \synopsis

  \desc \b \this is equivalent to \b debug_fscanf with the format string in code memory.
*/
int debug_fscanf_c(DEBUG_FILE *stream, __code const char *format, ...);


/*! \brief Formatted read \ingroup Debug Terminal Input Functions \synopsis

  \desc \b \this is equivalent to \b debug_scanf with the format string in code memory.
*/
int debug_scanf_c(__code const char *format, ...);  


#endif


/*! \brief Formatted write \ingroup File Functions \synopsis

  \desc \b \this is equivalent to \b debug_fprintf with arguments passed using \b stdarg.h
  rather than a variable number of arguments.
 */
int debug_vfprintf(DEBUG_FILE *stream, const char *format, __va_list);


/*! \brief Formatted write \ingroup Debug Terminal Output Functions \synopsis

  \desc \b \this is equivalent to \b debug_printf with arguments passed using \b stdarg.h
  rather than a variable number of arguments.
*/
int debug_vprintf(const char *format, __va_list);


/*! \brief Formatted read \ingroup File Functions \synopsis

  \desc \b \this is equivalent to \b debug_fscanf with arguments passed using \b stdarg.h
  rather than a variable number of arguments.
*/
int debug_vfscanf(DEBUG_FILE *stream, const char *format, __va_list);


/*! \brief Formatted read \ingroup Debug Terminal Input Functions \synopsis

  \desc \b \this is equivalent to \b debug_scanf with arguments passed using \b stdarg.h
  rather than a variable number of arguments.
*/
int debug_vscanf(const char *format, __va_list);


/*! \brief Push a character \ingroup File Functions \synopsis

  \desc \b \this pushes the character \a c onto the input \a stream. If
  successful \a c is returned, otherwise -1 is returned.
*/
int debug_ungetc(int c, DEBUG_FILE *stream);


/*! \brief Return file position \ingroup File Functions \synopsis

  \desc \b \this is equivalent to \b debug_fseek .
*/
int debug_fgetpos(DEBUG_FILE *stream, long *pos);


/*! \brief Teturn file position \ingroup File Functions \synopsis

  \desc \b \this is equivalent to \b debug_fseek with 0 as the \b origin.
*/
int debug_fsetpos(DEBUG_FILE *stream, const long *pos);


/*! \brief Reopens a file on the host PC \ingroup File Functions \synopsis

  \desc \b \this is the same as \b debug_open except the file associated with the \a stream
  is closed and the opened file is then associated with the \a stream.
*/
DEBUG_FILE *debug_freopen(const char *filename, const char *mode, DEBUG_FILE *stream);


/*! \brief Display error \ingroup Misc Functions \synopsis

  \desc \b \this displays the optional string \a s on the \b Debug \b Terminal together with a string
  corresponding to the errno value of the last Debug IO operation.
*/
void debug_perror(const char *s);


/*! \brief Deletes a file on the host PC \ingroup File Functions \synopsis

  \desc \b \this removes the filename denoted by \a filename and
  returns \b 0 on success or \b -1 on error. The \a filename is a host PC 
  filename which is relative to the debugger working directory.
*/
int debug_remove(const char *filename);


/*! \brief Renames a file on the host PC \ingroup File Functions \synopsis

  \desc \b \this renames the file denoted by \a oldpath to
  \a newpath and returns zero on success or non-zero on error.
  The \a oldpath and \a newpath are host PC filenames which are relative to the 
  debugger working directory.
*/
int debug_rename(const char *oldfilename, const char *newfilename);


/*! \brief Open a temporary file \ingroup File Functions \synopsis

  \desc \b \this creates a temporary file on the host PC which is deleted when
  the stream is closed.
*/
DEBUG_FILE *debug_tmpfile(void);


/*! \brief Generate temporary filename \ingroup File Functions \synopsis

  \desc \b \this returns a unique temporary filename. If \a str is \b NULL then a static buffer is used 
  to store the filename, otherwise the filename is stored in \a str. On success a pointer to the string is returned, 
  on failure \b 0 is returned.
*/
char *debug_tmpnam(char *str);


/*! \brief Get environment variable value \ingroup Misc Functions \synopsis

  \desc \b \this returns the value of the environment variable \a name or
  0 if the environment variable cannot be found.
*/
char *debug_getenv(char *name);


/*! \brief Execute command \ingroup Misc Functions \synopsis

  \desc \b \this executes the \a command with the host command line
  interpreter and returns the commands exit status.
*/
int debug_system(char *command);


/*! \brief Get arguments \ingroup Debugger Functions \synopsis

  \desc \b \this stores the debugger command line arguments into the memory
  pointed at by \a buf up to a maximum of \a bufsize bytes. The command line is stored as 
  a C \b argc array of null terminated string and the number of entries is returned as the result.
*/
int debug_getargs(unsigned bufsize, unsigned char *buf);


/*! \brief Load debugging symbols \ingroup Debugger Functions \synopsis

  \desc \b \this instructs the debugger to load the debugging symbols in the file denoted
  by \a filename. The \a filename is a (macro expanded) host PC filename which is relative 
  to the debugger working directory. The \a address is the load address which is required for 
  debugging position independent executables, supply \b NULL for regular executables. 
  The \a breaksymbol is the name of a symbol in the filename to set a temporary breakpoint 
  on or \b NULL.
*/
void debug_loadsymbols(const char *filename, const void *address, const char *breaksymbol);


/*! \brief Unload debugging symbols \ingroup Debugger Functions \synopsis

  \desc \b \this instructs the debugger to unload the debugging symbols (previously loaded by a
  call to \b debug_loadsymbols) in the file denoted by \a filename. The \a filename is a host PC 
  filename which is relative to the debugger working directory.
*/
void debug_unloadsymbols(const char *filename);


/*! \brief Test if debug input/output is enabled \ingroup Debugger Functions \synopsis

  \desc \b \this returns non-zero if the debugger is connected - you can use
  this to test if a debug input/output functions will work.
*/
int debug_enabled(void);


#ifdef __cplusplus
}
#endif

#endif
