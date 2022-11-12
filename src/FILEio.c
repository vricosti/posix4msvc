#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "fcntl.h"
#include "stdio.h"
#include "sys/stat_ex.h"
#include <errno.h>

#include <get_osfhandle-nothrow.h>

extern int	__sflags(const char *, int *);

int __cdecl 
getc_unlocked(FILE* stream)
{
    return _fgetc_nolock(stream);
}

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

// The following two functions were taken from mingw_lock.c

_CRTIMP void __cdecl _lock(int locknum);
_CRTIMP void __cdecl _unlock(int locknum);
#define _STREAM_LOCKS 16
#define _IOLOCKED 0x8000
typedef struct {
    FILE f;
    CRITICAL_SECTION lock;
} _FILEX;

void __cdecl 
flockfile(FILE* F)
{
    _lock_file(F);
    /*if ((F >= (&__iob_func()[0])) && (F <= (&__iob_func()[_IOB_ENTRIES - 1]))) {
        _lock(_STREAM_LOCKS + (int)(F - (&__iob_func()[0])));
        F->_flag |= _IOLOCKED;
    }
    else
        EnterCriticalSection(&(((_FILEX*)F)->lock));*/
}

void __cdecl 
funlockfile(FILE* F)
{
    _unlock_file(F);
    /*if ((F >= (&__iob_func()[0])) && (F <= (&__iob_func()[_IOB_ENTRIES - 1]))) {
        F->_flag &= ~_IOLOCKED;
        _unlock(_STREAM_LOCKS + (int)(F - (&__iob_func()[0])));
    }
    else
        LeaveCriticalSection(&(((_FILEX*)F)->lock));*/
}



int __cdecl 
ftrylockfile(FILE* filehandle)
{

}

