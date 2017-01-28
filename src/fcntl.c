/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */

#define __USE_MAXGW

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winioctl.h>
#include <tchar.h>

#include <direct.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <bsd/string.h> /*strlcat && strlcpy*/
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <get_osfhandle-nothrow.h>
#include <p4msvc_utils.h>

#include "windevblk.h"






int __cdecl
sys_open(const char *pathname, unsigned short st_mode, int flags, mode_t mode);

int __cdecl
__openat(int fd, const char *pathname, int flags, mode_t mode);

//TODO move somewhere else

uid_t __cdecl 
getuid(void) { return 0; }

uid_t __cdecl 
geteuid(void) { return 0; }

gid_t __cdecl 
getgid(void) { return 0; }

gid_t __cdecl 
getegid(void) { return 0; }

int __cdecl 
seteuid(uid_t euid) { return 0;  }

int __cdecl 
setegid(gid_t egid) { return 0; }

char * __cdecl 
ttyname(int fd)
{
    return NULL;
}

int __cdecl 
ttyname_r(int fd, char *buf, size_t buflen)
{
    return ENOTTY;
}


int __cdecl
fcntl(int fd, int cmd, ... /* arg */)
{
	int ret;
	HANDLE hFile;

	ret = -1;
	UNREFERENCED_PARAMETER(cmd);

	hFile = (HANDLE)_get_osfhandle(fd);
	if (hFile != INVALID_HANDLE_VALUE)
	{

	}

	return ret;
}

#if __STDC__

int __cdecl
creat(const char* pathname, mode_t mode)
{
	return __openat(AT_FDCWD, pathname, _O_CREAT | _O_TRUNC | _O_WRONLY, mode);
}

int __cdecl
open(const char* pathname, int flags, ...)
{
	mode_t mode = 0;

	if ((_O_CREAT & flags) != 0)
	{
		va_list args;
		va_start(args, flags);
		mode = (mode_t)va_arg(args, int);
		va_end(args);
	}

	return __openat(AT_FDCWD, pathname, flags, mode);
}
#endif /*__STDC__*/

int __cdecl
openat(int fd, const char *pathname, int flags, ...)
{
	mode_t mode = 0;

	if ((_O_CREAT & flags) != 0)
	{
		va_list args;
		va_start(args, flags);
		mode = (mode_t)va_arg(args, int);
		va_end(args);
	}

	return __openat(fd, pathname, flags, mode);
}





int __cdecl
__openat(int dirfd, const char *pathname, int flags, mode_t mode)
{
	int fd = -1;
	HANDLE hDir = INVALID_HANDLE_VALUE;
	BOOL bRet = FALSE;
	DWORD dwRet = 0;
	char szBuffer[MAX_PATH + 1];
	size_t ulLen = 0;
	const char* szNormPath = NULL;

	// First we check pathname is valid
	if (!pathname || pathname[0] == '\0') {
		_set_errno(EINVAL);
		return -1;
	}

	// If pathname is not relative => ignore dirfd 
	p4msvc_norm_path_t norm_path;
	szNormPath = p4msvc_norm_path(pathname, &norm_path, TRUE);
	if (norm_path.isAbsolute) {
		return sys_open(szNormPath, norm_path.st_mode, flags, mode);
	}

	// If we receive a file descriptor first check 
	// we can get a native handle from it
	if (dirfd != AT_FDCWD) {
		hDir = (HANDLE)_get_osfhandle(dirfd);
		if (hDir == INVALID_HANDLE_VALUE) {
			_set_errno(EBADF);
			return -1;
		}
	}
	// Check if the native handle corresponds to a directory
	BY_HANDLE_FILE_INFORMATION info;
	memset(&info, 0, sizeof(BY_HANDLE_FILE_INFORMATION));
	bRet = GetFileInformationByHandle(hDir, &info);
	if (!bRet || (FILE_ATTRIBUTE_DIRECTORY != info.dwFileAttributes)) {
		_set_errno(ENOTDIR);
		return -1;
	}



	memset(szBuffer, 0, sizeof(szBuffer) / sizeof(szBuffer[0]));
	if (dirfd == AT_FDCWD) {
		_getcwd(szBuffer, MAX_PATH);
	}
	else {
		dwRet = GetFinalPathNameByHandle(hDir, szBuffer, MAX_PATH, FILE_NAME_NORMALIZED);
		if (dwRet > 4)
		{
			if (strncmp(szBuffer, "\\\\?\\", strlen("\\\\?\\")) == 0) {
				memmove(szBuffer, szBuffer + 4, strlen(szBuffer + 4) + 1);
			}
		}
	}
	//check we have something in our buffer
	ulLen = strlen(szBuffer);
	if (ulLen == 0) {
		_set_errno(EINVAL);
		return -1;
	}

	//Protect us from buffer overflow (strlcat)
	strlcat(szBuffer, "\\", MAX_PATH);
	strlcat(szBuffer, pathname, MAX_PATH);

	fd = sys_open(szBuffer, S_IFREG, flags, mode);


	return fd;
}

int __cdecl
sys_open(const char *pathname, unsigned short st_mode, int flags, mode_t mode)
{
	int fd = -1;
	HANDLE hDevBlock;
	char szPath[MAX_PATH];
	char str[7];
	size_t ulLen, offset;
	int devNb, partNb;
    char * pEnd;
	DWORD dwRet;
    HDEVBLK hDevBlk;
    HANDLE hDisk;
    const char* pbuf;

	offset = 0;

    pbuf = pathname;
	if (S_ISBLK(st_mode))
	{
		ulLen = strlcpy(szPath, pathname, MAX_PATH);

		if (!strncmp(szPath, "/dev/sd", 7))
		{
			if ((ulLen >= sizeof("/dev/sdx") - 1) && isalpha(pathname[7]))
			{
                if (pathname[7] >= 'a' && pathname[7] <= 'z')
                {                
                    devNb = pathname[7] - 'a';
                    partNb = strtol(pathname + 8, &pEnd, 10);
                    hDevBlk = DevBlkOpen(devNb, partNb, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE);
                    if (hDevBlk)
                    {
                        hDisk = DevBlkGetDiskHandle(hDevBlk);
                        fd = _open_osfhandle((intptr_t)hDisk, 0);
                    }

                    
                }				
			}
		}
	}
	else
	{
        if (pathname && pathname[0]=='/')
        {
            snprintf(szPath, MAX_PATH, "%s%s", "C:/ProgramData/Smartmobili/Posix4Win", pathname);
            pbuf = szPath;
        }

		fd = _open(pbuf, flags, mode);
        int err = errno;
        if (err < 0)
        {
            printf("%d", err);
        }
	}

	return fd;
}


int __cdecl close(int fd)
{
    int ret;
    HDEVBLK hDevBlk;
   
    hDevBlk = DevBlkFromDiskHandle((HANDLE)_get_osfhandle(fd));
    if (hDevBlk)
    {
        ret = devblk_close(fd);
    }
    else
    {
        ret = _close(fd);
    }
}

int __cdecl 
sys_stat(const char *path, struct stat *buf)
{
	// First we check pathname is valid
	if (!path || path[0] == '\0') {
		_set_errno(ENOENT);
		return -1;
	}

	return -1;
}

//+//int _tstati64(const TCHAR* path, struct _stati64 * buf)
//+//{
//+//  int ret;
//+//  struct __stat64 buf64;
//+//
//+//  ret = _tstat64(path, &buf64);
//+//  if (!ret)
//+//    stat64_to_stati64(&buf64, buf);
//+//  return ret;
//+//}
//+
//+#endif
//+
//+HANDLE fdtoh(int fd); //file.c
//+
//+#if _USE_STAT64
//+ int CDECL _tstat64(const _TCHAR *path, struct _stat64 *buf)
//+ #else
//+ int CDECL _tstat64i32(const _TCHAR *path, struct _stat64i32 *buf)
//+ #endif
//+ {
//    +DWORD dw;
//    +WIN32_FILE_ATTRIBUTE_DATA hfi;
//    +unsigned short mode = ALL_S_IREAD;
//    +int plen;
//    +
//        +TRACE(":file (%s) buf(%p)\n", path, buf);
//    +
//        +if (!GetFileAttributesEx(path, GetFileExInfoStandard, &hfi))
//        + {
//        +TRACE("failed (%d)\n", GetLastError());
//        +__set_errno(ERROR_FILE_NOT_FOUND);
//        +return -1;
//        +}
//    +
//        +memset(buf, 0, sizeof(struct __stat64));
//    +
//        +  /* FIXME: rdev isn't drive num, despite what the docs say-what is it?
//           +     Bon 011120: This FIXME seems incorrect
//           +                 Also a letter as first char isn't enough to be classified
//           +		 as a drive letter
//           +  */
//        +#ifndef _UNICODE
//        + if (isalpha(*path) && (*(path + 1) == ':'))
//        + buf->st_dev = buf->st_rdev = toupper(*path) - 'A'; /* drive num */
//    +#else
//        + if (iswalpha(*path))
//        + buf->st_dev = buf->st_rdev = toupperW(*path - 'A'); /* drive num */
//    +#endif
//        + else
//        + buf->st_dev = buf->st_rdev = _getdrive() - 1;
//    +
//        +#ifndef _UNICODE
//        + plen = strlen(path);
//    +#else
//        + plen = strlenW(path);
//    +#endif
//        +
//        +  /* Dir, or regular file? */
//        +if ((hfi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ||
//            +(path[plen - 1] == '\\'))
//        + mode |= (_S_IFDIR | ALL_S_IEXEC);
//    +else
//        + {
//        +mode |= _S_IFREG;
//        +    /* executable? */
//            +if (plen > 6 && path[plen - 4] == '.')  /* shortest exe: "\x.exe" */
//            + {
//            +#ifndef _UNICODE
//                + unsigned int ext = tolower(path[plen - 1]) | (tolower(path[plen - 2]) << 8) |
//                +(tolower(path[plen - 3]) << 16);
//            +if (ext == EXE || ext == BAT || ext == CMD || ext == COM)
//                + mode |= ALL_S_IEXEC;
//            +#else
//                + ULONGLONG ext = tolowerW(path[plen - 1]) | (tolowerW(path[plen - 2]) << 16) |
//                +((ULONGLONG)tolowerW(path[plen - 3]) << 32);
//            +if (ext == WCEXE || ext == WCBAT || ext == WCCMD || ext == WCCOM)
//                + mode |= ALL_S_IEXEC;
//            +#endif
//                + }
//        +}
//    +
//        +if (!(hfi.dwFileAttributes & FILE_ATTRIBUTE_READONLY))
//        + mode |= ALL_S_IWRITE;
//    +
//        +buf->st_mode = mode;
//    +buf->st_nlink = 1;
//    +#if _USE_STAT64
//        + buf->st_size = buf->st_size = ((__int64)hfi.nFileSizeHigh << 32) + hfi.nFileSizeLow;
//    +#else
//        + buf->st_size = hfi.nFileSizeLow;
//    +#endif
//        + RtlTimeToSecondsSince1970((LARGE_INTEGER *)&hfi.ftLastAccessTime, &dw);
//    +buf->st_atime = dw;
//    +RtlTimeToSecondsSince1970((LARGE_INTEGER *)&hfi.ftLastWriteTime, &dw);
//    +buf->st_mtime = buf->st_ctime = dw;
//    +TRACE("%d %d 0x%08lx%08lx %ld %ld %ld\n", buf->st_mode, buf->st_nlink,
//        +(long)(buf->st_size >> 16), (long)buf->st_size,
//        +(long)buf->st_atime, (long)buf->st_mtime, (long)buf->st_ctime);
//    +return 0;
//    +}

int __cdecl
stat(const char *path, struct stat *buf)
{
	int ret;
    int fd;

    fd = open(path, O_RDONLY);
    if (fd < 1)
        return fd;

    ret = fstat(fd, buf);

    close(fd);
	
    return ret;
}



int __cdecl 
fstat(int fd, struct stat *buf)
{
	int ret;
	HANDLE handle;

	ret = -1;

	/* first check this fd is not backed up by a handle */
	handle = (HANDLE)_get_osfhandle(fd);
	if (DevBlkFromDiskHandle(handle))
	{
        ret = devblk_fstat(fd, buf);
	}
	else
	{
		ret = os_fstat(fd, (struct os_stat *)buf);
	}
	
	return ret;
}



int __cdecl
fstatat(int dirfd, const char *pathname, struct stat *buf, int flags)
{
	int fd = -1;
	HANDLE hDir = INVALID_HANDLE_VALUE;
	BOOL bRet = FALSE;
	DWORD dwRet = 0;
	char szBuffer[MAX_PATH + 1];
	size_t ulLen = 0;
	const char* szNormPath = NULL;

	// First we check pathname is valid
	if (!pathname || pathname[0] == '\0') {
		_set_errno(ENOENT);
		return -1;
	}

	// If pathname is not relative => ignore dirfd 
	p4msvc_norm_path_t norm_path;
	szNormPath = p4msvc_norm_path(pathname, &norm_path, TRUE);
	if (norm_path.isAbsolute) {
		return stat(szNormPath, buf);
	}

	// If we receive a file descriptor first check 
	// we can get a native handle from it
	if (dirfd != AT_FDCWD) {
		hDir = (HANDLE)_get_osfhandle(dirfd);
		if (hDir == INVALID_HANDLE_VALUE) {
			_set_errno(EBADF);
			return -1;
		}
	}
	// Check if the native handle corresponds to a directory
	BY_HANDLE_FILE_INFORMATION info;
	memset(&info, 0, sizeof(BY_HANDLE_FILE_INFORMATION));
	bRet = GetFileInformationByHandle(hDir, &info);
	if (!bRet || (FILE_ATTRIBUTE_DIRECTORY != info.dwFileAttributes)) {
		_set_errno(ENOTDIR);
		return -1;
	}

	memset(szBuffer, 0, sizeof(szBuffer) / sizeof(szBuffer[0]));
	if (dirfd == AT_FDCWD) {
		_getcwd(szBuffer, MAX_PATH);
	}
	else {
		dwRet = GetFinalPathNameByHandle(hDir, szBuffer, MAX_PATH, FILE_NAME_NORMALIZED);
		if (dwRet > 4)
		{
			if (strncmp(szBuffer, "\\\\?\\", strlen("\\\\?\\")) == 0) {
				memmove(szBuffer, szBuffer + 4, strlen(szBuffer + 4) + 1);
			}
		}
	}
	//check we have something in our buffer
	ulLen = strlen(szBuffer);
	if (ulLen == 0) {
		_set_errno(EINVAL);
		return -1;
	}

	//Protect us from buffer overflow (strlcat)
	strlcat(szBuffer, "\\", MAX_PATH);
	strlcat(szBuffer, pathname, MAX_PATH);

	fd = stat(szBuffer, buf);

	return fd;
}

int __cdecl 
faccessat (int dirfd, const char *pathname, int mode, int flags)
{
	return 0;
}

int __cdecl
unlinkat(int dirfd, const char *pathname, int flags)
{
	int fd = -1;
	HANDLE hDir = INVALID_HANDLE_VALUE;
	BOOL bRet = FALSE;
	DWORD dwRet = 0;
	char szBuffer[MAX_PATH + 1];
	size_t ulLen = 0;
	const char* szNormPath = NULL;

	// First we check pathname is valid
	if (!pathname || pathname[0] == '\0') {
		_set_errno(ENOTDIR);
		return -1;
	}

	// If pathname is not relative => ignore dirfd 
	p4msvc_norm_path_t norm_path;
	szNormPath = p4msvc_norm_path(pathname, &norm_path, TRUE);
	if (norm_path.isAbsolute) {
		if (flags & AT_REMOVEDIR)
			return _rmdir(pathname);
		else
			return _unlink(pathname);
	}

	// If we receive a file descriptor first check 
	// we can get a native handle from it
	if (dirfd != AT_FDCWD) {
		hDir = (HANDLE)_get_osfhandle(dirfd);
		if (hDir == INVALID_HANDLE_VALUE) {
			_set_errno(EBADF);
			return -1;
		}
	}
	// Check if the native handle corresponds to a directory
	BY_HANDLE_FILE_INFORMATION info;
	memset(&info, 0, sizeof(BY_HANDLE_FILE_INFORMATION));
	bRet = GetFileInformationByHandle(hDir, &info);
	if (!bRet || (FILE_ATTRIBUTE_DIRECTORY != info.dwFileAttributes)) {
		_set_errno(ENOTDIR);
		return -1;
	}

	memset(szBuffer, 0, sizeof(szBuffer) / sizeof(szBuffer[0]));
	if (dirfd == AT_FDCWD) {
		_getcwd(szBuffer, MAX_PATH);
	}
	else {
		dwRet = GetFinalPathNameByHandle(hDir, szBuffer, MAX_PATH, FILE_NAME_NORMALIZED);
		if (dwRet > 4)
		{
			if (strncmp(szBuffer, "\\\\?\\", strlen("\\\\?\\")) == 0) {
				memmove(szBuffer, szBuffer + 4, strlen(szBuffer + 4) + 1);
			}
		}
	}
	//check we have something in our buffer
	ulLen = strlen(szBuffer);
	if (ulLen == 0) {
		_set_errno(EINVAL);
		return -1;
	}

	//Protect us from buffer overflow (strlcat)
	strlcat(szBuffer, "\\", MAX_PATH);
	strlcat(szBuffer, pathname, MAX_PATH);

	if (flags & AT_REMOVEDIR)
		fd = _rmdir(szBuffer);
	else
		fd = _unlink(szBuffer);

	return fd;
}

