#ifndef _GET_OSFHANDLE_NOTHROW_H
#define _GET_OSFHANDLE_NOTHROW_H

#if (defined _WIN32 || defined __WIN32__) && ! defined __CYGWIN__

#include <io.h>


#ifdef __cplusplus
extern "C" {
#endif

extern intptr_t _get_osfhandle_nothrow(int fd);
#define _get_osfhandle _get_osfhandle_nothrow


#ifdef __cplusplus
}
#endif

#endif /* _WIN32 && ! __CYGWIN__ */

#endif /* _GET_OSFHANDLE_NOTHROW_H */