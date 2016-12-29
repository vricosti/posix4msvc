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

#include <sys/featuretest.h>

 /* Machine type dependent parameters. */
#include <machine/types.h>

#include <machine/ansi.h>
#include <machine/int_types.h>


#include <sys/ansi.h>

#ifndef	int8_t
typedef	__int8_t	int8_t;
#define	int8_t		__int8_t
#endif

#ifndef	uint8_t
typedef	__uint8_t	uint8_t;
#define	uint8_t		__uint8_t
#endif

#ifndef	int16_t
typedef	__int16_t	int16_t;
#define	int16_t		__int16_t
#endif

#ifndef	uint16_t
typedef	__uint16_t	uint16_t;
#define	uint16_t	__uint16_t
#endif

#ifndef	int32_t
typedef	__int32_t	int32_t;
#define	int32_t		__int32_t
#endif

#ifndef	uint32_t
typedef	__uint32_t	uint32_t;
#define	uint32_t	__uint32_t
#endif

#ifndef	int64_t
typedef	__int64_t	int64_t;
#define	int64_t		__int64_t
#endif

#ifndef	uint64_t
typedef	__uint64_t	uint64_t;
#define	uint64_t	__uint64_t
#endif

typedef	uint8_t		u_int8_t;
typedef	uint16_t	u_int16_t;
typedef	uint32_t	u_int32_t;
typedef	uint64_t	u_int64_t;

#include <machine/endian.h>

#if defined(_NETBSD_SOURCE)
typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;

typedef unsigned char	unchar;		/* Sys V compatibility */
typedef	unsigned short	ushort;		/* Sys V compatibility */
typedef	unsigned int	uint;		/* Sys V compatibility */
typedef unsigned long	ulong;		/* Sys V compatibility */
#endif

typedef	uint64_t	u_quad_t;	/* quads */
typedef	int64_t		quad_t;
typedef	quad_t *	qaddr_t;

/*
* The types longlong_t and u_longlong_t exist for use with the
* Sun-derived XDR routines involving these types, and their usage
* in other contexts is discouraged.  Further note that these types
* may not be equivalent to "long long" and "unsigned long long",
* they are only guaranteed to be signed and unsigned 64-bit types
* respectively.  Portable programs that need 64-bit types should use
* the C99 types int64_t and uint64_t instead.
*/

typedef	int64_t		longlong_t;	/* for XDR */
typedef	uint64_t	u_longlong_t;	/* for XDR */

typedef	int64_t		blkcnt_t;	/* fs block count */
typedef	int32_t		blksize_t;	/* fs optimal block size */

#ifndef	fsblkcnt_t
typedef	__fsblkcnt_t	fsblkcnt_t;	/* fs block count (statvfs) */
#define fsblkcnt_t	__fsblkcnt_t
#endif

#ifndef	fsfilcnt_t
typedef	__fsfilcnt_t	fsfilcnt_t;	/* fs file count */
#define fsfilcnt_t	__fsfilcnt_t
#endif

#if !defined(_KERNEL) && !defined(_STANDALONE)
                                    /* We don't and shouldn't use caddr_t in the kernel anymore */
#ifndef	caddr_t
typedef	__caddr_t	caddr_t;	/* core address */
#define	caddr_t		__caddr_t
#endif
#endif

#ifdef __daddr_t
typedef	__daddr_t	daddr_t;	/* disk address */
#undef __daddr_t
#else
typedef	int64_t		daddr_t;	/* disk address */
#endif

typedef	uint64_t	dev_t;		/* device number */
typedef	uint32_t	fixpt_t;	/* fixed point number */

#ifndef	gid_t
typedef	__gid_t		gid_t;		/* group id */
#define	gid_t		__gid_t
#endif

typedef	uint32_t	id_t;		/* group id, process id or user id */
typedef	uint64_t	ino_t;		/* inode number */
typedef	long		key_t;		/* IPC key (for Sys V IPC) */

#ifndef	mode_t
typedef	__mode_t	mode_t;		/* permissions */
#define	mode_t		__mode_t
#endif

typedef	uint32_t	nlink_t;	/* link count */

#ifndef	off_t
typedef	__off_t		off_t;		/* file offset */
#define	off_t		__off_t
#endif

typedef __int64 _off64_t;
typedef _off64_t off64_t;

#ifndef	pid_t
typedef	__pid_t		pid_t;		/* process id */
#define	pid_t		__pid_t
#endif
typedef int32_t		lwpid_t;	/* LWP id */
typedef uint64_t	rlim_t;		/* resource limit */
typedef	int32_t		segsz_t;	/* segment size */
typedef	int32_t		swblk_t;	/* swap offset */

#ifndef	uid_t
typedef	__uid_t		uid_t;		/* user id */
#define	uid_t		__uid_t
#endif

typedef int		mqd_t;

typedef	unsigned long	cpuid_t;

typedef	int		psetid_t;

typedef volatile __cpu_simple_lock_nv_t __cpu_simple_lock_t;

#if defined(_KERNEL) || defined(_STANDALONE)

#include <sys/stdbool.h>

/*
* Deprecated Mach-style boolean_t type.  Should not be used by new code.
*/
typedef int	boolean_t;
#ifndef TRUE
#define	TRUE	1
#endif
#ifndef FALSE
#define	FALSE	0
#endif

#endif /* _KERNEL || _STANDALONE */

#if defined(_KERNEL) || defined(_LIBC)
/*
* semctl(2)'s argument structure.  This is here for the benefit of
* <sys/syscallargs.h>.  It is not in the user's namespace in SUSv2.
* The SUSv2 semctl(2) takes variable arguments.
*/
union __semun {
    int		val;		/* value for SETVAL */
    struct semid_ds	*buf;		/* buffer for IPC_STAT & IPC_SET */
    unsigned short	*array;		/* array for GETALL & SETALL */
};
#include <sys/stdint.h>
#endif /* _KERNEL || _LIBC */

/*
* These belong in unistd.h, but are placed here too to ensure that
* long arguments will be promoted to off_t if the program fails to
* include that header or explicitly cast them to off_t.
*/
#if defined(_NETBSD_SOURCE)
#ifndef __OFF_T_SYSCALLS_DECLARED
#define __OFF_T_SYSCALLS_DECLARED
#ifndef _KERNEL
#include <sys/cdefs.h>
__BEGIN_DECLS
//off_t	 lseek(int, off_t, int);
//int	 ftruncate(int, off_t);
//int	 truncate(const char *, off_t);
__END_DECLS
#endif /* !_KERNEL */
#endif /* __OFF_T_SYSCALLS_DECLARED */
#endif /* defined(_NETBSD_SOURCE) */

#if defined(_NETBSD_SOURCE)
/* Major, minor numbers, dev_t's. */
typedef int32_t __devmajor_t, __devminor_t;
#define devmajor_t __devmajor_t
#define devminor_t __devminor_t
#define NODEVMAJOR (-1)
#define	major(x)	((devmajor_t)(((uint32_t)(x) & 0x000fff00) >>  8))
#define	minor(x)	((devminor_t)((((uint32_t)(x) & 0xfff00000) >> 12) | \
				   (((uint32_t)(x) & 0x000000ff) >>  0)))
#define	makedev(x,y)	((dev_t)((((dev_t)(x) <<  8) & 0x000fff00U) | \
				 (((dev_t)(y) << 12) & 0xfff00000U) | \
				 (((dev_t)(y) <<  0) & 0x000000ffU)))
#endif

#ifdef	_BSD_CLOCK_T_
//typedef	_BSD_CLOCK_T_		clock_t;
#undef	_BSD_CLOCK_T_
#endif

#ifdef	_BSD_PTRDIFF_T_
typedef	_BSD_PTRDIFF_T_		ptrdiff_t;
#undef	_BSD_PTRDIFF_T_
#endif

#ifdef	_BSD_SIZE_T_
typedef	_BSD_SIZE_T_		size_t;
#define _SIZE_T
#undef	_BSD_SIZE_T_
#endif

#ifdef	_BSD_SSIZE_T_
typedef	_BSD_SSIZE_T_		ssize_t;
#undef	_BSD_SSIZE_T_
#endif

#ifdef	_BSD_TIME_T_
typedef	_BSD_TIME_T_		time_t;
#undef	_BSD_TIME_T_
#endif

#ifdef	_BSD_CLOCKID_T_
typedef	_BSD_CLOCKID_T_		clockid_t;
#undef	_BSD_CLOCKID_T_
#endif

#ifdef	_BSD_TIMER_T_
typedef	_BSD_TIMER_T_		timer_t;
#undef	_BSD_TIMER_T_
#endif

#ifdef	_BSD_SUSECONDS_T_
typedef	_BSD_SUSECONDS_T_	suseconds_t;
#undef	_BSD_SUSECONDS_T_
#endif

#ifdef	_BSD_USECONDS_T_
typedef	_BSD_USECONDS_T_	useconds_t;
#undef	_BSD_USECONDS_T_
#endif

#ifdef _NETBSD_SOURCE
#include <sys/fd_set.h>

#define	NBBY			8

typedef struct kauth_cred *kauth_cred_t;

typedef int pri_t;

#endif

#if defined(__STDC__) && (defined(_KERNEL) || defined(_KMEMUSER))
/*
* Forward structure declarations for function prototypes.  We include the
* common structures that cross subsystem boundaries here; others are mostly
* used in the same place that the structure is defined.
*/
struct	lwp;
typedef struct lwp lwp_t;
struct	__ucontext;
struct	proc;
typedef struct proc proc_t;
struct	pgrp;
struct	rusage;
struct	file;
typedef struct file file_t;
struct	buf;
typedef struct buf buf_t;
struct	tty;
struct	uio;
#endif

#ifdef _KERNEL
#define SET(t, f)	((t) |= (f))
#define	ISSET(t, f)	((t) & (f))
#define	CLR(t, f)	((t) &= ~(f))
#endif

#if !defined(_KERNEL) && !defined(_STANDALONE)
#if (_POSIX_C_SOURCE - 0L) >= 199506L || (_XOPEN_SOURCE - 0) >= 500 || \
    defined(_NETBSD_SOURCE)
//#include <pthread_types.h>
#endif
#endif


//#include <time.h>
//
//
//typedef unsigned int useconds_t;
//typedef __int32 ssize_t;
//
//
//#if __STDC__
//typedef unsigned short _ino_t;
//typedef _ino_t ino_t;
//
//typedef unsigned int _dev_t;
//typedef _dev_t dev_t;
//
//typedef long _off_t;
//typedef _off_t off_t;
//
//typedef __int64 _off64_t;
//typedef _off64_t off64_t;
//
//#endif
//
//
//#ifdef _USE_32BIT_TIME_T
//#ifdef _WIN64
//#undef _USE_32BIT_TIME_T
//#endif
//#endif
//
//#ifndef _TIME32_T_DEFINED
//#define _TIME32_T_DEFINED
//typedef long	__time32_t;
//#endif
//
//#ifndef _TIME64_T_DEFINED
//#define _TIME64_T_DEFINED
//typedef __int64	__time64_t;
//#endif
//
//#ifndef _TIME_T_DEFINED
//#define _TIME_T_DEFINED
//#ifdef _USE_32BIT_TIME_T
//typedef __time32_t time_t;
//#else
//typedef __time64_t time_t;
//#endif
//#endif
//
//#ifndef _GID_T_DEFINED
//#define _GID_T_DEFINED
//typedef int gid_t;
//typedef int uid_t;
//#endif
//   
//#ifndef _INO_T_DEFINED
//#define _INO_T_DEFINED
//typedef unsigned short _ino_t;
//#ifndef	NO_OLDNAMES
//typedef unsigned short ino_t;
//#endif
//#endif
//
//#ifndef _DEV_T_DEFINED
//#define _DEV_T_DEFINED
//typedef unsigned int _dev_t;
//#ifndef	NO_OLDNAMES
//typedef unsigned int dev_t;
//#endif
//#endif
//
///* Major, minor numbers, dev_t's. */
//#define	major(x)	((int32_t)(((uint32_t)(x) >> 24) & 0xff))
//#define	minor(x)	((int32_t)((x) & 0xffffff))
//#define	makedev(x,y)	((dev_t)(((x) << 24) | (y)))
//
//#ifndef _PID_T_
//#define	_PID_T_
//#ifndef _WIN64
//typedef int	_pid_t;
//#else
//__MINGW_EXTENSION
//typedef __int64	_pid_t;
//#endif
//
//#ifndef	NO_OLDNAMES
//#undef pid_t
//typedef _pid_t	pid_t;
//#endif
//#endif	/* Not _PID_T_ */
//
//#ifndef _MODE_T_
//#define	_MODE_T_
//typedef unsigned short _mode_t;
//
//#ifndef	NO_OLDNAMES
//typedef _mode_t	mode_t;
//#endif
//#endif	/* Not _MODE_T_ */
//
////#include <_mingw_off_t.h>
//
//#ifndef __NO_ISOCEXT
//typedef unsigned int useconds_t;
//#endif
//
//
////struct timespec {
////  time_t  tv_sec;	/* Seconds */
////  long    tv_nsec;	/* Nanoseconds */
////};
//
//struct itimerspec {
//  struct timespec  it_interval;	/* Timer period */
//  struct timespec  it_value;	/* Timer expiration */
//};
//
//
//#ifndef	_SUSECONDS_T
//#define	_SUSECONDS_T
//typedef long	suseconds_t;	/* signed # of microseconds */
//#endif	/* _SUSECONDS_T */
//
//
//#ifndef _SIGSET_T_
//#define _SIGSET_T_
//#ifdef _WIN64
//__MINGW_EXTENSION
//typedef unsigned long long _sigset_t;
//#else
//typedef unsigned long	_sigset_t;
//#endif
//
//
//typedef unsigned short _mode_t;
//
//#ifdef _POSIX
//typedef _sigset_t	sigset_t;
//#endif
//#endif	/* Not _SIGSET_T_ */
//
//
//#define OPEN_MAX	512
//#define _PATH_TMP   "/tmp/"

#endif	/* _INC_TYPES */

