#include <get_osfhandle-nothrow.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <crtdbg.h>

#undef _get_osfhandle


static int msvc_nothrow_initialized  = 0;


void __cdecl
my_osfhandle_invalid_handler(
const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t);

void __cdecl
msvc_install_handler_once(void);

void __cdecl
my_osfhandle_invalid_handler(const wchar_t* expression,
	const wchar_t* function,
	const wchar_t* file,
	unsigned int line,
	uintptr_t pReserved)
{
	UNREFERENCED_PARAMETER(expression);
	UNREFERENCED_PARAMETER(function);
	UNREFERENCED_PARAMETER(file);
	UNREFERENCED_PARAMETER(line);
	UNREFERENCED_PARAMETER(pReserved);
	return;
}


void __cdecl
msvc_install_handler_once (void)
 {
    if (msvc_nothrow_initialized == 0)
      {
        _set_invalid_parameter_handler(my_osfhandle_invalid_handler);
		_CrtSetReportMode(_CRT_ASSERT, 0);
        msvc_nothrow_initialized = 1;
      }
}
 
intptr_t
_get_osfhandle_nothrow (int fd)
{
  intptr_t handle;

  msvc_install_handler_once();
  handle = _get_osfhandle(fd);

  return handle;
}