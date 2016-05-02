#include <sys/stat.h>

#if (defined _WIN32 || defined __WIN32__) && ! defined __CYGWIN__

#include <unistd.h>
/* Microsoft headers already define chmod (io.h) when !__STDC__ */
#if __STDC__

int __cdecl 
chmod (const char *path, mode_t mode)
{
	if (!path || path[0]==0)
		return -1;
	
	return 0;
}

#endif /*__STDC__*/

int __cdecl 
fchmod (int fd, mode_t mode)
{
	return 0;
}




#endif

