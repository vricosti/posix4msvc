#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sys/types_ex.h>
#include <io.h>
#include <stdlib.h>
#include <errno.h>

#include <get_osfhandle-nothrow.h>
#include "windevblk.h"


int __cdecl 
link(const char *oldpath, const char *newpath)
{
    return 0;
}

int __cdecl 
access(const char *pathname, int mode)
{
    return _access(pathname, mode);
}

char __cdecl
*realpath(const char *name, char *resolved)
{
    char *retname = NULL;  

    if (name == NULL)
        errno = EINVAL;

    else if (access(name, 4) == 0)
    {
        if ((retname = resolved) == NULL)
        {
            retname = malloc(MAX_PATH);
        }

        if (retname == NULL)
            errno = ENOMEM;
        else if ((retname = _fullpath(retname, name, MAX_PATH)) == NULL)
            errno = ENAMETOOLONG;
    }

    return retname;
}

long _cdecl 
lseek(int fd, long offset, int origin)
{
	long lret;
	HANDLE handle;
    LARGE_INTEGER liOffset, liNewOffset;

	lret = -1L;

	handle = (HANDLE)_get_osfhandle(fd);
	if (handle != INVALID_HANDLE_VALUE)
	{
        HDEVBLK hDevBlk = DevBlkFromDiskHandle(handle);
        if (hDevBlk)
        {
            liOffset.QuadPart = offset;
            if (DevBlkSetPointerEx(handle, liOffset, &liNewOffset, origin))
            {
                lret = liNewOffset.LowPart;
            }
        }
        else
        {
            lret = SetFilePointer(handle, offset, (void*)0, origin);
        }
	}
	else
	{
		lret = _lseek(fd, offset, origin);
	}
	return lret;
}

off64_t __cdecl  
lseek64(int fd, off64_t offset, int whence)
{
	off64_t lret;
	HANDLE handle;
	LARGE_INTEGER liOffset, liNewOffset;
	unsigned int ret;

	lret = -1L;

	handle = (HANDLE)_get_osfhandle(fd);
	if (handle != INVALID_HANDLE_VALUE)
	{	
        HDEVBLK hDevBlk = DevBlkFromDiskHandle(handle);
        if (hDevBlk)
        {
            liOffset.QuadPart = offset;
            if (DevBlkSetPointerEx(hDevBlk, liOffset, &liNewOffset, whence) == TRUE)
            {
                lret = liNewOffset.QuadPart;
            }
        }
        else
        {
            liOffset.QuadPart = offset;
            if (SetFilePointerEx(handle, liOffset, &liNewOffset, whence) == TRUE)
            {
                lret = liNewOffset.QuadPart;
            }
        }
	}
	else
	{
		lret = _lseeki64(fd, offset, whence);
	}
	return lret;
}

ssize_t __cdecl 
read(int fd, void *buf, size_t count)
{
	ssize_t size;
	HANDLE handle;
	DWORD dwReadLen, dwErr;

    size = -1;
	handle = (HANDLE)_get_osfhandle(fd);
	if (handle != INVALID_HANDLE_VALUE)
	{
        HDEVBLK hDevBlk = DevBlkFromDiskHandle(handle);
        if (hDevBlk)
        {
            if (DevBlkRead(hDevBlk, buf, count, &dwReadLen))
            {
                size = dwReadLen;
            }
            else
            {
                dwErr = GetLastError();
            }
        }
        else
        {
            if (ReadFile(handle, buf, count, &dwReadLen, NULL))
            {
                size = dwReadLen;
            }
            else
            {
                dwErr = GetLastError();
            }
        }
	}
	else
	{
		size = _read(fd, buf, count);
	}

	return size;
}
