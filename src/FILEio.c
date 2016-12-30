#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "stdio.h"
#include <errno.h>

#include <get_osfhandle-nothrow.h>
#include "windevblk.h"


FILE *__cdecl 
posix_fopen(const char *path, const char *mode)
{
    return fopen(path, mode);
}