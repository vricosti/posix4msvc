/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#ifndef _INC_TYPES
#define _INC_TYPES

#ifndef _WIN32
#error Only Win32 target is supported!
#endif

//#include <sys/cdefs.h>

#include <time.h>


typedef unsigned int useconds_t;
typedef __int32 ssize_t;


#if __STDC__
typedef unsigned short _ino_t;
typedef _ino_t ino_t;

typedef unsigned int _dev_t;
typedef _dev_t dev_t;

typedef long _off_t;
typedef _off_t off_t;

typedef __int64 _off64_t;
typedef _off64_t off64_t;

#endif


#ifdef _USE_32BIT_TIME_T
#ifdef _WIN64
#undef _USE_32BIT_TIME_T
#endif
#endif

#ifndef _TIME32_T_DEFINED
#define _TIME32_T_DEFINED
typedef long	__time32_t;
#endif

#ifndef _TIME64_T_DEFINED
#define _TIME64_T_DEFINED
typedef __int64	__time64_t;
#endif

#ifndef _TIME_T_DEFINED
#define _TIME_T_DEFINED
#ifdef _USE_32BIT_TIME_T
typedef __time32_t time_t;
#else
typedef __time64_t time_t;
#endif
#endif

#ifndef _GID_T_DEFINED
#define _GID_T_DEFINED
typedef int gid_t;
typedef int uid_t;
#endif
   
#ifndef _INO_T_DEFINED
#define _INO_T_DEFINED
typedef unsigned short _ino_t;
#ifndef	NO_OLDNAMES
typedef unsigned short ino_t;
#endif
#endif

#ifndef _DEV_T_DEFINED
#define _DEV_T_DEFINED
typedef unsigned int _dev_t;
#ifndef	NO_OLDNAMES
typedef unsigned int dev_t;
#endif
#endif

/* Major, minor numbers, dev_t's. */
#define	major(x)	((int32_t)(((uint32_t)(x) >> 24) & 0xff))
#define	minor(x)	((int32_t)((x) & 0xffffff))
#define	makedev(x,y)	((dev_t)(((x) << 24) | (y)))

#ifndef _PID_T_
#define	_PID_T_
#ifndef _WIN64
typedef int	_pid_t;
#else
__MINGW_EXTENSION
typedef __int64	_pid_t;
#endif

#ifndef	NO_OLDNAMES
#undef pid_t
typedef _pid_t	pid_t;
#endif
#endif	/* Not _PID_T_ */

#ifndef _MODE_T_
#define	_MODE_T_
typedef unsigned short _mode_t;

#ifndef	NO_OLDNAMES
typedef _mode_t	mode_t;
#endif
#endif	/* Not _MODE_T_ */

//#include <_mingw_off_t.h>

#ifndef __NO_ISOCEXT
typedef unsigned int useconds_t;
#endif


//struct timespec {
//  time_t  tv_sec;	/* Seconds */
//  long    tv_nsec;	/* Nanoseconds */
//};

struct itimerspec {
  struct timespec  it_interval;	/* Timer period */
  struct timespec  it_value;	/* Timer expiration */
};


#ifndef	_SUSECONDS_T
#define	_SUSECONDS_T
typedef long	suseconds_t;	/* signed # of microseconds */
#endif	/* _SUSECONDS_T */


#ifndef _SIGSET_T_
#define _SIGSET_T_
#ifdef _WIN64
__MINGW_EXTENSION
typedef unsigned long long _sigset_t;
#else
typedef unsigned long	_sigset_t;
#endif


typedef unsigned short _mode_t;

#ifdef _POSIX
typedef _sigset_t	sigset_t;
#endif
#endif	/* Not _SIGSET_T_ */

#endif	/* _INC_TYPES */

