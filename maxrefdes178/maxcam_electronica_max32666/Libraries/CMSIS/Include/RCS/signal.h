// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef __signal_h
#define __signal_h

#ifdef __cplusplus
extern "C" {
#endif

#define SIGABRT 0
#define SIGFPE 1
#define SIGILL 2
#define SIGINT 3
#define SIGSEGV 4
#define SIGTERM 5

#define SIG_DFL (void (*)(int))0
#define SIG_IGN (void (*)(int))1

void *signal(int sig, void (*func)(int));

#ifdef __cplusplus
}
#endif

#endif
