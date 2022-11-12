#if BUILD_WINDEVBLK
#define __USE_MAXGW

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winioctl.h>
#include <sys/ioctl.h>

#include <get_osfhandle-nothrow.h>

#include "windevblk.h"




int __cdecl 
ioctl(int fd, unsigned long request, ...)
{
	int ret;
	HANDLE handle;
	BOOL bRet;
	LARGE_INTEGER liSize;
	unsigned long count;
	void* pbuffer;

	ret = -1;
	handle = (HANDLE)_get_osfhandle(fd);
	if (handle == INVALID_HANDLE_VALUE)
	{
		return -1;
	}


	switch (request) {
	case BLKGETSIZE64:
        if (FILE_TYPE_DISK == GetFileType(handle))
        {
            HDEVBLK hDevBlk = DevBlkFromDiskHandle(handle);
            if (hDevBlk)
            {
                bRet = DevBlkGetFileSizeEx(hDevBlk, &liSize);
                if (bRet)
                {
                    va_list args;
                    va_start(args, request);
                    pbuffer = va_arg(args, unsigned long long *);
                    (*(unsigned long long *)pbuffer) = liSize.QuadPart;
                    va_end(args);

                    ret = 0;
                }
            }
        }
		break;

	default:
		break;
	}

	
	



	return ret;
}
#endif