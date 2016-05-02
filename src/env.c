#include <stdlib_ex.h>


int __cdecl
setenv (const char *name, const char *value, int overwrite)
{
    int errcode = 0;
    if(!overwrite) {
        size_t envsize = 0;
        errcode = getenv_s(&envsize, NULL, 0, name);
        if(errcode || envsize) return errcode;
    }
    return _putenv_s(name, value);
}

int __cdecl 
unsetenv (const char *name)
{
	return _putenv_s(name, "");
}