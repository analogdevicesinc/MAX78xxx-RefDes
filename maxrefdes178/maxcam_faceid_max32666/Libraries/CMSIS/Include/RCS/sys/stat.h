// Rowley CrossWorks, runtime support.
//
//  Copyright (c) 2001-2018 Rowley Associates Limited.
//
// This file may be distributed under the terms of the License Agreement
// provided with this software.
//
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#ifndef _STAT_H
#define _STAT_H

#ifdef __cplusplus
extern "C" {
#endif

struct stat 
{
  mode_t st_mode;
  off_t	st_size;
};

#define	_IFMT  0170000
#define	_IFDIR 0040000
#define	_IFCHR 0020000
#define	_IFBLK 0060000
#define	_IFREG 0100000
#define	_IFLNK 0120000
#define	_IFSOCK	0140000
#define	_IFIFO 0010000

#define	S_IFMT  _IFMT
#define	S_IFDIR	_IFDIR
#define	S_IFCHR	_IFCHR
#define	S_IFBLK	_IFBLK
#define	S_IFREG	_IFREG
#define	S_IFLNK	_IFLNK
#define	S_IFSOCK _IFSOCK
#define	S_IFIFO _IFIFO

#define	S_IRWXU (S_IRUSR | S_IWUSR | S_IXUSR)
#define	S_IRUSR	0000400
#define	S_IWUSR	0000200
#define	S_IXUSR 0000100
#define	S_IRWXG	(S_IRGRP | S_IWGRP | S_IXGRP)
#define	S_IRGRP	0000040
#define	S_IWGRP	0000020
#define	S_IXGRP 0000010
#define	S_IRWXO	(S_IROTH | S_IWOTH | S_IXOTH)
#define	S_IROTH	0000004
#define	S_IWOTH	0000002
#define	S_IXOTH 0000001

#define	S_IREAD	S_IRUSR
#define	S_IWRITE S_IWUSR

int fstat(int fildes, struct stat *buf);

#ifdef __cplusplus
}
#endif

#endif
