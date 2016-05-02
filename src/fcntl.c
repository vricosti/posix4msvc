/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */

#define __USE_MAXGW

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <direct.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <bsd/string.h> /*strlcat && strlcpy*/
#include <stdlib.h>
#include <errno.h>

#include <get_osfhandle-nothrow.h>
#include <p4msvc_utils.h>








int __cdecl
__openat(int fd, const char *pathname, int flags, mode_t mode);


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
		return _open(szNormPath, flags, mode);
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

	fd = _open(szBuffer, flags, mode);


	return fd;
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

