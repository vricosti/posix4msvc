#include <unistd.h>

#if (defined _WIN32 || defined __WIN32__) && ! defined __CYGWIN__

# define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <errno.h>
#include "get_osfhandle-nothrow.h"


ssize_t 
readlink (const char *path, char *buf, size_t bufsiz)
{
	return readlinkat(-1, path, buf, bufsiz);
}


int __cdecl 
readlinkat (int dirfd, const char *pathname, char *buf, size_t bufsiz)
{
	return 0;
}


#endif