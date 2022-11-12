#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include <get_osfhandle-nothrow.h>
#include <p4msvc_utils.h>

int __cdecl
symlink(const char *oldpath, const char *newpath)
{
    int res = -1;
    struct stat st_oldpath, st_newpath;

    if (strlen(oldpath) >= MAX_PATH || strlen(newpath) >= MAX_PATH)
    {
        _set_errno(ENAMETOOLONG);
        goto done;
    }

    /* As a windows limitation, the source path must exist. */
    if (stat(oldpath, &st_oldpath))
    {
        goto done;
    }

    /* As per POSIX, the destination must not exist */
    if (stat(newpath, &st_newpath))
    {
        if (errno != ENOENT)
        {
            //debug_printf("error: %d", errno);
            goto done;
        }
        else
            _set_errno(0);
    }
    else
    {
        _set_errno(EEXIST);
        goto done;
    }

done:
    return res;
}