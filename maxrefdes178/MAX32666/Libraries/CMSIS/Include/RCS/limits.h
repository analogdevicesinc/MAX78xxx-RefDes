// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __limits_H
#define __limits_H


/*! \brief Number of bits in a character \ingroup Type sizes \synopsis

  \desc \b \this is the number of bits for smallest object that is not a bit-field (byte).
*/
#define CHAR_BIT   8


/*! \brief Minimum value of a plain character \ingroup Character minimum and maximum values \synopsis

  \desc \b \this is the minimum value for an object of type \b char.
*/
#define CHAR_MIN   0


/*! \brief Maximum value of a plain character \ingroup Character minimum and maximum values \synopsis

  \desc \b \this is the maximum value for an object of type \b char.
*/
#define CHAR_MAX   255


/*! \brief Maximum value of a signed character \ingroup Character minimum and maximum values \synopsis

  \desc \b \this is the maximum value for an object of type \b{signed char}.
*/
#define SCHAR_MAX  127


/*! \brief Minimum value of a signed character \ingroup Character minimum and maximum values \synopsis

  \desc \b \this is the minimum value for an object of type \b{signed char}.
*/
#define SCHAR_MIN  (-128)


/*! \brief Maximum value of an unsigned char \ingroup Character minimum and maximum values \synopsis

  \desc \b \this is the maximum value for an object of type \b{unsigned char}.
*/
#define UCHAR_MAX  255


/*! \brief Minimum value of a short integer \ingroup Short integer minimum and maximum values \synopsis

  \desc \b \this is the minimum value for an object of type \b{short int}.
*/
#define SHRT_MIN   (-32767 - 1)


/*! \brief Maximum value of a short integer \ingroup Short integer minimum and maximum values \synopsis

  \desc \b \this is the minimum value for an object of type \b{short int}.
*/
#define SHRT_MAX   32767


/*! \brief Maximum value of an unsigned short integer \ingroup Short integer minimum and maximum values \synopsis

  \desc \b \this is the minimum value for an object of type \b{unsigned short int}.
*/
#define USHRT_MAX  65535


#if defined(__CROSSWORKS_MSP430) || defined(__CROSSWORKS_AVR) || defined(__CROSSWORKS_MAXQ)


/*! \brief Minimum value of an integer \ingroup Integer minimum and maximum values \synopsis

  \desc \b \this is the minimum value for an object of type \b int.
*/
#define INT_MIN   (-32767 - 1)


/*! \brief Maximum value of an integer \ingroup Integer minimum and maximum values \synopsis

  \desc \b \this is the maximum value for an object of type \b int.
*/
#define INT_MAX   32767


/*! \brief Maximum value of an unsigned integer \ingroup Integer minimum and maximum values \synopsis

  \desc \b \this is the maximum value for an object of type \b{unsigned int}.
*/
#define UINT_MAX  65535U


#else

/*! \brief Minimum value of an integer \ingroup Integer minimum and maximum values \synopsis

  \desc \b \this is the minimum value for an object of type \b int.
*/
#define INT_MIN	   (-2147483647 - 1)


/*! \brief Maximum value of an integer \ingroup Integer minimum and maximum values \synopsis

  \desc \b \this is the maximum value for an object of type \b int.
*/
#define INT_MAX    2147483647


/*! \brief Maximum value of an unsigned integer \ingroup Integer minimum and maximum values \synopsis

  \desc \b \this is the maximum value for an object of type \b{unsigned int}.
*/
#define UINT_MAX   4294967295U


#endif


/*! \brief Maximum value of a long integer \ingroup Long integer minimum and maximum values \synopsis

  \desc \b \this is the maximum value for an object of type \b{long int}.
*/
#define LONG_MAX   2147483647L


/*! \brief Minimum value of a long integer \ingroup Long integer minimum and maximum values \synopsis

  \desc \b \this is the minimum value for an object of type \b{long int}.
*/
#define LONG_MIN   (-2147483647L - 1)


/*! \brief Maximum value of an unsigned long integer \ingroup Long integer minimum and maximum values \synopsis

  \desc \b \this is the maximum value for an object of type \b{unsigned long int}.
*/
#define ULONG_MAX  4294967295UL


/*! \brief Minimum value of a long long integer \ingroup Long long integer minimum and maximum values \synopsis

  \desc \b \this is the minimum value for an object of type \b{long long int}.
*/
#define LLONG_MIN  (-9223372036854775807LL - 1)


/*! \brief Maximum value of a long long integer \ingroup Long long integer minimum and maximum values \synopsis

  \desc \b \this is the maximum value for an object of type \b{long long int}.
*/
#define LLONG_MAX  9223372036854775807LL


/*! \brief Maximum value of an unsigned long long integer \ingroup Long long integer minimum and maximum values \synopsis

  \desc \b \this is the maximum value for an object of type \b{unsigned long long int}.
*/
#define ULLONG_MAX 18446744073709551615ULL


/*! \brief maximum number of bytes in a multi-byte character \ingroup Multi-byte values \synopsis

  \desc \b \this is the maximum number of bytes in a multi-byte character
   for any supported locale. Unicode (ISO 10646) characters between 0 
   and 10FFFF inclusive are supported which convert to a maximum
   of four bytes in the UTF-8 encoding.
*/
#define MB_LEN_MAX  4


#endif
