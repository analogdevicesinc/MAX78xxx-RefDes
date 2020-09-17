// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef _SYS_TIME_H
#define _SYS_TIME_H

#ifdef __cplusplus
extern "C" {
#endif

// POSIX 1003.1-2001
struct timeval
{
  long tv_sec;  // Seconds since the Epoch
  long tv_usec;   // Microseconds
};

// POSIX 1003.1-2001
int gettimeofday(struct timeval *__tp, void *__tzp);

// Like Linux...
int settimeofday(const struct timeval *__tp, const void *__tzp);

#ifdef __cplusplus
}
#endif

#endif
