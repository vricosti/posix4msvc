#define __USE_MAXGW

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winioctl.h>
#include <sys/ioctl.h>

#include <get_osfhandle-nothrow.h>





int __cdecl 
ioctl(int fd, unsigned long request, ...)
{
	int ret;
	HANDLE handle;
	BOOL bRet;
	DISK_GEOMETRY_EX disk_geom;
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
		bRet = DeviceIoControl(handle,
			IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,
			NULL, 0,
			&disk_geom, sizeof(DISK_GEOMETRY_EX),
			&count,
			(LPOVERLAPPED)NULL);
		if (bRet)
		{
			va_list args;
			va_start(args, request);
			pbuffer = va_arg(args, unsigned long long *);
			(*(unsigned long long *)pbuffer) = disk_geom.DiskSize.QuadPart;
			va_end(args);

			ret = 0;
		}
		break;

	default:
		break;
	}

	
	



	return ret;
}
