#ifndef _P4MSVC_UTILS_H_
#define _P4MSVC_UTILS_H_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif

void __cdecl
my_osfhandle_invalid_handler(const wchar_t*, const wchar_t*, const wchar_t*,
	unsigned int, uintptr_t);
	
	
typedef struct p4msvc_norm_path_s
{
	BOOL  isAbsolute;
	BOOL  isNormalized;
	unsigned short st_mode;
	char  szNormalizedPath[4096];

} p4msvc_norm_path_t;

const char*  __cdecl
p4msvc_norm_path(const char* pathname, p4msvc_norm_path_t* pPath, int normalize);


#ifdef	__cplusplus
}
#endif


#endif /*_P4MSVC_UTILS_H_*/