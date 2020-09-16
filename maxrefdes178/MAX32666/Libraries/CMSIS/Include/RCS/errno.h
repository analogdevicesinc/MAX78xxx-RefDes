// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __errno_h
#define __errno_h

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __ARM_EABI__
extern const int __aeabi_EDOM;
extern const int __aeabi_EILSEQ;
extern const int __aeabi_ERANGE;
#define EDOM     (__aeabi_EDOM)
#define EILSEQ   (__aeabi_EILSEQ)
#define ERANGE   (__aeabi_ERANGE)

#else

#define EDOM     0x01
#define EILSEQ   0x02
#define ERANGE   0x03

#endif

#define EHEAP    0x04


/*! \brief No memory available \ingroup Error numbers \synopsis

  \desc \b \this - no memory can be allocated by a function in the library.  Note that
  \b malloc, \b calloc, and \b realloc do not set \b errno to \b ENOMEM on failure,
  but other library routines (such as \b duplocale) may set \b errno to \b ENOMEM
  when memory allocation fails.
*/
#define ENOMEM    0x05


/*! \brief Invalid argument \ingroup Error numbers \synopsis

  \desc \b \this - An argument was invalid, or a combination of arguments was invalid.
*/
#define EINVAL    0x06


#ifdef __ARM_EABI__
volatile int *__aeabi_errno_addr(void);
#else
volatile int *__errno(void);
#endif

#ifdef __ARM_EABI__
#define errno (*__aeabi_errno_addr())
#else
#define errno (*__errno())
#endif

#ifdef __CROSSWORKS_DOCUMENTATION

/*! \brief Domain error \ingroup Error numbers \synopsis

  \desc \b \this - an input argument is outside the defined domain of a
    mathematical function.
*/
#define EDOM   ...


/*! \brief Illegal byte sequence \ingroup Error numbers \synopsis

  \desc \b \this - A wide-character code has been detected that does not correspond to a valid character, 
  or a byte sequence does not form a valid wide-character code.
*/
#define EILSEQ   ...


/*! \brief Result too large or too small \ingroup Error numbers \synopsis

  \desc \b \this - the result of the function is too large (overflow) or too small (underflow) to be represented 
  in the available space.
*/
#define ERANGE   ...


/*! \brief Last-set error condition \ingroup Macros \synopsis

  \desc \b \this is treated as an writable l-value, but the implementation of
  how the l-value is read an written is hidden from the user.

  The value of \b \this is zero at program startup, but is never set to 
  zero by any library function. The value of \b \this may be set to a nonzero 
  value by a library function, and this effect is documented in each function that 
  does so.

  \note The ISO standard does not specify whether \b \this is a macro or an identifier 
  declared with external linkage. Portable programs must not make assumptions 
  about the implementation of \b \this.

  In this implementation, \b \this expands to a function call to \b __errno
  (MSP430, AVR, MAXQ) or \b __aeabi_errno_addr (ARM) that returns a pointer to a
  volatile \b int. This function can be implemented by the application to provide
  a thread-specific \b errno.

*/
int errno;

#endif

#ifdef __cplusplus
}
#endif

#endif
