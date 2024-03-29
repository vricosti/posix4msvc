/*
 * DIRENT.H (formerly DIRLIB.H)
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 */

#ifndef _DIRENT_H_
#define _DIRENT_H_

/* All the headers include this file. */
//#include <sys/cdefs.h>

#include <io.h>
#include <sys/types_ex.h>

#ifndef RC_INVOKED

#pragma pack(push,_CRT_PACKING)

#ifdef __cplusplus
extern "C" {
#endif

struct dirent
{
	long		d_ino;		/* Always zero. */
	unsigned short	d_reclen;	/* Always zero. */
	unsigned short	d_namlen;	/* Length of name in d_name. */
	char		d_name[260]; /* [FILENAME_MAX] */ /* File name. */
};

/*
 * This is an internal data structure. Good programmers will not use it
 * except as an argument to one of the functions below.
 * dd_stat field is now int (was short in older versions).
 */
typedef struct
{
	/* disk transfer area for this dir */
	struct _finddata_t	dd_dta;

	/* dirent struct to return from dir (NOTE: this makes this thread
	 * safe as long as only one thread uses a particular DIR struct at
	 * a time) */
	struct dirent		dd_dir;

	/* _findnext handle */
	intptr_t		dd_handle;
	
	/* Used by dirfd to store fd returned and deallocate them during closedir */
	//int				dd_fds[1024];
	/*
	 * Status of search:
	 *   0 = not started yet (next entry to read is first entry)
	 *  -1 = off the end
	 *   positive = 0 based index of next entry
	 */
	int			dd_stat;

	/* given path for dir with search pattern (struct is extended) */
	char			dd_name[1];
} DIR;

DIR* __cdecl  opendir (const char*);
struct dirent* __cdecl  readdir (DIR*);
int __cdecl  closedir (DIR*);
void __cdecl  rewinddir (DIR*);
long __cdecl  telldir (DIR*);
void __cdecl  seekdir (DIR*, long);
int __cdecl  dirfd(DIR *dirp);

int __cdecl readdir_r(DIR * dirp, struct dirent * entry,
					  struct dirent ** result);
					 
					 
/* wide char versions */

struct _wdirent
{
	long		d_ino;		/* Always zero. */
	unsigned short	d_reclen;	/* Always zero. */
	unsigned short	d_namlen;	/* Length of name in d_name. */
	wchar_t		d_name[260]; /* [FILENAME_MAX] */ /* File name. */
};

/*
 * This is an internal data structure. Good programmers will not use it
 * except as an argument to one of the functions below.
 */
typedef struct
{
	/* disk transfer area for this dir */
	struct _wfinddata_t	dd_dta;

	/* dirent struct to return from dir (NOTE: this makes this thread
	 * safe as long as only one thread uses a particular DIR struct at
	 * a time) */
	struct _wdirent		dd_dir;

	/* _findnext handle */
	intptr_t		dd_handle;

	/* Used by dirfd to store fd returned and deallocate them during closedir */
	//int				dd_fds[1024];
	
	/*
	 * Status of search:
	 *   0 = not started yet (next entry to read is first entry)
	 *  -1 = off the end
	 *   positive = 0 based index of next entry
	 */
	int			dd_stat;

	/* given path for dir with search pattern (struct is extended) */
	wchar_t			dd_name[1];
} _WDIR;

_WDIR* __cdecl  _wopendir (const wchar_t*);
struct _wdirent* __cdecl  _wreaddir (_WDIR*);
int __cdecl  _wclosedir (_WDIR*);
void __cdecl  _wrewinddir (_WDIR*);
long __cdecl  _wtelldir (_WDIR*);
void __cdecl  _wseekdir (_WDIR*, long);
int __cdecl  _wdirfd(_WDIR *dirp);

#ifdef	__cplusplus
}
#endif

#pragma pack(pop)

#endif	/* Not RC_INVOKED */

#endif	/* Not _DIRENT_H_ */

