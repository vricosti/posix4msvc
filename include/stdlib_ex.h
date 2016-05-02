/*
 * DIRENT.H (formerly DIRLIB.H)
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 */

#ifndef _STDLIB_EX_H_
#define _STDLIB_EX_H_

//#include <sys/cdefs.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

int __cdecl setenv(const char *name, const char *value, int overwrite);
int __cdecl unsetenv(const char *name);

#ifdef	__cplusplus
}
#endif

#endif	/* Not _STDLIB_EX_H_ */

