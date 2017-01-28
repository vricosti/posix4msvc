
#ifndef _P4MSVC_IOCTL_H_
#define _P4MSVC_IOCTL_H_

//#include <sys/cdefs.h>

#include <sys/stat.h>
#include <sys/types_ex.h>

#ifndef _INC_FCNTL
#define _O_RDONLY      0x0000  // open for reading only
#define _O_WRONLY      0x0001  // open for writing only
#define _O_RDWR        0x0002  // open for reading and writing
#define _O_APPEND      0x0008  // writes done at eof

#define _O_CREAT       0x0100  // create and open file
#define _O_TRUNC       0x0200  // open and truncate
#define _O_EXCL        0x0400  // open only if file doesn't already exist

// O_TEXT files have <cr><lf> sequences translated to <lf> on read()'s and <lf>
// sequences translated to <cr><lf> on write()'s

#define _O_TEXT        0x4000  // file mode is text (translated)
#define _O_BINARY      0x8000  // file mode is binary (untranslated)
#define _O_WTEXT       0x10000 // file mode is UTF16 (translated)
#define _O_U16TEXT     0x20000 // file mode is UTF16 no BOM (translated)
#define _O_U8TEXT      0x40000 // file mode is UTF8  no BOM (translated)

// macro to translate the C 2.0 name used to force binary mode for files
#define _O_RAW _O_BINARY

#define _O_NOINHERIT   0x0080  // child process doesn't inherit file
#define _O_TEMPORARY   0x0040  // temporary file bit (file is deleted when last handle is closed)
#define _O_SHORT_LIVED 0x1000  // temporary storage file, try not to flush
#define _O_OBTAIN_DIR  0x2000  // get information about a directory
#define _O_SEQUENTIAL  0x0020  // file access is primarily sequential
#define _O_RANDOM      0x0010  // file access is primarily random

#if __STDC__
#define O_RDONLY     _O_RDONLY
#define O_WRONLY     _O_WRONLY
#define O_RDWR       _O_RDWR
#define O_APPEND     _O_APPEND
#define O_CREAT      _O_CREAT
#define O_TRUNC      _O_TRUNC
#define O_EXCL       _O_EXCL
#define O_TEXT       _O_TEXT
#define O_BINARY     _O_BINARY
#define O_RAW        _O_BINARY
#define O_TEMPORARY  _O_TEMPORARY
#define O_NOINHERIT  _O_NOINHERIT
#define O_SEQUENTIAL _O_SEQUENTIAL
#define O_RANDOM     _O_RANDOM
#endif /*!__STDC__*/

#if __POSIX_VISIBLE >= 200809
#define	O_CLOEXEC	0x00020000	/* atomically set FD_CLOEXEC */
#endif
#define	O_FBLOCKING	0x00040000	/* force blocking I/O */
#define	O_FNONBLOCKING	0x00080000	/* force non-blocking I/O */
#define	O_FAPPEND	0x00100000	/* force append mode for write */
#define	O_FOFFSET	0x00200000	/* force specific offset */
#define	O_FSYNCWRITE	0x00400000	/* force synchronous write */
#define	O_FASYNCWRITE	0x00800000	/* force asynchronous write */
#if defined(_KERNEL) || defined(_KERNEL_STRUCTURES)
#define	O_UNUSED24	0x01000000
#define	O_UNUSED25	0x02000000
#define	O_UNUSED26	0x04000000
#endif

#endif /*_INC_FCNTL*/

// xxxxat(openat, creatat, ...) flags
#define AT_FDCWD                -100
#define AT_SYMLINK_NOFOLLOW     0x100
#define AT_REMOVEDIR            0x200
#define AT_SYMLINK_FOLLOW       0x400
#define AT_NO_AUTOMOUNT         0x800
#define AT_EMPTY_PATH           0x1000


/*
* Constants used for fcntl(2)
*/

/* command values */
#define	F_DUPFD			0		/* duplicate file descriptor */
#define	F_GETFD			1		/* get file descriptor flags */
#define	F_SETFD			2		/* set file descriptor flags */
#define	F_GETFL			3		/* get file status flags */
#define	F_SETFL			4		/* set file status flags */
#define	F_GETOWN		5		/* get SIGIO/SIGURG proc/pgrp */
#define F_SETOWN		6		/* set SIGIO/SIGURG proc/pgrp */
#define	F_GETLK			7		/* get record locking information */
#define	F_SETLK			8		/* set record locking information */
#define	F_SETLKW		9		/* F_SETLK; wait if blocked */
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
#define F_CHKCLEAN      41      /* Used for regression test */
#define F_PREALLOCATE   42		/* Preallocate storage */
#define F_SETSIZE       43		/* Truncate a file without zeroing space */	
#define F_RDADVISE      44      /* Issue an advisory read async with no copy to user */
#define F_RDAHEAD       45      /* turn read ahead off/on for this fd */
#define F_READBOOTSTRAP 46      /* Read bootstrap from disk */
#define F_WRITEBOOTSTRAP 47     /* Write bootstrap on disk */
#define F_NOCACHE       48      /* turn data caching off/on for this fd */
#define F_LOG2PHYS		49		/* file offset to device offset */
#define F_GETPATH       50      /* return the full path of the fd */
#define F_FULLFSYNC     51		/* fsync + ask the drive to flush to the media */
#define F_PATHPKG_CHECK 52      /* find which component (if any) is a package */
#define F_FREEZE_FS     53      /* "freeze" all fs operations */
#define F_THAW_FS       54      /* "thaw" all fs operations */
#define	F_GLOBAL_NOCACHE 55		/* turn data caching off/on (globally) for this file */

#ifdef PRIVATE
#define	F_OPENFROM	56		/* SPI: open a file relative to fd (must be a dir) */
#define	F_UNLINKFROM	57		/* SPI: open a file relative to fd (must be a dir) */
#define	F_CHECK_OPENEVT 58		/* SPI: if a process is marked OPENEVT, or in O_EVTONLY on opens of this vnode */
#endif /* PRIVATE */

#define F_ADDSIGS	59		/* add detached signatures */

#define F_MARKDEPENDENCY 60             /* this process hosts the device supporting the fs backing this fd */

#define F_ADDFILESIGS	61		/* add signature from same file (used by dyld for shared libs) */

// FS-specific fcntl()'s numbers begin at 0x00010000 and go up
#define FCNTL_FS_SPECIFIC_BASE  0x00010000

#endif /* (_POSIX_C_SOURCE && !_DARWIN_C_SOURCE) */

/* file descriptor flags (F_GETFD, F_SETFD) */
#define	FD_CLOEXEC	1		/* close-on-exec flag */

/* record locking flags (F_GETLK, F_SETLK, F_SETLKW) */
#define	F_RDLCK			1			/* shared or read lock */
#define	F_UNLCK			2			/* unlock */
#define	F_WRLCK			3			/* exclusive or write lock */
#ifdef KERNEL
#define	F_WAIT			0x010		/* Wait until lock is granted */
#define	F_FLOCK			0x020	 	/* Use flock(2) semantics for lock */
#define	F_POSIX			0x040	 	/* Use POSIX semantics for lock */
#define	F_PROV			0x080		/* Non-coelesced provisional lock */
#define F_WAKE1_SAFE    0x100       /* its safe to only wake one waiter */
#endif

/*
* [XSI] The values used for l_whence shall be defined as described
* in <unistd.h>
*/
#ifndef SEEK_SET
#define	SEEK_SET	0	/* set file offset to offset */
#define	SEEK_CUR	1	/* set file offset to current plus offset */
#define	SEEK_END	2	/* set file offset to EOF plus offset */
#endif	/* !SEEK_SET */

#include <sys/stat_ex.h>
#if TRYREFACTOR
/*
* [XSI] The symbolic names for file modes for use as values of mode_t
* shall be defined as described in <sys/stat.h>
*/
#ifndef S_IFMT
/* File type */
#define	S_IFMT		0170000		/* [XSI] type of file mask */
#define	S_IFIFO		0010000		/* [XSI] named pipe (fifo) */
#define	S_IFCHR		0020000		/* [XSI] character special */
#define	S_IFDIR		0040000		/* [XSI] directory */
#define	S_IFBLK		0060000		/* [XSI] block special */
#define	S_IFREG		0100000		/* [XSI] regular */
#define	S_IFLNK		0120000		/* [XSI] symbolic link */
#define	S_IFSOCK	0140000		/* [XSI] socket */
#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
#define	S_IFWHT		0160000		/* whiteout */
#endif

/* File mode */
/* Read, write, execute/search by owner */
#define	S_IRWXU		0000700		/* [XSI] RWX mask for owner */
#define	S_IRUSR		0000400		/* [XSI] R for owner */
#define	S_IWUSR		0000200		/* [XSI] W for owner */
#define	S_IXUSR		0000100		/* [XSI] X for owner */
/* Read, write, execute/search by group */
#define	S_IRWXG		0000070		/* [XSI] RWX mask for group */
#define	S_IRGRP		0000040		/* [XSI] R for group */
#define	S_IWGRP		0000020		/* [XSI] W for group */
#define	S_IXGRP		0000010		/* [XSI] X for group */
/* Read, write, execute/search by others */
#define	S_IRWXO		0000007		/* [XSI] RWX mask for other */
#define	S_IROTH		0000004		/* [XSI] R for other */
#define	S_IWOTH		0000002		/* [XSI] W for other */
#define	S_IXOTH		0000001		/* [XSI] X for other */

#define	S_ISUID		0004000		/* [XSI] set user id on execution */
#define	S_ISGID		0002000		/* [XSI] set group id on execution */
#define	S_ISVTX		0001000		/* [XSI] directory restrcted delete */

#if !defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE)
#define	S_ISTXT		S_ISVTX		/* sticky bit: not supported */
#define	S_IREAD		S_IRUSR		/* backward compatability */
#define	S_IWRITE	S_IWUSR		/* backward compatability */
#define	S_IEXEC		S_IXUSR		/* backward compatability */
#endif
#endif	/* !S_IFMT */
#endif

#ifdef  __cplusplus
extern "C" {
#endif

extern int __cdecl fcntl(int fd, int cmd, ... /* arg */ );

#if __STDC__

#ifdef __clang__
#pragma clang diagnostic ignored "-Winconsistent-dllimport"
#endif

//#ifdef _USE_32BIT_TIME_T
//struct _stat32
//{
//	_dev_t         st_dev;
//	_ino_t         st_ino;
//	unsigned short st_mode;
//	short          st_nlink;
//	short          st_uid;
//	short          st_gid;
//	_dev_t         st_rdev;
//	_off_t         st_size;
//	__time32_t     st_atime;
//	__time32_t     st_mtime;
//	__time32_t     st_ctime;
//};
//
//#define os_stat _stat32
//#define os_fstat _fstat32
//
//#else
//struct stat
//{
//	_dev_t         st_dev;
//	_ino_t         st_ino;
//	unsigned short st_mode;
//	short          st_nlink;
//	short          st_uid;
//	short          st_gid;
//	_dev_t         st_rdev;
//	_off_t         st_size;
//	__time64_t     st_atime;
//	__time64_t     st_mtime;
//	__time64_t     st_ctime;
//};
//
//#define os_stat		_stat64i32
//#define os_fstat	_fstat64i32
//
//#endif //_USE_32BIT_TIME_T



extern char *__cdecl strdup(const char *s);
extern pid_t __cdecl getpid(void);
extern int __cdecl access(const char *path, int mode);
extern int __cdecl chmod(const char *pathname, mode_t mode);
extern int __cdecl chsize(int fd, long size);
extern int __cdecl fchmod(int fd, mode_t mode);
extern int __cdecl open(const char*, int, ...);
extern int __cdecl close(int fd);
extern int __cdecl creat(const char*, mode_t);
extern int __cdecl stat(char const* pathname, struct stat* _Stat);
extern int __cdecl fstat(int fd, struct stat *buf);
extern int __cdecl lstat(const char *path, struct stat *buf);
extern int __cdecl dup(int oldfd);
extern int __cdecl dup2(int oldfd, int newfd);
extern int __cdecl eof(int fd);
extern long __cdecl filelength(int fd);
extern int __cdecl isatty(int fd);
extern int __cdecl locking(int fd, int mode, long nbytes);
extern long _cdecl lseek(int fd, long offset, int origin);
extern off64_t __cdecl  lseek64(int fd, off64_t offset, int whence);
extern char *_cdecl mktemp(char *template);
extern ssize_t _cdecl read(int fd, void *buf, size_t count);
extern void *_cdecl setmode(const char *mode_str); /* msvc implementation returns int */
extern int _cdecl sopen(const char *filename, int oflag, int shflag, ...);
extern long _cdecl tell(int fd);
extern int _cdecl umask(int pmode);
extern int _cdecl unlink(const char *filename);
extern ssize_t write(int fd, const void *buf, size_t count); /* msvc implementation returns int*/



#endif

extern int __cdecl openat(int, const char*, int, ...);
extern int __cdecl fstatat(int dirfd, const char *pathname, struct stat *buf, int flags);
extern int __cdecl unlinkat(int dirfd, const char *pathname, int flags);




#ifdef  __cplusplus
}
#endif


#endif