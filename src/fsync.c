#include <unistd.h>

#if (defined _WIN32 || defined __WIN32__) && ! defined __CYGWIN__

# define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <errno.h>
#include "get_osfhandle-nothrow.h"


int __cdecl 
fdatasync (int fd)
{
	return fsync(fd);
}

int __cdecl
fsync (int fd)
{
	DWORD err;
	HANDLE h;

	h = (HANDLE)_get_osfhandle(fd);
	if (h == INVALID_HANDLE_VALUE)
	{
		_set_errno(EBADF);
		return -1;
	}

	if (!FlushFileBuffers(h))
	{
		err = GetLastError();
		switch (err)
		{
		case ERROR_ACCESS_DENIED:
			return 0;

		case ERROR_INVALID_HANDLE:
			_set_errno(EINVAL);
			break;

		default:
			_set_errno(EIO);
		}
		return -1;
	}

	return 0;
}

#endif