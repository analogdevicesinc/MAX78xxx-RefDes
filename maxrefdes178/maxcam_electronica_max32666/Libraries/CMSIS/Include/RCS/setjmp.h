// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __setjmp_h
#define __setjmp_h

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __CROSSWORKS_DOCUMENTATION

/*! \brief Array capable of storing the information of a calling environment \ingroup Types \synopsis

  \desc The type \b \this is an array type suitable for holding the calling environment 
  saved by a call to \b setjmp which can be restored by a call to \b longjmp. 
*/

typedef unsigned jmp_buf[];

#endif

#if defined(__CROSSWORKS_AVR)
#if __AVR_FLASH_SIZE > 128
typedef unsigned char jmp_buf[25];  // R2-R19, R28-R29, PC, SP: 8M devices
#else
typedef unsigned char jmp_buf[24];  // R2-R19, R28-R29, PC, SP: 8K-128K devices 
#endif
#elif defined(__CROSSWORKS_MSP430)
#if __MSP430_CODE_ADDRESS_BITS > 16
typedef unsigned jmp_buf[11];       // R4-R11, PC, SP
#else
typedef unsigned jmp_buf[10];       // R4-R11, PC, SP
#endif
#elif defined(__CROSSWORKS_MAXQ)
typedef unsigned jmp_buf[14];       // A[0]-A[3], A[8]-A[15], SP, PC
#elif defined(__CROSSWORKS_MAXQ30)
typedef unsigned jmp_buf[14];       // A[0]-A[3], A[8]-A[15], SP, PC
#elif defined(__CROSSWORKS_ARM) || defined(__SES_ARM)

#if defined(__ARM_ARCH_VFP__) || defined(__ARM_ARCH_VFP3_D16__) || defined(__ARM_ARCH_VFP3_D32__) || defined(__ARM_ARCH_VFPV4_D16__) || defined(__ARM_ARCH_FPV4_SP_D16__) || defined(__ARM_ARCH_FPV5_SP_D16__) || defined(__ARM_ARCH_FPV5_D16__)
typedef unsigned long long jmp_buf[14];  // R4-R14, D8-D15
#else
typedef unsigned long jmp_buf[11];  // R4-R14
#endif


#endif

/*! \brief Save calling environment for non-local jump \ingroup Functions \synopsis

  \desc \b \this saves its calling environment in the \a env for later use by the \b longjmp function.

  On return from a direct invocation \b \this returns the value zero. 
  On return from a call to the \b longjmp function, the \b \this
  returns a nonzero value determined by the call to \b longjmp.

  The environment saved by a call to \b setjmp 
  consists of information sufficient for a call to the \b longjmp function to return 
  execution to the correct block and invocation of that block, were it called 
  recursively.
*/
int setjmp(jmp_buf __env);


#ifdef __CROSSWORKS_DOCUMENTATION
/*! \brief Restores the saved environment \ingroup Functions \synopsis

  \desc \b \this restores the environment saved by \b setjmp in the corresponding
  \a env argument. If there has 
  been no such invocation, or if the function containing the invocation of \b setjmp
  has terminated execution in the interim, the behavior of \b longjmp is undefined.

  After \b \this is completed, program execution continues as if the corresponding 
  invocation of \b setjmp had just returned the value specified by \a val. 

  \note \b \this cannot cause \b setjmp to return the value 0; if 
  \a val is 0, \b setjmp returns the value 1.

  Objects of automatic storage allocation that are local to the function containing 
  the invocation of the corresponding \b setjmp that do not have \b volatile qualified 
  type and have been changed between the \b setjmp invocation and \b this
  call are indeterminate.
*/
void longjmp(jmp_buf __env, int __val);
#else
#if defined(__CROSSWORKS_ARM) || defined(__SES_ARM)
__attribute__((noreturn))
#endif
void longjmp(jmp_buf __env, int __val);
#endif

#ifdef __cplusplus
}
#endif

#endif
