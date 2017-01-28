#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "stdio.h"

static BOOL g_isInited = FALSE;
static CRITICAL_SECTION g_critSec;

void init(void);
void cleanup(void);

void init_devblk(void);
void cleanup_devblk(void);

void init(void)
{
    InitializeCriticalSection(&g_critSec);

    if(!g_isInited)
    {
        //init_hooking();
        init_devblk();
        g_isInited = TRUE;
    }
    DeleteCriticalSection(&g_critSec);
}

void cleanup(void)
{
    InitializeCriticalSection(&g_critSec);

    if (g_isInited)
    {
        //cleanup_hooking();
        cleanup_devblk();
        g_isInited = FALSE;
    }

    DeleteCriticalSection(&g_critSec);
}

