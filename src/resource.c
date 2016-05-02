#include <sys/time.h>
#include <sys/resource.h>

#if (defined _WIN32 || defined __WIN32__) && ! defined __CYGWIN__
	  
int	__cdecl 
getrusage (int who, struct rusage *usage)
{
	return -1;
}

int __cdecl 
getpriority (int which, int who)
{
	return -1;
}


int	__cdecl 
setpriority (int which, int who, int prio)
{
	return -1;
}

int	__cdecl 
getrlimit (int resource, struct rlimit *rlim)
{
	return 0;
}


int	__cdecl 
setrlimit (int resource, const struct rlimit *rlim)
{
	return 0;
}


#endif