#ifndef _P4MSVC_TIME_H_
#define _P4MSVC_TIME_H_

#include_next <time.h>
#include <sys/time.h>


#ifdef  __cplusplus
extern "C" {
#endif

struct tm* __cdecl gmtime_r(const time_t* timer, struct tm* buf);
struct tm *__cdecl localtime_r(const time_t *timep, struct tm *result);


#ifdef  __cplusplus
}
#endif


#endif /*_P4MSVC_TIME_H_*/