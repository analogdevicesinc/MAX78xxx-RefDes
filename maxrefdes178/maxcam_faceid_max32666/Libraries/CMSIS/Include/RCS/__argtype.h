// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef ARGTYPE

typedef void *ARGTYPE_PtrArg;
typedef char *ARGTYPE_CharPtrArg;

#define ARGTYPE_PtrArg_NULL NULL

#define ARGTYPE va_list
#define ARGTYPE_getIntArg(a) va_arg(a, int)
#define ARGTYPE_getUnsignedArg(a) va_arg(a, unsigned)
#define ARGTYPE_getLongArg(a) va_arg(a, long)
#define ARGTYPE_getUnsignedLongArg(a) va_arg(a, unsigned long)
#define ARGTYPE_getLongLongArg(a) va_arg(a, value_t)
#define ARGTYPE_getDoubleArg(a) va_arg(a, double)
#define ARGTYPE_getPtrArg(a) va_arg(a, void *)
#define ARGTYPE_getStringArg(a) va_arg(a, char *)
#define ARGTYPE_setLongPtrArg(a, n) long *xp = va_arg(args, long *); *xp = n; 
#define ARGTYPE_setLongLongPtrArg(a, n) long long *xp = va_arg(args, long long *); *xp = n; 
#define ARGTYPE_setIntPtrArg(a, n) int *xp = va_arg(args, int *); *xp = n; 
#define ARGTYPE_setCharPtrArg(a, n) *(char *)va_arg(args, char *) = n; 
#define ARGTYPE_writePtrArgChar(a, ptr, v) *(char *)ptr = v
#define ARGTYPE_writePtrArgShort(a, ptr, v) *(short *)ptr = v
#define ARGTYPE_writePtrArgInt(a, ptr, v) *(int *)ptr = v
#define ARGTYPE_writePtrArgLong(a, ptr, v) *(long *)ptr = v
#define ARGTYPE_writePtrArgLongLong(a, ptr, v) *(int64_t *)ptr = v
#define ARGTYPE_writePtrArgUnsignedChar(a, ptr, v) *(unsigned char *)ptr = v
#define ARGTYPE_writePtrArgUnsignedShort(a, ptr, v) *(unsigned short *)ptr = v
#define ARGTYPE_writePtrArgUnsignedInt(a, ptr, v) *(unsigned int *)ptr = v
#define ARGTYPE_writePtrArgUnsignedLong(a, ptr, v) *(unsigned long *)ptr = v
#define ARGTYPE_writePtrArgUnsignedLongLong(a, ptr, v) *(uint64_t *)ptr = v
#define ARGTYPE_writePtrArgFloat(a, ptr, v) *(float *)ptr = v
#define ARGTYPE_writePtrArgDouble(a, ptr, v) *(double *)ptr = v
#define ARGTYPE_writePtrArgLongDouble(a, ptr, v) *(long double *)ptr = v
#endif
