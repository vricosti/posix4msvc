#include "time.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <sys/time.h>

struct tm* __cdecl 
gmtime_r(const time_t* timer, struct tm* buf)
{
	gmtime_s(buf, timer);
	return buf;
}

struct tm* __cdecl 
localtime_r(const time_t* timep, struct tm* result)
{
	// Windows signature of localtime_s is a bit different than on linux
	//errno_t localtime_s(struct tm* const tmDest, time_t const* const sourceTime);
	localtime_s(result, timep);
	return (struct tm*)timep;
}

int __cdecl 
gettimeofday(struct timeval* tp, struct timezone* tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970 
    static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    time = ((uint64_t)file_time.dwLowDateTime);
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec = (long)((time - EPOCH) / 10000000L);
    tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
    return 0;
}
