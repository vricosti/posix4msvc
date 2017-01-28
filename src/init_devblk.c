#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Shlobj.h>
#include <Shlwapi.h>
#include <Strsafe.h>

#include <sys/file.h>
#include <stdint.h>

#include <malloc.h>
#include "windevblk.h"

void init_devblk(void);
void cleanup_devblk(void);

#define ROOTFS  "\\Smartmobili\\Posix4Win\\"


typedef struct _root_name
{
    CHAR name[MAX_PATH];
} root_name;

root_name g_rootNames[] =
{
    { "proc" },
    { "dev" },
    { "etc" },
};


typedef struct _proc_partitions
{
    UINT major;
    UINT minor;
    ULONG blocks;
    CHAR name[128];
} proc_partitions;

proc_partitions g_ProcPartions[] =  
{
    { 1   ,     0  ,     65536,  "ram0"    },
    { 1   ,     1  ,     65536,  "ram1"    },
    { 1   ,     2  ,     65536,  "ram2"   },
    { 1   ,     3  ,     65536,  "ram3"    },
    { 1   ,     4  ,     65536,  "ram4"    },
    { 1   ,     5  ,     65536,  "ram5"    },
    { 1   ,     6  ,     65536,  "ram6"    },
    { 1   ,     7  ,     65536,  "ram7"    },
    { 1   ,     8  ,     65536,  "ram8"    },
    { 1   ,     9  ,     65536,  "ram9"    },
    { 1   ,    10  ,     65536,  "ram10"   },
    { 1   ,    11  ,     65536,  "ram11"   },
    { 1   ,    12  ,     65536,  "ram12"   },
    { 1   ,    13  ,     65536,  "ram13"   },
    { 1   ,    14  ,     65536,  "ram14"   },
    { 1   ,    15  ,     65536,  "ram15"   },
    { 11  ,     0  ,   1048575,  "sr0"     },
    { 8   ,     0  , 125034840,  "sda"     },
    { 8   ,     1  ,    102400,  "sda1"    },
    { 8   ,     2  ,  95666195,  "sda2"    },
    { 8   ,     3  ,         1,  "sda3"    },
    { 8   ,     5  ,   3142656,  "sda5"    },
    { 8   ,     6  ,  26121216,  "sda6"    },
    { 8   ,    16  ,   2014208,  "sdb"     },
    { 8   ,    17  ,    204800,  "sdb1"    },
    { 8   ,    18  ,     51200,  "sdb2"    },
    { 8   ,    19  ,     51200,  "sdb3"    },
    { 8   ,    20  ,     51200,  "sdb4"    },
    { 8   ,    21  ,     51200,  "sdb5"    },
    { 8   ,    22  ,     51200,  "sdb6"    },
    { 8   ,    23  ,     51200,  "sdb7"    },
};


BOOL FolderExists(LPTSTR pszPath)
{
    BOOL bRet = FALSE;

    DWORD dwAttr = GetFileAttributes(pszPath);
    if (dwAttr != INVALID_FILE_ATTRIBUTES && (dwAttr & FILE_ATTRIBUTE_DIRECTORY))
    {
        bRet = TRUE;
    }
    
    return bRet;
}

BOOL CreateFolderIfNotExists(LPTSTR pszPath)
{
    BOOL bRet = FALSE;

    if (FolderExists(pszPath))
    {
        bRet = TRUE;
    }
    else
    {
        if (SUCCEEDED(SHCreateDirectoryEx(NULL,pszPath, NULL)))
        {
            bRet = TRUE;
        }
    }

    return bRet;
}

BOOL create_rootfs()
{
    BOOL bRet = FALSE;
    TCHAR szPath[MAX_PATH];
    TCHAR szRootPath[MAX_PATH];

    if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szRootPath)))
    {
        PathAppend(szRootPath, TEXT(ROOTFS));
        for (int i = 0; i < _countof(g_rootNames); i++)
        {
            StringCchCopy(szPath, MAX_PATH, szRootPath);
            PathAppend(szPath, g_rootNames[i].name);
            CreateFolderIfNotExists(szPath);
        }

        bRet = TRUE;
    }

    return bRet;
}


void init_devblk(void)
{
    BOOL bRet;
    DWORD dwcbNeeded, dwCount;
    TCHAR szProcPartPath[MAX_PATH];
    TCHAR szBuffer[MAX_PATH];
    TCHAR szBlockName[MAX_PATH];
    int fd, lock;
    int blocks, major, minor;
    FILE* f;

    bRet = create_rootfs();
    if (bRet)
    {
        bRet = DevBlkEnumDevices(NULL, 0, &dwcbNeeded, NULL);
        if (bRet)
        {
            SMI_DEVBLK_INFO* pDriveInfoArray = malloc(dwcbNeeded);
            bRet = DevBlkEnumDevices((LPBYTE)pDriveInfoArray, dwcbNeeded, &dwcbNeeded, &dwCount);
            if (bRet)
            {
                SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szProcPartPath);
                PathAppend(szProcPartPath, TEXT(ROOTFS));
                PathAppend(szProcPartPath, TEXT("proc"));
                PathAppend(szProcPartPath, TEXT("partitions"));

                FILE* f = fopen(szProcPartPath, "w+");
                int lock = flock(_fileno(f), LOCK_SH);

                //fd = open(szProcPartPath, O_CREAT | O_TRUNC);
                //int lock = flock(fd, LOCK_SH);
                if (lock == 0)
                {
                    fprintf(f, "major minor  #blocks  name\n");
                    fprintf(f, "\n");

                    for (int i = 0; i < dwCount; i++)
                    {
                        PSMI_DEVBLK_INFO pEntry = &(pDriveInfoArray[i]);
                        if (pEntry)
                        {
                            major = major(pEntry->RootDev);
                            minor = minor(pEntry->RootDev);
                            blocks = pEntry->PartitionLength.QuadPart / 1024;
                            fprintf(f, "%4d %8d %10d %s\n", major, minor, blocks, pEntry->PosixName);

                            //OutputDebugString("");
                        }

                    }
                    lock = flock(_fileno(f), LOCK_UN);
                    fclose(f);
                }
            }
        }
    }

   
}

void cleanup_devblk(void)
{

}