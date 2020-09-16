// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __stddef_H
#define __stddef_H

#include "__crossworks.h"

#ifndef __RAL_SIZE_T_DEFINED
#define __RAL_SIZE_T_DEFINED
/*! \brief size_t type \ingroup Types \synopsis

  \desc \b \this is the unsigned integral type returned by the sizeof operator.
*/
typedef __RAL_SIZE_T size_t;
#endif

/*! \brief ptrdiff_t type \ingroup Types \synopsis

  \desc \b \this is the signed integral type of the result of subtracting two pointers.
*/
typedef __RAL_PTRDIFF_T ptrdiff_t;


#ifndef NULL
/*! \brief NULL pointer \ingroup Macros \synopsis

  \desc \b \this is the null pointer constant.
*/
#define NULL 0
#endif


#if !defined(__RAL_WCHAR_T_DEFINED)
#define __RAL_WCHAR_T_DEFINED

typedef __RAL_WCHAR_T wchar_t;

#endif


#define offsetof(s,m) ((size_t)&(((s *)0)->m))

#ifdef __CROSSWORKS_DOCUMENTATION

/*! \brief offsetof \ingroup Macros \synopsis

  \desc \b \this returns the offset in bytes to the structure \a member, from the beginning of its structure \a type. 
*/
#define offsetof(type, member)

#endif

#endif
