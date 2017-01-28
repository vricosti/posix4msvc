#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "stdio.h"

#include "easyhook.h"


void init_hooking(void);
void cleanup_hooking(void);
static HOOK_TRACE_INFO g_hHook;

// Some useful macros
#define PPCAT_NX(A, B) A ## B
#define PPCAT(A, B) PPCAT_NX(A, B)
#define STRINGIZE_NX(A) #A
#define STRINGIZE(A) STRINGIZE_NX(A)

#ifndef ARRAY_SIZE
# define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#ifdef _DEBUG
#define REL_OR_DBG_SUFFIX d
#else
#define REL_OR_DBG_SUFFIX
#endif
#define DEBUG_OR_RELEASE(x) STRINGIZE(PPCAT(T1, REL_OR_DBG_SUFFIX))

typedef struct _HOOKING_INFO
{
    TCHAR ModuleName[MAX_PATH];
    TCHAR FuncName[MAX_PATH];
    void* InHookProc;
} HOOKING_INFO, *PHOOKING_INFO;

static HOOKING_INFO g_HookingInfo[] =
{
    { TEXT("ucrtbased"), TEXT("fopen"), posix_fopen },
};



void init_hooking(void)
{
    NTSTATUS result;
    ULONG ACLEntries[1] = { 0 };
    ZeroMemory(&g_hHook, sizeof(HOOK_TRACE_INFO));

#ifndef LHINSTALLHOOK
# define LHINSTALLHOOK(a,b,c) (LhInstallHook(GetProcAddress(GetModuleHandle(a), b), c, NULL, &g_hHook))
#endif

    for (size_t i = 0; i < ARRAY_SIZE(g_HookingInfo); i++)
    {
        PHOOKING_INFO pHookInfo = &g_HookingInfo[i];
        result = LHINSTALLHOOK(pHookInfo->ModuleName, pHookInfo->FuncName, pHookInfo->InHookProc);
        if (FAILED(result))
            break;
    }

    if (SUCCEEDED(result))
    {
        LhSetInclusiveACL(ACLEntries, 1, &g_hHook);
    }

#undef LHINSTALLHOOK
}

void cleanup_hooking(void)
{
    if (g_hHook.Link)
    {
        LhUninstallHook(&g_hHook);
        LhWaitForPendingRemovals();
    }
}