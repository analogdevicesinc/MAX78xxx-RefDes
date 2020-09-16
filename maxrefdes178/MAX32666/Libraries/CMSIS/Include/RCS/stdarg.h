// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __stdarg_H
#define __stdarg_H

// Get definition of __va_list
#include "__crossworks.h"

#ifdef __CROSSWORKS

typedef __va_list va_list;
__va_list __va_base(int);  // intrinsic

#define va_start(ap,parmN) \
    ((void)((ap) = __va_base(0)))

#define va_arg(ap,type) \
    __va_arg(ap, type)

#define va_copy(dest,src) \
    ((void)((dest) = (src)))

#define va_end(ap) \
    ((void)((ap) = (void *)0))

#else

typedef __va_list va_list;

#define va_start(v,l) \
    __builtin_va_start((v),l) 

#define va_arg \
    __builtin_va_arg

#define va_copy(d,s) \
    __builtin_va_copy((d),(s))

#define va_end(ap) \
    __builtin_va_end(ap)

#endif

#ifdef __CROSSWORKS_DOCUMENTATION

/*! \brief Start access to variable arguments \ingroup Macros \synopsis

  \desc \b \this initializes \a ap for subsequent use by the \b va_arg 
  and \b va_end macros.

  The parameter \a parmN is the identifier of the last fixed parameter in 
  the variable parameter list in the function definition (the one just before 
  the '\b, ...\b').

  The behaviour of \b va_start and \b va_arg is undefined if the parameter 
  \a parmN is declared with the \b register storage class, with a function 
  or array type, or with a type that is not compatible with the type that results 
  after application of the default argument promotions.

  \b \this must be invoked before any access to the unnamed arguments.

  \b \this and \b va_copy must not be invoked to reinitialize \a ap
  without an intervening invocation of the \b va_end macro for the same \a ap.
*/
void va_start(va_list ap, paramN);


/*! \brief Get variable argument value \ingroup Macros \synopsis

  \desc \b \this expands to an expression that has the specified type and the 
  value of the \a type argument. The \a ap parameter must have been initialized 
  by \b va_start or \b va_copy, without an intervening invocation of \b va_end. 
  You can create a pointer to a \b va_list and pass that pointer to another 
  function, in which case the original function may make further use of the original 
  list after the other function returns.

  Each invocation of the \b \this macro modifies \a ap so that the 
  values of successive arguments are returned in turn. The parameter type must 
  be a type name such that the type of a pointer to an object that has the specified 
  type can be obtained simply by postfixing a \b * to \b type. 

  If there is no actual next argument, or if type is not compatible with the 
  type of the actual next argument (as promoted according to the default argument 
  promotions), the behavior of \b va_arg is undefined, except for the following 
  cases:
  
  \item one type is a signed integer type, the other type is the corresponding unsigned 
  integer type, and the value is representable in both types;

  \item one type is pointer to \b void and the other is a pointer to a character type.

  The first invocation of the \b \this macro after that of the \b va_start
  macro returns the value of the argument after that specified by \b parmN. 
  Successive invocations return the values of the remaining arguments in succession. 
*/
type va_arg(va_list ap, type);


/*! \brief Finish access to variable arguments \ingroup Macros \synopsis

  \desc \b \this indicates a normal return from the function whose variable argument 
  list \a ap was initialised by \b va_start or \b va_copy. The \b \this
  macro may modify \b ap so that it is no longer usable without being reinitialized 
  by \b va_start or \b va_copy. If there is no corresponding invocation 
  of \b va_start or \b va_copy, or if \b va_end is not invoked before 
  the return, the behavior is undefined.
*/
void va_end(va_list ap);


/*! \brief Copy var args \ingroup Macros \synopsis

  \desc \b \this initializes \b dest as a copy of \b src, as if the 
  \b va_start macro had been applied to \b dest followed by the same sequence 
  of uses of the \b va_arg macro as had previously been used to reach the 
  present state of \b src. Neither the \b va_copy nor \b va_start
  macro shall be invoked to reinitialize \b dest without an intervening invocation 
  of the \b va_end macro for the same \b dest.
*/
void va_copy(va_list dest, val_list src);


#endif

#endif
