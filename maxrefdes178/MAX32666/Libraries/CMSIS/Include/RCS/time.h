// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __time_H
#define __time_H

#include "__crossworks.h"
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __RAL_SIZE_T_DEFINED
#define __RAL_SIZE_T_DEFINED
typedef __RAL_SIZE_T size_t;
#endif

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000
#endif

#ifndef NULL
#define NULL 0
#endif

/*! \brief Clock type \ingroup Types \synopsis

  \desc \b \this is the type returned by the \b clock function.
*/
typedef long clock_t;

/*! \brief Time type \ingroup Types \synopsis

  \desc \b \this is a long type that represents the time in number of seconds
  since UTC 1 January 1970, negative values indicate time before UTC 1 January 1970.
*/
typedef long time_t;

/*! \brief Time structure \ingroup Types \synopsis

  \desc \b \this structure has the following fields.

  \table{1.5in,*}
  || \b Member | \b Description
  || tm_sec    | seconds after the minute - [0,59]
  || tm_min    | minutes after the hour - [0,59]
  || tm_hour   | hours since midnight - [0,23]
  || tm_mday   | day of the month - [1,31]
  || tm_mon    | months since January - [0,11]
  || tm_year   | years since 1900
  || tm_wday   | days since Sunday - [0,6]
  || tm_yday   | days since January 1 - [0,365]
  || tm_isdst  | daylight savings time flag
  \endtable
*/
struct tm
{
  int tm_sec;     /* seconds after the minute - [0,59] */
  int tm_min;     /* minutes after the hour - [0,59] */
  int tm_hour;    /* hours since midnight - [0,23] */
  int tm_mday;    /* day of the month - [1,31] */
  int tm_mon;     /* months since January - [0,11] */
  int tm_year;    /* years since 1900 */
  int tm_wday;    /* days since Sunday - [0,6] */
  int tm_yday;    /* days since January 1 - [0,365] */
  int tm_isdst;   /* daylight savings time flag */
};

clock_t clock(void);
time_t time(time_t *__tp);
//lint -esym(534, time)


/*! \brief Calculates the difference between two times \ingroup Functions \synopsis

  \desc \b \this returns \a time1 - \a time0 as a double precision number.
*/
double difftime(time_t __time2, time_t __time1);


/*! \brief Convert a struct tm to time_t \ingroup Functions \synopsis

  \desc \b \this validates (and updates) the \a *tp struct to ensure that the
  \b tm_sec, \b tm_min, \b tm_hour, \b tm_mon fields are within the
  supported integer ranges and the \b tm_mday, \b tm_mon and \b tm_year fields
  are consistent. The validated \a *tp struct is converted to the number of seconds
  since UTC 1 January 1970 and returned.
*/
time_t mktime(struct tm *__tp);


/*! \brief Convert a struct tm to a string \ingroup Functions \synopsis

  \desc \b \this converts the \a *tp struct to a null terminated string of the
  form \tt{Sun Sep 16 01:03:52 1973}. The returned string is held in a static buffer.
  \b \this is not re-entrant.
*/
char *asctime(const struct tm *__tp);


/*! \brief Convert a struct tm to a string \ingroup Functions \synopsis

  \desc \b \this converts the \a *tp struct to a null terminated string of the
  form Sun Sep 16 01:03:52 1973 in \a buf and returns \a buf. The \a buf must point to
  an array at least 26 bytes in length.
*/
char *asctime_r(const struct tm *__tp, char *__buf);
//lint -esym(534, asctime_r)


/*! \brief Convert a time_t to a string \ingroup Functions \synopsis

  \desc \b \this converts the \a *tp to a null terminated string.
  The returned string is held in a static buffer, this function is not re-entrant.
*/
char *ctime(const time_t *__tp);


/*! \brief Convert a time_t to a string \ingroup Functions \synopsis

  \desc \b \this converts the \a *tp to a null terminated string in \a buf and
  returns \a buf. The \a buf must point to an array at least 26 bytes in length.
*/
char *ctime_r(const time_t *__tp, char *__buf);
//lint -esym(534, ctime_r)


/*! \brief Convert a time_t to a struct tm \ingroup Functions \synopsis

  \desc \b \this converts the \a *tp time format to a \b struct tm time format.
  The returned value points to a static object - this function is not re-entrant.
*/
struct tm *gmtime(const time_t *__tp);


/*! \brief Convert a time_t to a struct tm \ingroup Functions \synopsis

  \desc \b \this converts the \a *tp time format to a \b struct tm time format in
  \a *result and returns \a result.
*/
struct tm *gmtime_r(const time_t *__tp, struct tm *__result);
//lint -esym(534, gmtime_r)


/*! \brief Convert a time_t to a struct tm \ingroup Functions \synopsis

  \desc \b \this converts the \a *tp time format to a \b struct tm local time format.
  The returned value points to a static object - this function is not re-entrant.
*/
struct tm *localtime(const time_t *__tp);


/*! \brief Convert a time_t to a struct tm \ingroup Functions \synopsis

  \desc \b \this converts the \a *tp time format to a \b struct tm local time format in
  \a *result and returns \a result.
*/
struct tm *localtime_r(const time_t *__tp, struct tm *__result);
//lint -esym(534, localtime_r)


/*! \brief Format a struct tm to a string \ingroup Functions \synopsis

  \desc \b \this  formats the \a *tp struct to a null terminated string of maximum size \a smax-1 into the
  array at \a *s based on the \a fmt format string. The format string consists of conversion specifications
  and ordinary characters. Conversion specifications start with a \b % character followed by an optional \b # character.
  The following conversion specifications are supported:

  \table{1.5in,*}
  || Specification | Description
  || %s    | Abbreviated weekday name
  || %A    | Full weekday name
  || %b    | Abbreviated month name
  || %B    | Full month name
  || %c    | Date and time representation appropriate for locale
  || %#c   | Date and time formatted as "%A, %B %#d, %Y, %H:%M:%S" (Microsoft extension)
  || %C    | Century number
  || %d    | Day of month as a decimal number [01,31]
  || %#d   | Day of month without leading zero [1,31]
  || %D    | Date in the form %m/%d/%y (POSIX.1-2008 extension)
  || %e    | Day of month [ 1,31], single digit preceded by space
  || %F    | Date in the format %Y-%m-%d
  || %h    | Abbreviated month name as %b
  || %H    | Hour in 24-hour format [00,23]
  || %#H   | Hour in 24-hour format without leading zeros [0,23]
  || %I    | Hour in 12-hour format [01,12]
  || %#I   | Hour in 12-hour format without leading zeros [1,12]
  || %j    | Day of year as a decimal number [001,366]
  || %#j   | Day of year as a decimal number without leading zeros [1,366]
  || %k    | Hour in 24-hour clock format [ 0,23] (POSIX.1-2008 extension)
  || %l    | Hour in 12-hour clock format [ 0,12] (POSIX.1-2008 extension)
  || %m    | Month as a decimal number [01,12]
  || %#m   | Month as a decimal number without leading zeros [1,12]
  || %M    | Minute as a decimal number [00,59]
  || %#M   | Minute as a decimal number without leading zeros [0,59]
  || %n    | Insert newline character (POSIX.1-2008 extension)
  || %p    | Locale's a.m or p.m indicator for 12-hour clock
  || %r    | Time as %I:%M:%s %p (POSIX.1-2008 extension)
  || %R    | Time as %H:%M (POSIX.1-2008 extension)
  || %S    | Second as a decimal number [00,59]
  || %t    | Insert tab character (POSIX.1-2008 extension)
  || %T    | Time as %H:%M:%S
  || %#S   | Second as a decimal number without leading zeros [0,59]
  || %U    | Week of year as a decimal number [00,53], Sunday is first day of the week
  || %#U   | Week of year as a decimal number without leading zeros [0,53], Sunday is first day of the week
  || %w    | Weekday as a decimal number [0,6], Sunday is 0
  || %W    | Week number as a decimal number [00,53], Monday is first day of the week
  || %#W   | Week number as a decimal number without leading zeros [0,53], Monday is first day of the week
  || %x    | Locale's date representation
  || %#x   | Locale's long date representation
  || %X    | Locale's time representation
  || %y    | Year without century, as a decimal number [00,99]
  || %#y   | Year without century, as a decimal number without leading zeros [0,99]
  || %Y    | Year with century, as decimal number
  || %z,%Z | Timezone name or abbreviation
  || %%    | %
  \endtable
*/
size_t strftime(char *__s, size_t __smax, const char *__fmt, const struct tm *__tp);
//lint -esym(534, strftime)

size_t strftime_l(char *__s, size_t __smax, const char *__fmt, const struct tm *__tp, __locale_t __loc);
//lint -esym(534, strftime_l)

#ifdef __cplusplus
}
#endif

#endif
