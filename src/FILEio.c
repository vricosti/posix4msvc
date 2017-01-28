#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "fcntl.h"
#include "stdio.h"
#include <errno.h>

#include <get_osfhandle-nothrow.h>

extern int	__sflags(const char *, int *);

FILE *__cdecl 
posix_fopen(const char *path, const char *mode)
{
    FILE *fp;
    int f;
    int flags, oflags;
    HANDLE h;

    if ((flags = __sflags(mode, &oflags)) == 0)
        return (NULL);
    if ((f = open(path, oflags, DEFFILEMODE)) < 0) {
        return (NULL);
    }
   
    /*
     * When opening in append mode, even though we use O_APPEND,
     * we need to seek to the end so that ftell() gets the right
     * answer.  If the user then alters the seek pointer, or
     * the file extends, this will fail, but there is not much
     * we can do about this.  (We could set __SAPP and check in
     * fseek and ftell.)
     */
    if (oflags & O_APPEND)
        lseek(f, (fpos_t)0, SEEK_END);

    if ((fp = _fdopen(f, mode)) == NULL)
    {
        close(f);
        return (NULL);
    }

    return (fp);
}
