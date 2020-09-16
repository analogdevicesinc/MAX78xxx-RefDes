// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __assert_h
#define __assert_h

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief User defined behaviour for the assert macro \ingroup Functions \synopsis

 \desc There is no default implementation of \b \this. Keeping \b \this 
  out of the library means that you can can customize its behaviour without rebuilding 
  the library. You must implement this function where \a expression is the stringized
  expression, \a filename is the filename of the source file and \a line
  is the linenumber of the failed assertion.
*/
extern void __assert(const char *__expression, const char *__filename, int __line);

#ifdef __cplusplus
}
#endif

#else
#undef assert
#endif

#ifdef NDEBUG
#define assert(ignore) ((void)0)
#else
#define assert(e) ((e) ? (void)0 : __assert(#e, __FILE__, __LINE__))
#endif

#ifdef __CROSSWORKS_DOCUMENTATION

/*! \brief Allows you to place assertions and diagnostic tests into programs \ingroup Macros \synopsis

  \desc If \b NDEBUG is defined as a macro name at the point in the source file 
  where \b <assert.h> is included, the \b \this macro is defined 
  as: 
  
  \code #define assert(ignore) ((void)0) \endcode

  If \b NDEBUG is not defined as a macro name at the point in the source 
  file where \b <assert.h> is included, the \b \this macro expands 
  to a \b void expression that calls \b __assert. 
  
  \code #define assert(e) ((e) ? (void)0 : __assert(#e, __FILE__, __LINE__)) \endcode
  
  When such an \b \this 
  is executed and \b e is false, \b \this calls the \b __assert
  function with information about the particular call that failed: the text of 
  the argument, the name of the source file, and the source line number. These 
  are the stringized expression and the values of the preprocessing macros \b __FILE__
  and \b__LINE__.

  \note The \b \this macro is redefined according to the current state of \b NDEBUG
  each time that \b <assert.h> is included.
*/
#define assert(e) ...

#endif
