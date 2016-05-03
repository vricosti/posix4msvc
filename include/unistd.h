/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#ifndef _UNISTD_H
#define _UNISTD_H
#define __UNISTD_H_SOURCED__ 1

#include <io.h>
#include <process.h>

#include <sys/types.h>
#include <sys/types_ex.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib_ex.h>

/***********************************************************************/
/* Normally inside <sys/stat.h> */
#define	_S_IFBLK	0x3000	/* Block: Is this ever set under w32? */

#define S_IFMT _S_IFMT
#define S_IFDIR _S_IFDIR
#define S_IFCHR _S_IFCHR
#define S_IFREG _S_IFREG
#define S_IREAD _S_IREAD
#define S_IWRITE _S_IWRITE
#define S_IEXEC _S_IEXEC
#define	S_IFIFO		_S_IFIFO
#define	S_IFBLK		_S_IFBLK

#define	_S_IRWXU	(_S_IREAD | _S_IWRITE | _S_IEXEC)
#define	_S_IXUSR	_S_IEXEC
#define	_S_IWUSR	_S_IWRITE

#define	S_IRWXU		_S_IRWXU
#define	S_IXUSR		_S_IXUSR
#define	S_IWUSR		_S_IWUSR
#define	S_IRUSR		_S_IRUSR
#define	_S_IRUSR	_S_IREAD

#define S_IRGRP    (S_IRUSR >> 3)
#define S_IWGRP    (S_IWUSR >> 3)
#define S_IXGRP    (S_IXUSR >> 3)
#define S_IRWXG    (S_IRWXU >> 3)

#define S_IROTH    (S_IRGRP >> 3)
#define S_IWOTH    (S_IWGRP >> 3)
#define S_IXOTH    (S_IXGRP >> 3)
#define S_IRWXO    (S_IRWXG >> 3)

#define	S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#define	S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)
#define	S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define	S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)
#define	S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
/***********************************************************************/


#define	F_OK	0	/* Check for file existence */
#define	X_OK	1	/* Check for execute permission. */
#define	W_OK	2	/* Check for write permission */
#define	R_OK	4	/* Check for read permission */

/* These are also defined in stdio.h. */
#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif

/* These are also defined in stdio.h. */
#ifndef STDIN_FILENO
#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
#endif

/* Used by shutdown(2). */
#ifdef _POSIX_SOURCE

/* MySql connector already defined SHUT_RDWR. */
#ifndef SHUT_RDWR
#define SHUT_RD   0x00
#define SHUT_WR   0x01
#define SHUT_RDWR 0x02
#endif

#endif

#ifdef __cplusplus
extern "C" {
#endif

#if 0

#pragma push_macro("sleep")
#undef sleep
unsigned int __cdecl sleep (unsigned int);
#pragma pop_macro("sleep")

#if !defined __NO_ISOCEXT
#include <sys/types.h> /* For useconds_t. */

int __cdecl  usleep(useconds_t);
#endif  /* Not __NO_ISOCEXT */

#ifndef FTRUNCATE_DEFINED
#define FTRUNCATE_DEFINED
/* This is defined as a real library function to allow autoconf
   to verify its existence. */
#if !defined(NO_OLDNAMES) || defined(_POSIX)
int ftruncate(int, off32_t);
int ftruncate64(int, off64_t);
int truncate(const char *, off32_t);
int truncate64(const char *, off64_t);
#ifndef __CRT__NO_INLINE
__CRT_INLINE int ftruncate(int __fd, off32_t __length)
{
  return _chsize (__fd, __length);
}
#endif /* !__CRT__NO_INLINE */
#else
int ftruncate(int, _off_t);
int ftruncate64(int, _off64_t);
int truncate(const char *, _off_t);
int truncate64(const char *, _off64_t);
#ifndef __CRT__NO_INLINE
__CRT_INLINE int ftruncate(int __fd, _off_t __length)
{
  return _chsize (__fd, __length);
}
#endif /* !__CRT__NO_INLINE */
#endif
#endif /* FTRUNCATE_DEFINED */

#ifndef _FILE_OFFSET_BITS_SET_FTRUNCATE
#define _FILE_OFFSET_BITS_SET_FTRUNCATE
#if (defined(_FILE_OFFSET_BITS) && (_FILE_OFFSET_BITS == 64))
#define ftruncate ftruncate64
#endif /* _FILE_OFFSET_BITS_SET_FTRUNCATE */
#endif /* _FILE_OFFSET_BITS_SET_FTRUNCATE */

#ifndef _CRT_SWAB_DEFINED
#define _CRT_SWAB_DEFINED /* Also in stdlib.h */
  void __cdecl swab(char *_Buf1,char *_Buf2,int _SizeInBytes) ;
#endif

#ifndef _CRT_GETPID_DEFINED
#define _CRT_GETPID_DEFINED /* Also in process.h */
  int __cdecl getpid(void) ;
#endif

uid_t __cdecl getuid(void);
uid_t __cdecl geteuid(void);
gid_t __cdecl getgid(void);
gid_t __cdecl getegid(void);
int __cdecl seteuid(uid_t euid);
int __cdecl setegid(gid_t egid);

pid_t __cdecl fork(void);
int __cdecl pipe(int pipefd[2]);

char *__cdecl ttyname(int fd);
int __cdecl ttyname_r(int fd, char *buf, size_t buflen);
	   
/*int __cdecl select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval *timeout);

void __cdecl FD_CLR(int fd, fd_set *set);
int  __cdecl FD_ISSET(int fd, fd_set *set);
void __cdecl FD_SET(int fd, fd_set *set);
void __cdecl FD_ZERO(fd_set *set);

int __cdecl gethostname(char *name, size_t len);
int __cdecl sethostname(const char *name, size_t len);
*/





/* sysconf stuff */
#define _SC_HOST_NAME_MAX 	2048


long sysconf(int name);

#endif

/* sysconf stuff */
long __cdecl sysconf(int name);
#define _SC_HOST_NAME_MAX 	2048

ssize_t __cdecl readlink(const char *path, char *buf, size_t bufsiz);
int __cdecl readlinkat(int dirfd, const char *pathname, char *buf, size_t bufsiz);
int __cdecl faccessat(int dirfd, const char *pathname, int mode, int flags);

/* normally defined inside <stdlib.h> */
int __cdecl mkstemp(char *template);
//int __cdecl mkostemp(char *template, int flags);
/*------------------------------------*/

/* normally defined inside <string.h> */
char *__cdecl strndup(const char *s, size_t n);
char *__cdecl strdupa(const char *s);
char *__cdecl strndupa(const char *s, size_t n);
char *__cdecl strsep(char **stringp, const char *delim);


/* normally defined inside <sys/stat.h>*/
int __cdecl mkdir(const char *pathname, mode_t mode);

#if __STDC__
int __cdecl chmod(const char *path, mode_t mode);

extern int   __cdecl fcloseall(void);
extern FILE* __cdecl fdopen(int _FileHandle, char const* _Format);
extern int   __cdecl fgetchar(void);
extern int   __cdecl fileno(FILE* _Stream);
extern int   __cdecl flushall(void);
extern int   __cdecl fputchar(int _Ch);
extern int   __cdecl getw(FILE* _Stream);
extern int   __cdecl putw(int _Ch, FILE* _Stream);
extern int   __cdecl rmtmp(void);

#endif /*__STDC__*/


int __cdecl fchmod(int fd, mode_t mode);

/* normally defined inside <stdio.h>*/
int __cdecl asprintf(char **strp, const char *fmt, ...);
int __cdecl vasprintf(char **strp, const char *fmt, va_list ap);


int __cdecl fsync(int fd);
int __cdecl fdatasync(int fd);




#ifdef __cplusplus
}
#endif



#undef __UNISTD_H_SOURCED__
#endif /* _UNISTD_H */

