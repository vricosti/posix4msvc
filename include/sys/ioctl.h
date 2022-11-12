/*
 * DIRENT.H (formerly DIRLIB.H)
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 */

#ifndef _IOCTL_H_
#define _IOCTL_H_

/* All the headers include this file. */
#include <crtdefs.h>

#if BUILD_WINDEVBLK
extern int __cdecl ioctl(int fd, unsigned long request, ...);
#endif

#endif	/* Not _IOCTL_H_ */

