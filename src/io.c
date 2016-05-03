#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sys/types_ex.h>

#include <get_osfhandle-nothrow.h>


long _cdecl 
lseek(int fd, long offset, int origin)
{
	long lret;
	HANDLE handle;

	lret = -1L;

	handle = (HANDLE)_get_osfhandle(fd);
	if (handle != INVALID_HANDLE_VALUE)
	{
		lret = SetFilePointer(handle, offset, (void*)0, origin);
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
		liOffset.QuadPart = offset;
		ret = SetFilePointerEx(handle, liOffset, &liNewOffset, whence);
		lret = liNewOffset.QuadPart;
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

	handle = (HANDLE)_get_osfhandle(fd);
	if (handle != INVALID_HANDLE_VALUE)
	{
		size = -1;
		if (ReadFile(handle, buf, count, &dwReadLen, NULL))
			size = dwReadLen;
		else
		{
			dwErr = GetLastError();
		}
	}
	else
	{
		size = _read(fd, buf, count);
	}

	return size;
}
