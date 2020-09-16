// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __stdio_h
#define __stdio_h

// Can't just #include <stddef.h> to get a tranche of common
// definitions, nor #include <stdarg.h> to get va_list, so #include
// "__crossworks.h" to get the private versions of all.
#include "__crossworks.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef EOF
#define EOF (-1)
#endif

#ifndef __RAL_SIZE_T_DEFINED
#define __RAL_SIZE_T_DEFINED
typedef __RAL_SIZE_T size_t;
#endif



/*! \brief Write a character to standard output \ingroup Character and string I/O functions \synopsis

  \desc \b \this writes the character \a c to the standard output stream. 

  \b \this returns the character written. If a write error occurs, \b putchar
  returns \a EOF.
*/
int putchar(int __c);


/*! \brief Read a character from standard input \ingroup Character and string I/O functions \synopsis

  \desc \b \this reads a single character from the standard input stream.

  If the stream is at end-of-file or a read error occurs, \b \this returns \b EOF.
*/
int getchar(void);


/*! \brief Write a string to standard output \ingroup Character and string I/O functions \synopsis

  \desc \b \this writes the string pointed to by \a s to the standard output 
  stream using \b putchar and appends a new-line character to the output. 
  The terminating null character is not written.

  \b \this returns \b EOF if a write error occurs; otherwise it returns 
  a nonnegative value.
*/
int puts(const char *__s);


/*! \brief Read a string from standard input \ingroup Character and string I/O functions \synopsis

  \desc \b \this reads characters from standard input into the
  array pointed to by \a s until end-of-file is encountered or
  a new-line character is read. Any new-line character is discarded,
  and a null character is written immediately after the
  last character read into the array.

  \b \this returns \a s if successful. If end-of-file
  is encountered and no characters have been read into the array, the
  contents of the array remain unchanged and \b \this returns a 
  null pointer. If a read error occurs during the operation, the
  array contents are indeterminate and \b \this returns a null pointer.
*/
char *gets(char *__s);


/*! \brief Write formatted text to a string \ingroup Formatted output functions \synopsis

  \desc \b \this writes to the string pointed to by \a s under control
  of the string pointed to by \a format that specifies how subsequent arguments
  are converted for output. A null character is written at the end of the characters
  written; it is not counted as part of the returned value.

  If there are insufficient arguments for the format, the behavior is undefined. 
  If the format is exhausted while arguments remain, the excess arguments are 
  evaluated but are otherwise ignored.

  If copying takes place between objects that overlap, the behavior is undefined.

  \b \this returns number of characters transmitted (not counting the terminating 
  null), or a negative value if an output or encoding error occurred.
*/
int sprintf(char *__s, const char *__format, ...);


/*! \brief Write formatted text to a string with truncation \ingroup Formatted output functions \synopsis

  \desc \b \this writes to the string pointed to by \a s under control 
  of the string pointed to by \a format that specifies how subsequent arguments 
  are converted for output.

  If \a n is zero, nothing is written, and \a s can 
  be a null pointer. Otherwise, output characters beyond the \a n\minus~1\sup{st}
  are discarded rather than being written to the array, and a null character is 
  written at the end of the characters actually written into the array. A null 
  character is written at the end of the conversion; it is not counted as part 
  of the returned value.

  If there are insufficient arguments for the format, the behavior is undefined. 
  If the format is exhausted while arguments remain, the excess arguments are 
  evaluated but are otherwise ignored.

  If copying takes place between objects that overlap, the behavior is undefined.

  \b \this returns the number of characters that would have been written 
  had \a n been sufficiently large, not counting the terminating 
  null character, or a negative value if an encoding error occurred. Thus, the 
  null-terminated output has been completely written if and only if the returned 
  value is nonnegative and less than \a n>.
*/
int snprintf(char *__s, size_t __n, const char *__format, ...);


/*! \brief Write formatted text to a string with truncation using variable argument context
  \ingroup Formatted output functions \synopsis

  \desc \b \this writes to the string pointed to by \a s under control
  of the string pointed to by \a format that specifies how subsequent arguments
  are converted for output. Before calling \b \this, \a arg must
  be initialized by the \b va_start macro (and possibly subsequent \b va_arg calls).
  \b \this does not invoke the \b va_end macro.

  If \a n is zero, nothing is written, and \a s can 
  be a null pointer. Otherwise, output characters beyond the \a n\minus~1\sup{st}
  are discarded rather than being written to the array, and a null character is 
  written at the end of the characters actually written into the array. A null 
  character is written at the end of the conversion; it is not counted as part 
  of the returned value.

  If there are insufficient arguments for the format, the behavior is undefined. 
  If the format is exhausted while arguments remain, the excess arguments are 
  evaluated but are otherwise ignored.

  If copying takes place between objects that overlap, the behavior is undefined.

  \b \this returns the number of characters that would have been written
  had \a n been sufficiently large, not counting the terminating
  null character, or a negative value if an encoding error occurred. Thus,
  the null-terminated output has been completely written if and only if the returned
  value is nonnegative and less than \a n.

  \note

  \b \this is equivalent to \b snprintf with the
  variable argument list replaced by \a arg.
*/
int vsnprintf(char *__s, size_t __n, const char *__format, __va_list __arg);


/*! \brief Write formatted text to standard output \ingroup Formatted output functions \synopsis

  \desc \b \this writes to the standard output stream using \b putchar, under 
  control of the string pointed to by \a format that specifies how subsequent 
  arguments are converted for output.

  If there are insufficient arguments for the format, the behavior is undefined. 
  If the format is exhausted while arguments remain, the excess arguments are 
  evaluated but are otherwise ignored.

  \b \this returns the number of characters transmitted, or a negative value 
  if an output or encoding error occurred.

  \task Formatted output control strings

  The format is composed of zero or more directives: ordinary characters (not 
  \q{%}, which are copied unchanged to the output 
  stream; and conversion specifications, each of which results in fetching zero 
  or more subsequent arguments, converting them, if applicable, according to the 
  corresponding conversion specifier, and then writing the result to the output 
  stream.

  Each conversion specification is introduced by the character \q{%}. 
  After the \q{%} the following appear in sequence:

  \item Zero or more \em flags (in any order) that modify the meaning of the 
    conversion specification.
  \item An optional \em{minimum field width}. If the converted value has fewer 
    characters than the field width, it is padded with spaces (by default) on 
    the left (or right, if the left adjustment flag has been given) to the field 
    width. The field width takes the form of an asterisk \q{*} or a decimal
    integer.
  \item An optional precision that gives the minimum number of digits to appear 
    for the \q{d}, \q{i}, \q{o}, \q{u}, \q{x}, and \q{X} conversions, the number
    of digits to appear after the decimal-point character for \q{e},
    \q{E}, \q{f}, and \q{F} conversions, the maximum number of significant
    digits for the \q{g} and \q{G} conversions, or the maximum number 
    of bytes to be written for \q{s} conversions. The precision takes 
    the form of a period \q{.} followed either by an asterisk \q{*}
    or by an optional decimal integer; if only the period is specified,
    the precision is taken as zero. If a precision appears with any other
    conversion specifier, the behavior is undefined.
  \item An optional length modifier that specifies the size of the argument.
  \item A conversion specifier character that specifies the type of conversion 
    to be applied.

  As noted above, a field width, or precision, or both, may be indicated by 
  an asterisk. In this case, an int argument supplies the field width or precision. 
  The arguments specifying field width, or precision, or both, must appear (in 
  that order) before the argument (if any) to be converted. A negative field width 
  argument is taken as a \q{-} flag followed by a positive field width. A negative
  precision argument is taken as if the precision were omitted.

  Some library variants do not support width and precision specifiers 
  in order to reduce code and data space requirements; please ensure that you 
  have selected the correct library in the \b{Printf Width/Precision Support}
  property of the project if you use these.

  \task Flag characters

  The flag characters and their meanings are:

  \term \q{-}
  \item The result of the conversion is left-justified within the field. The default, 
    if this flag is not specified, is that the result of the conversion is left-justified 
    within the field.
  \term \q{+}
  \item The result of a signed conversion \em always begins with a plus or minus 
    sign. The default, if this flag is not specified, is that it begins with a 
    sign only when a negative value is converted.
  \term \em space
  \item If the first character of a signed conversion is not a sign, or if a signed 
    conversion results in no characters, a space is prefixed to the result. If 
    the space and \q{+} flags both appear, the space 
    flag is ignored.
  \term \q{#}
  \item The result is converted to an \em{alternative form}. For \q{o} 
    conversion, it increases the precision, if and only if necessary,
    to force the first digit of the result to be a zero (if the value
    and precision are both zero, a single \q{0} is printed). For \q{x}
    or \q{X} conversion, a nonzero result has \q{0x} or \q{0X}
    prefixed to it. For \q{e}, \q{E}, \q{f}, \q{F}, \q{g}, and \q{G}
    conversions, the result of converting a floating-point number always
    contains a decimal-point character, even if no digits follow it.
    (Normally, a decimal-point character appears in the result 
    of these conversions only if a digit follows it.) For \q{g} 
    and \q{F} conversions, trailing zeros are not removed from the
    result. As an extension, when used in \q{p} conversion, the results
    has \q{#} prefixed to it. For other conversions, the behavior
    is undefined. 
  \term \q{0}
  \item For \q{d}, \q{i}, \q{o}, \q{u}, \q{x}, \q{X}, \q{e}, \q{E}, \q{f},
    \q{F}, \q{g}, and \q{G} conversions, leading zeros (following any
    indication of sign or base) are used to pad to the field width
    rather than performing space padding, except when converting an
    infinity or NaN. If the \q{0} and \q{-} flags both appear, the \q{0} 
    flag is ignored. For \q{d}, \q{i}, \q{o}, \q{u}, \q{x}, and \q{X} 
    conversions, if a precision is specified, the \q{0}  flag is
    ignored. For other conversions, the behavior is undefined.

  \task Length modifiers

  The length modifiers and their meanings are:

  \term \q{hh}
  \item Specifies that a following \q{d}, \q{i}, 
    \q{o}, \q{u}, 
    \q{x}, or \q{X} 
    conversion specifier applies to a \b{signed char} or \b{unsigned char}
    argument (the argument will have been promoted according to the integer promotions, 
    but its value will be converted to \b{signed char} or \b{unsigned char}
    before printing); or that a following \q{n} conversion specifier applies 
    to a pointer to a \b{signed char} argument.
  \term \q{h}
  \item Specifies that a following \q{d}, \q{i}, 
    \q{o}, \q{u}, 
    \q{x}, or \q{X} 
    conversion specifier applies to a \b{short int} or \b{unsigned short int}
    argument (the argument will have been promoted according to the integer
    promotions, but its value is converted to \b{short int} or \b{unsigned short int}
    before printing); or that a following \q{n} conversion specifier applies 
    to a pointer to a \b{short int} argument.
  \term \q{l}
  \item Specifies that a following \q{d}, \q{i}, \q{o}, \q{u}, \q{x}, or \q{X} 
    conversion specifier applies to a \b{long int} or \b{unsigned long int} 
    argument; that a following \q{n} conversion specifier applies to a pointer
    to a \b{long int} argument; or has no effect on a following \q{e}, \q{E}, 
    \q{f}, \q{F}, \q{g}, or \q{G} conversion specifier. Some
    library variants do not support the \q{l} length modifier in order to
    reduce code and data space requirements; please ensure that you have
    selected the correct library in the \b{Printf Integer Support} property
    of the project if you use this length modifier.
  \term \q{ll}
  \item Specifies that a following \q{d}, \q{i}, \q{o}, \q{u},  \q{x}, or \q{X} 
    conversion specifier applies to a \b{long long int} or \b{unsigned long long int}
    argument; that a following \q{n} conversion specifier applies to a
    pointer to a \b{long long int} argument.  Some library
    variants do not support the \q{ll} length modifier in order to reduce
    code and data space requirements; please ensure that you have selected
    the correct library in the \b{Printf Integer Support} property of the
    project if you use this length modifier.

  If a length modifier appears with any conversion specifier other than as specified 
  above, the behavior is undefined. Note that the C99 length modifiers \q{j}, 
  \q{z}, \q{t}, and \q{L} are not supported.

  \task Conversion specifiers

  The conversion specifiers and their meanings are:

  \term \q{d}, \q{i}
  \item The argument is converted to signed decimal in the style [-]\em{dddd}.
    The precision specifies the minimum number of digits to appear; if the value
    being converted can be represented in fewer digits, it is expanded with leading 
    spaces. The default precision is one. The result of converting a zero
    value with a precision of zero is no characters.
  \term \q{o}, \q{u}, \q{x}, \q{X}
  \item The unsigned argument is converted to unsigned octal for \q{o}, 
    unsigned decimal for \q{u}, or unsigned hexadecimal notation for
    \q{x} or \q{X} in the style \em dddd the letters \q{abcdef} are used
    for \q{x} conversion and the letters \q{ABCDEF} for \q{X} conversion.
    The precision specifies the minimum number of digits to appear; if the
    value being converted can be represented in fewer digits, it is expanded 
    with leading spaces. The default precision is one. The result of converting 
    a zero value with a precision of zero is no characters.
  \term \q{f}, \q{F}
  \item A double argument representing a floating-point number is converted to decimal 
    notation in the style [-]\em{ddd.ddd}, where the number of digits after
    the decimal-point character is equal to the precision specification. If the 
    precision is missing, it is taken as 6; if the precision is zero and the \q{#} 
    flag is not specified, no decimal-point character appears. If a decimal-point 
    character appears, at least one digit appears before it. The value is rounded 
    to the appropriate number of digits. A double argument representing an infinity 
    is converted to \q{inf}. A double argument representing a NaN is converted to
    \q{nan}. The \q{F} conversion specifier produces \q{INF} or \q{NAN} instead of
    \q{inf} or \q{nan}, respectively. Some library variants do not
    support the \q{f} and \q{F} conversion specifiers in order to reduce code and
    data space requirements; please ensure that you have selected the correct library
    in the \b{Printf Floating Point Support} property of the project if you use
    these conversion specifiers.
  \term \q{e}, \q{E}
  \item A double argument representing a floating-point number is converted in the 
    style [-]\em{d.ddd}e\plusminus~\em{dd}, where there is one digit (which is
    nonzero if the argument is nonzero) before the decimal-point character and the
    number of digits after it is equal to the precision; if the precision is
    missing, it is taken as 6; if the precision is zero and the \q{#} flag is not
    specified, no decimal-point character appears. The value is rounded to the
    appropriate number of digits. The \q{E} conversion specifier produces a number
    with \q{E} instead of \q{e} introducing the exponent. The exponent always
    contains at least two digits, and only as many more digits as necessary to
    represent the exponent. If the value is zero, the exponent is zero. A double
    argument representing an infinity is converted to \q{inf}. A double argument
    representing a NaN is converted to \q{nan}. The \q{E} conversion specifier
    produces \q{INF} or \q{NAN} instead of \q{inf} or \q{nan}, respectively.
    Some library variants do not support the \q{f} and \q{F} 
    conversion specifiers in order to reduce code and data space requirements; 
    please ensure that you have selected the correct library in the \b{Printf 
    Floating Point Support} property of the project if you use these conversion 
    specifiers.
  \term \q{g}, \q{G}
  \item A double argument representing a floating-point number is converted in
    style \q{f} or \q{e} (or in style \q{F} or \q{e} in the case of a \q{G}
    conversion specifier), with the precision specifying the number of
    significant digits. If the precision is zero, it is taken as one. The style
    used depends on the value converted; style \q{e} (or \q{E}) is used only if
    the exponent resulting from such a conversion is less than -4 or greater
    than or equal to the precision. Trailing zeros are removed from the
    fractional portion of the result unless the \q{#} flag is specified; a
    decimal-point character appears only if it is followed by a digit. A double
    argument representing an infinity is converted to \q{inf}. A double
    argument representing a NaN is converted to \q{nan}. The \q{G} conversion
    specifier produces \q{INF} or \q{NAN} instead of \q{inf} or \q{nan},
    respectively. Some library variants do not support the \q{f}
    and \q{F} conversion specifiers in order to reduce code and data space
    requirements; please ensure that you have selected the correct library in
    the \b{Printf Floating Point Support} property of the project if you use
    these conversion specifiers.
  \term \q{c}
  \item The argument is converted to an \b{unsigned char}, and the resulting 
    character is written.
  \term \q{s}
  \item The argument is be a pointer to the initial element of an array of character 
    type. Characters from the array are written up to (but not including) the 
    terminating null character. If the precision is specified, no more than that 
    many characters are written. If the precision is not specified or is greater 
    than the size of the array, the array must contain a null character.
  \term \q{p}
  \item The argument is a pointer to \b{void}. The value of the pointer is converted 
    in the same format as the \q{x} conversion specifier 
    with a fixed precision of 2*\b sizeof(\b void *).
  \term \q{n}
  \item The argument is a pointer to a signed integer into which is \em written
    the number of characters written to the output stream so far by the call to 
    the formatting function. No argument is converted, but one is consumed. If 
    the conversion specification includes any flags, a field width, or a precision, 
    the behavior is undefined.
  \term \q{%}
  \item A \q{%} character is written. No argument is converted.

  Note that the C99 width modifier \q{l} used in conjunction with the \q{c} and \q{s} 
  conversion specifiers is not supported and nor are the conversion specifiers 
  \q{a} and \q{A}.
*/
int printf(const char *__format, ...);


/*! \brief Write formatted text to standard output using variable argument context
  \ingroup Formatted output functions \synopsis

  \desc \b \this writes to the standard output stream using \b putchar
  under control of the string pointed to by \a format that specifies how
  subsequent arguments are converted for output. Before calling \b \this,
  \a arg must be initialized by the \b va_start macro (and possibly subsequent
  \b va_arg calls).  \b \this does not invoke the \a va_end macro.

  \b \this returns the number of characters transmitted, or a negative value
  if an output or encoding error occurred.

  \note
  
  \b \this is equivalent to \b printf with the variable argument list
  replaced by \a arg.
*/
int vprintf(const char *__format, __va_list __arg);


/*! \brief Write formatted text to a string using variable argument context
  \ingroup Formatted output functions \synopsis

  \desc \b \this writes to the string pointed to by \a s under control
  of the string pointed to by \a format that specifies how subsequent arguments
  are converted for output. Before calling \b \this, \a arg must
  be initialized by the \b va_start macro (and possibly subsequent \a va_arg calls).
  \b \this does not invoke the \b va_end macro.

  A null character is written at the end of the characters written; it is not
  counted as part of the returned value.

  If there are insufficient arguments for the format, the behavior is undefined. 
  If the format is exhausted while arguments remain, the excess arguments are 
  evaluated but are otherwise ignored.

  If copying takes place between objects that overlap, the behavior is undefined.

  \b \this returns number of characters transmitted (not counting the
  terminating  null), or a negative value if an output or encoding error occurred.

  \note 

  \b \this is equivalent to \b sprintf with the variable argument list
  replaced by \a arg.
*/
int vsprintf(char *__s, const char *__format, __va_list __arg);


/*! \brief Read formatted text from standard input \ingroup Formatted input functions \synopsis

  \desc \b \this reads input from the standard input stream under control 
  of the string pointed to by \a format that specifies the admissible 
  input sequences and how they are to be converted for assignment, using
  subsequent arguments as pointers to the objects to receive the converted
  input.

  If there are insufficient arguments for the format, the behavior is undefined. 
  If the format is exhausted while arguments remain, the excess arguments are 
  evaluated but are otherwise ignored.

  \b \this returns the value of the macro \a EOF if an input failure occurs
  before any conversion. Otherwise, \b \this returns the number of input
  items assigned, which can be fewer than provided for, or even zero, in the
  event of an early matching failure.

  \task Formatted input control strings

  The format is composed of zero or more directives: one or more white-space 
  characters, an ordinary character (neither \b{%} nor a white-space
  character), or a conversion specification.

  Each conversion specification is introduced by the character \b{%}. 
  After the \b{%}, the following appear in sequence:

  \item An optional assignment-suppressing character \b{*}.
  \item An optional nonzero decimal integer that specifies the maximum field width 
    (in characters).
  \item An optional length modifier that specifies the size of the receiving object.
  \item A conversion specifier character that specifies the type of conversion 
    to be applied.

  The formatted input function executes each directive of the format in turn. 
  If a directive fails, the function returns. Failures are described as input 
  failures (because of the occurrence of an encoding error or the unavailability 
  of input characters), or matching failures (because of inappropriate input).

  A directive composed of white-space character(s) is executed by reading input 
  up to the first non-white-space character (which remains unread), or until no 
  more characters can be read.

  A directive that is an ordinary character is executed by reading the next characters 
  of the stream. If any of those characters differ from the ones composing the 
  directive, the directive fails and the differing and subsequent characters remain 
  unread. Similarly, if end-of-file, an encoding error, or a read error prevents 
  a character from being read, the directive fails.

  A directive that is a conversion specification defines a set of matching input 
  sequences, as described below for each specifier. A conversion specification 
  is executed in the following steps:

 \item Input white-space characters (as specified by the \b{isspace} function) 
    are skipped, unless the specification includes a \b{[}, \b{c}, or \b{n} 
    specifier.
 \item An input item is read from the stream, unless the specification includes 
    an n specifier. An input item is defined as the longest sequence of input 
    characters which does not exceed any specified field width and which is, or 
    is a prefix of, a matching input sequence. The first character, if any, after 
    the input item remains unread. If the length of the input item is zero, the 
    execution of the directive fails; this condition is a matching failure unless
    end-of-file, an encoding error, or a read error prevented input from the stream, 
    in which case it is an input failure.
  \item Except in the case of a \b{%} specifier, the 
    input item (or, in the case of a %n directive, the count of input characters) 
    is converted to a type appropriate to the conversion specifier. If the input 
    item is not a matching sequence, the execution of the directive fails: this 
    condition is a matching failure. Unless assignment suppression was indicated 
    by a \b{*}, the result of the conversion is 
    placed in the object pointed to by the first argument following the format 
    argument that has not already received a conversion result. If this object 
    does not have an appropriate type, or if the result of the conversion cannot 
    be represented in the object, the behavior is undefined.

  \task Length modifiers

  The length modifiers and their meanings are:

  \term \q{hh}
  \item Specifies that a following \q{d}, \q{i}, \q{o}, \q{u}, \q{x}, \q{X}, 
    or \q{n} conversion specifier applies to an argument with type pointer
    to \b{signed char} or pointer to \b{unsigned char}.
  \term \q{h}
  \item Specifies that a following \q{d}, \q{i}, \q{o}, \q{u}, \q{x}, \q{X}, 
    or \q{n} conversion specifier applies to an  argument with type pointer
    to \b{short int} or \b{unsigned short int}.
  \term \q{l}
  \item Specifies that a following \q{d}, \q{i}, \q{o}, \q{u}, \q{x}, \q{X},
    or \q{n} conversion specifier applies to an argument with type pointer
    to \b{long int} or \b{unsigned long int}; that a following \q{e}, \q{E}, 
    \q{f}, \q{F}, \q{g}, or \q{G} conversion specifier applies to an argument
    with type pointer to \b{double}. Some library variants do not
    support the \q{l} length modifier in order to reduce code and data space
    requirements; please ensure that you have selected the correct library in
    the \b{Printf Integer Support} property of the project if you use this
    length modifier. 
  \term \q{ll}
  \item Specifies that a following \q{d}, \q{i}, \q{o}, \q{u}, \q{x}, \q{X}, 
    or \q{n} conversion specifier applies to an argument with type pointer to
    \b{long long int} or \b{unsigned long long int}.  Some library
    variants do not support the \q{ll} length modifier in order to reduce code
    and data space requirements; please  ensure that you have selected the correct
    library in the \b{Printf Integer Support} property of the project if you use
    this length modifier.

  If a length modifier appears with any conversion specifier other than as specified 
  above, the behavior is undefined. Note that the C99 length modifiers \q{j}, 
  \q{z}, \q{t}, and  \q{L} are not supported.

  \task Conversion specifiers

  \term \q{d}
  \item Matches an optionally signed decimal integer, whose format is the same as 
    expected for the subject sequence of the \b{strtol} function with the value 
    10 for the \b{base} argument. The corresponding argument must be a pointer 
    to signed integer.
  \term \q{i}
  \item Matches an optionally signed integer, whose format is the same as expected 
    for the subject sequence of the \b{strtol} function with the value zero 
    for the \b{base} argument. The corresponding argument must be a pointer 
    to signed integer.
  \term \q{o}
  \item Matches an optionally signed octal integer, whose format is the same as 
    expected for the subject sequence of the \b{strtol} function with the value 
    18 for the \b{base} argument. The corresponding argument must be a pointer 
    to signed integer.
  \term \q{u}
  \item Matches an optionally signed decimal integer, whose format is the same as 
    expected for the subject sequence of the \b{strtoul} function with the 
    value 10 for the \b{base} argument. The corresponding argument must be 
    a pointer to unsigned integer.
  \term \q{x}
  \item Matches an optionally signed hexadecimal integer, whose format is the same 
    as expected for the subject sequence of the \b{strtoul} function with the 
    value 16 for the \b{base} argument. The corresponding argument must be 
    a pointer to unsigned integer.
  \term \q{e}, \q{f}, \q{g}
  \item Matches an optionally signed floating-point number whose format is the same 
    as expected for the subject sequence of the \b{strtod} function. The corresponding 
    argument shall be a pointer to floating. Some library variants 
    do not support the \q{e}, \q{f} and \q{F} conversion specifiers in order to 
    reduce code and data space requirements; please ensure that you have selected 
    the correct library in the \b{Scanf Floating Point Support} property of 
    the project if you use these conversion specifiers.
  \term \q{c}
  \item Matches a sequence of characters of exactly the number specified by the 
    field width (one if no field width is present in the directive). The corresponding 
    argument must be a pointer to the initial element of a character array large 
    enough to accept the sequence. No null character is added. 
  \term \q{s}
  \item Matches a sequence of non-white-space characters The corresponding argument 
    must be a pointer to the initial element of a character array large enough 
    to accept the sequence and a terminating null character, which will be added 
    automatically.
  \term \q{[}
  \item Matches a nonempty sequence of characters from a set of expected characters 
    (the \em scanset). The corresponding argument must be a pointer to the 
    initial element of a character array large enough to accept the sequence and 
    a terminating null character, which will be added automatically. The conversion 
    specifier includes all subsequent characters in the format string, up to and 
    including the matching right bracket \q{]}. The characters between the brackets
    (the \em scanlist) compose the scanset, unless the character after the left
    bracket is a circumflex \q{^}, in which case the scanset contains all characters
    that do not appear in the scanlist between the circumflex and the right bracket.
    If the conversion specifier begins with \q{[]} or\q{[^]}, 
    the right bracket character is in the scanlist and the next following right 
    bracket character is the matching right bracket that ends the specification; 
    otherwise the first following right bracket character is the one that ends 
    the specification. If a \q{-} character is in 
    the scanlist and is not the first, nor the second where the first character 
    is a \q{^}, nor the last character, it is treated 
    as a member of the scanset. Some library variants do not support 
    the \q{[} conversion specifier in order to reduce 
    code and data space requirements; please ensure that you have selected the 
    correct library in the \b{Scanf Classes Supported} property of the project 
    if you use this conversion specifier.
  \term \q{p}
  \item Reads a sequence output by the corresponding \q{%p} 
    formatted output conversion. The corresponding argument must be a pointer 
    to a pointer to \b{void}.
  \term \q{n}
  \item No input is consumed. The corresponding argument shall be a pointer to signed 
    integer into which is to be written the number of characters read from the 
    input stream so far by this call to the formatted input function. Execution 
    of a \q{%n} directive does not increment the 
    assignment count returned at the completion of execution of the fscanf function. 
    No argument is converted, but one is consumed. If the conversion specification 
    includes an assignment-suppressing character or a field width, the behavior 
    is undefined.
  \term \q{%}
  \item Matches a single \q{%} character; no conversion 
    or assignment occurs.

  Note that the C99 width modifier \q{l} used in conjunction with the
  \q{c}, \q{s}, and \q{[} conversion specifiers is not supported and nor
  are the conversion specifiers \q{a} and \q{A}.

*/
int scanf(const char *__format, ...);


/*! \brief Read formatted text from string \ingroup Formatted input functions \synopsis

  \desc \b \this reads input from the string \a s under 
  control of the string pointed to by \a format that specifies the 
  admissible input sequences and how they are to be converted for assignment, 
  using subsequent arguments as pointers to the objects to receive the converted 
  input.

  If there are insufficient arguments for the format, the behavior is undefined. 
  If the format is exhausted while arguments remain, the excess arguments are 
  evaluated but are otherwise ignored.

  \b \this returns the value of the macro \a EOF
  if an input failure occurs before any conversion. Otherwise, \b \this
  returns the number of input items assigned, which can be fewer than provided 
  for, or even zero, in the event of an early matching failure.
*/
int sscanf(const char *__s, const char *__format, ...);


/*! \brief Read formatted text from standard using variable argument context
  \ingroup Formatted input functions \synopsis

  \desc \b \this reads input from the standard input stream under
  control  of the string pointed to by \a format that specifies
  the admissible  input sequences and how they are to be converted for assignment,
  using subsequent arguments as pointers to the objects to receive the converted
  input. Before calling \b \this, \a arg must be initialized by the \a va_start
  macro (and possibly subsequent \a va_arg calls). \b \this does not invoke the
  \a va_end macro.

  If there are insufficient arguments for the format, the behavior is undefined.

  \b \this returns the value of the macro \b EOF if an input failure occurs before
  any conversion. Otherwise, \b \this returns the number of input items assigned,
  which can be fewer than provided for, or even zero, in the event of an early
  matching failure.

  \note

  \b \this is equivalent to \a scanf with the variable argument list replaced
  \a arg.
*/
int vscanf(const char *__format, __va_list __arg);


/*! \brief Read formatted text from a string using variable argument context
  \ingroup Formatted input functions \synopsis

  \desc \b \this reads input from the string \a s under
  control of the string pointed to by \a format that specifies
  the  admissible input sequences and how they are to be converted for assignment,
  using subsequent arguments as pointers to the objects to receive the converted
  input. Before calling \b \this, \a arg must be initialized by the \b va_start
  macro (and possibly subsequent \b va_arg calls). \b \this does not invoke the
  \b va_end macro.

  If there are insufficient arguments for the format, the behavior is undefined.

  \b \this returns the value of the macro \b EOF if an input failure occurs before
  any conversion. Otherwise, \b \this returns the number of input items
  assigned, which can be fewer than provided for, or even zero, in the event of
  an early matching failure.

  \note \b \this is equivalent to \b sscanf with the variable argument list replaced
  by \a arg.
*/
int vsscanf(const char *__s, const char *__format, __va_list __arg);


// Macro version of putchar.
#define putchar(x) __putchar(x)

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define FILENAME_MAX 256
#define FOPEN_MAX 8
#define TMP_MAX 256
#define L_tmpnam 256
#define BUFSIZ 256
#define	_IOFBF 0 // setvbuf fully buffered
#define	_IOLBF 1 // setvbuf line buffered
#define	_IONBF 2 // setvbuf unbuffered

#ifndef __RAL_FILE_DEFINED
#define __RAL_FILE_DEFINED
typedef struct __RAL_FILE FILE;
#endif

typedef long fpos_t;
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;
void clearerr(FILE *);
int fclose(FILE *);
int feof(FILE *);
int ferror(FILE *);
int fflush(FILE *);
int fgetc(FILE *);
int fgetpos(FILE *, fpos_t *);
char *fgets(char *, int, FILE *);
int fileno(FILE *);
FILE *fopen(const char *, const char *);
int fprintf(FILE *, const char *, ...);
int fputc(int, FILE *);
int fputs(const char *, FILE *);
size_t fread(void *, size_t, size_t, FILE *);
FILE *freopen(const char *, const char *, FILE *);
int fscanf(FILE *, const char *, ...);
int fseek(FILE *, long, int);
int fsetpos(FILE *, const fpos_t *); 
long ftell(FILE *);
size_t fwrite(const void *, size_t, size_t, FILE *);
int getc(FILE *);
void perror(const char *);
int putc(int, FILE *);
int remove(const char *);
int rename(const char *, const char *);
void rewind(FILE *);
void setbuf(FILE *, char *);
int setvbuf(FILE *, char *, int, size_t);
FILE *tmpfile(void);
char *tmpnam(char *);
int ungetc(int, FILE *);
int vfprintf(FILE *, const char *, __va_list);
int vfscanf(FILE *, const char *, __va_list);

#ifdef __cplusplus
}
#endif

#endif
