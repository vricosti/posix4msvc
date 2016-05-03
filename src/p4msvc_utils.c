#include <p4msvc_utils.h>
#include <bsd/string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

const char* __cdecl
p4msvc_norm_path(const char* pathname, p4msvc_norm_path_t* pPath, int normalize)
{
	size_t ulLen = 0;
	const char* szRet = NULL;
	char str[3];

	if (!pathname || !pPath)
		return NULL;

	szRet = pathname;
	pPath->isAbsolute = FALSE;
	pPath->isNormalized = FALSE;
	pPath->szNormalizedPath[0] = '\0';
	pPath->st_mode = S_IFREG;

	ulLen = strlen(pathname);
	if (ulLen > 3)
	{
		/* handle c:\foo.txt */
		if (((isalpha(pathname[0]) && pathname[1] == ':')) &&
			((pathname[2] == '\\') || (pathname[2] == '/')))
		{
			pPath->isAbsolute = TRUE;
		}
		else if ((!strncmp(pathname, "\\\\.\\", 4) || (!strncmp(pathname, "\\\\?\\", 4))))
		{
			pPath->isAbsolute = TRUE;
			int nPrefixLen = sizeof("\\\\.\\PhysicalDrive") - 1;
			if ((ulLen > nPrefixLen) &&
				(!strncmp(pathname, "\\\\.\\PhysicalDrive", nPrefixLen)))
			{
				pPath->st_mode = S_IFBLK;
			}
		}
		else if (!strncmp(pathname, "/dev/sd", 7))
		{
			pPath->isAbsolute = TRUE;
			if ((ulLen == sizeof("/dev/sdx") - 1) && isalpha(pathname[7]))
			{
				strlcpy(pPath->szNormalizedPath, "\\\\.\\PhysicalDrive", MAX_PATH);
				snprintf(str, sizeof(str), "%d", (toupper(pathname[7]) - 'A'));
				strlcat(pPath->szNormalizedPath, str, MAX_PATH);
				pPath->st_mode = S_IFBLK;
				pPath->isNormalized = TRUE;
				szRet = pPath->szNormalizedPath;
			}
		}
		/* handle /c/foo.txt and => c:/foo.txt */
		else if ((pathname[0] == '/') && (isalpha(pathname[1])) &&
			(pathname[2] == '/'))
		{
			pPath->isAbsolute = TRUE;

			if (normalize)
			{
				strlcpy(pPath->szNormalizedPath, pathname, MAX_PATH);
				pPath->szNormalizedPath[0] = pathname[1];
				pPath->szNormalizedPath[1] = ':';
				pPath->szNormalizedPath[2] = '/';
				pPath->isNormalized = TRUE;
				szRet = pPath->szNormalizedPath;
			}
		}
		else if (ulLen >= (sizeof("/cygdrive/c") - 1))
		{
			pPath->isAbsolute = TRUE;
			//TODO: for now don't handle cygwin paths
		}

	}
	return szRet;
}
