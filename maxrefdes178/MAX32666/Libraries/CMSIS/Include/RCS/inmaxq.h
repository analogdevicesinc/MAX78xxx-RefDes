// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __inmaxq_H
#define __inmaxq_H

/*! \brief Swap nibbles within a byte \ingroup Data manipulation \synopsis

  \desc \b \this returns \a x with the high and low nibbles swapped.
  \b \this is an intrinsic function, produces inline code, and you
  can use it on any MAXQ30 device.
*/
#pragma intrinsic(__swap_nibbles)
unsigned char __swap_nibbles(unsigned char);

// Swap byte order within a word, 1234->4321
#pragma intrinsic(__swap_endian)
unsigned __swap_endian(unsigned x);

/*! \brief Swap bytes within a word \ingroup Data manipulation \synopsis

  \desc \b \this returns \a x with the high and low bytes swapped.
  \b \this is an intrinsic function, produces inline code, and you
  can use it on any MAXQ30 device.
*/
#pragma intrinsic(__swap_bytes)
unsigned __swap_bytes(unsigned x);

/*! \brief Insert an instruction into code \ingroup Miscellaneous functions \synopsis

  \desc \b \this inserts \a opcode into the code stream and can
  be used to insert special instructions directly into function code. \a opcode must
  be a compile-time  constant.

  \b \this is an intrinsic function, produces inline code, and you can use it
  on any MAXQ30 device.
*/
#pragma intrinsic(__insert_opcode)
void __insert_opcode(unsigned short op);

/*! \brief Disable interrupts and return previous enable state \ingroup Status register manipulation functions \synopsis

  \desc \b \this disables global interrupts by clearing the \b IGE
  bit in the interrupt control register IC and returns the value of the interrupt 
  control register before the \b IGE bit is cleared. You can restore 
  the state of the \b IGE bit from the value returned from \b \this 
  by using the \b __restore_interrupt intrinsic function.

  \b \this is an intrinsic function, produces inline code, and you can
  use it on any MAXQ30 device.

  \example

  \code
void critical_function(void)
{
// \em{Disable interrupts and save interrupt enable state}
unsigned char state = __disable_interrupt();

// \em{Critical processing here}

// \em{Restore interrupt enable state from saved value}
__restore_interrupt(state);
} \endcode
*/
#pragma intrinsic(__disable_interrupt)
unsigned __disable_interrupt(void);

/*! \brief Enable interrupts and return previous enable state \ingroup Status register manipulation functions \synopsis

  \desc \b \this enables global interrupts by setting the \b IGE
  bit in the interrupt control register \b IC and returns the value of the 
  interrupt control register before the \b IGE bit is set. You can 
  restore the state of the \b IGE bit from the value returned from 
  \b \this by using the \b __restore_interrupt intrinsic function.

  \b \this is an intrinsic function, produces inline code, and you can
  use it on any MAXQ30 device.

  \example
  
\code void run_with_interrupts_enabled(void)
{
// \em{Enable interrupts and save interrupt enable state}
unsigned char state = __enable_interrupt();

// \em{Processing with interrupts enabled here}

// em{Restore interrupt enable state from saved value}
__restore_interrupt(state);
} \endcode
*/
#pragma intrinsic(__enable_interrupt)
unsigned __enable_interrupt(void);

#ifdef __CROSSWORKS_DOCUMENTATION

/*! \brief Restore interrupt enable flag state \ingroup Status register manipulation functions \synopsis

  \desc \b \this restores the state of the IGE bit in the interrupt control register
  IC to the value saved in \a state. 

  The value of \a state is returned from the \b __disable_interrupt and 
  \b __enable_interrupt intrinsic 
  functions and is simply the previous value of the interrupt
  control register.

  \b \this is an intrinsic function and produces inline code.

  \sa \ref __disable_interrupt, \ref __enable_interrupt
*/
void __restore_interrupt(unsigned state);

#else
#define __restore_interrupt(x)  \
  do { if (x & 0x01) __enable_interrupt(); else __disable_interrupt(); } while (0)
#endif

/*! \brief Insert a NOP instruction into code \ingroup Miscellaneous functions \synopsis

  \desc \b \this inserts a NOP instruction into the function code.

  \b \this is an intrinsic function and produces inline code.
  You can use \b \this on any MAXQ30 device.
*/
#define __no_operation() __insert_opcode(0xda3a)

#endif
