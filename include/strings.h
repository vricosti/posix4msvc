/*
 * DIRENT.H (formerly DIRLIB.H)
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 */

#ifndef _STRINGS_H_
#define _STRINGS_H_

/* All the headers include this file. */
//#include <sys/cdefs.h>

#if defined(_WIN32) || defined(_WIN64) 
  #define strcasecmp _stricmp 
  #define strncasecmp _strnicmp 
#endif


#endif	/* Not _STRINGS_H_ */

