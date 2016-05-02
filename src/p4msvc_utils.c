#include <p4msvc_utils.h>
#include <bsd/string.h>

const char* __cdecl
p4msvc_norm_path(const char* pathname, p4msvc_norm_path_t* pPath, int normalize)
{
	size_t ulLen = 0;
	const char* szRet = NULL;

	if (!pathname || !pPath)
		return NULL;

	szRet = pathname;
	pPath->isAbsolute = FALSE;
	pPath->isNormalized = FALSE;
	pPath->szNormalizedPath[0] = '\0';


	ulLen = strlen(pathname);
	if (ulLen > 3)
	{
		if (((isalpha(pathname[0]) && pathname[1] == ':')) &&
			((pathname[2] == '\\') || (pathname[2] == '/')))
		{
			pPath->isAbsolute = TRUE;
		}
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
