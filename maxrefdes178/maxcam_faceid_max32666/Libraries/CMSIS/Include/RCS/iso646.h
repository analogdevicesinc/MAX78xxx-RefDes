// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __iso646_h
#define __iso646_h


/*! \overview The header \<iso646.h\> defines macros that expand
  to the corresponding tokens to ease writing C programs with
  keyboards that do not have keys for frequently-used operators.
*/


#ifndef __cplusplus

/*! \brief Alternative spelling for logical and operator \ingroup Macros \synopsis

  \desc \b \this defines the alternative spelling for \c{&&}.
*/
#define and     &&


/*! \brief Alternative spelling for logical and-equals operator \ingroup Macros \synopsis

  \desc \b \this defines the alternative spelling for \c{&=}.
*/
#define and_eq  &=


/*! \brief Alternative spelling for bitwise and operator \ingroup Macros \synopsis

  \desc \b \this defines the alternative spelling for \c{&}.
*/
#define bitand  &


/*! \brief Alternative spelling for bitwise or operator \ingroup Macros \synopsis

  \desc \b \this defines the alternative spelling for \c{|}.
*/
#define bitor   |


/*! \brief Alternative spelling for bitwise complement operator \ingroup Macros \synopsis

  \desc \b \this defines the alternative spelling for \c{~}.
*/
#define compl   ~


/*! \brief Alternative spelling for logical not operator \ingroup Macros \synopsis

  \desc \b \this defines the alternative spelling for \c{!}.
*/
#define not     !


/*! \brief Alternative spelling for not-equal operator \ingroup Macros \synopsis

  \desc \b \this defines the alternative spelling for \c{!=}.
*/
#define not_eq  !=


/*! \brief Alternative spelling for logical or operator \ingroup Macros \synopsis

  \desc \b \this defines the alternative spelling for \c{||}.
*/
#define or      ||


/*! \brief Alternative spelling for bitwise or-equals operator \ingroup Macros \synopsis

  \desc \b \this defines the alternative spelling for \c{|=}.
*/
#define or_eq   |=


/*! \brief Alternative spelling for bitwise exclusive or operator \ingroup Macros \synopsis

  \desc \b \this defines the alternative spelling for \c{^}.
*/
#define xor     ^


/*! \brief Alternative spelling for bitwise exclusive-or-equals operator \ingroup Macros \synopsis

  \desc \b \this defines the alternative spelling for \c{^=}.
*/
#define xor_eq  ^=


#endif

#endif

