
#ifndef _P4MSVC_STDIO_H_
#define _P4MSVC_STDIO_H_

#include_next <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

FILE *__cdecl posix_fopen(const char *path, const char *mode);

//FILE *fdopen(int fd, const char *mode);

//FILE *freopen(const char *path, const char *mode, FILE *stream);


#ifdef  __cplusplus
}
#endif


#endif /*_P4MSVC_STDIO_H_*/