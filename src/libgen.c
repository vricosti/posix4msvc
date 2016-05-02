#include <string.h>
#include "libgen.h"

#if (defined _WIN32 || defined __WIN32__) && ! defined __CYGWIN__

static void splitpath(char *path, char **dir, char **base) {
    char *startpath, *endpath;
    int len;
    if(!path || !(len = strlen(path))) {
	*dir = ".";
	*base = "";
	return;
    }

    endpath = &path[len-1];
    while(endpath >= path && *endpath == '\\') {
	*endpath = '\0';
	endpath--;
    }
    if(endpath < path) {
	*dir = "";
	*base = "";
	return;
    }
    len = endpath-path + 1;
    if(len > 2 && path[0] == '\\' && path[1] == '\\') {
	if(len > 4 && (path[2] == '.' || path[2] == '?') && path[3] == '\\')
	    startpath = strchr(path + 4, '\\');
	else 
	    startpath = strchr(path + 2, '\\');

	if(!startpath) {
	    *dir = path;
	    *base = "";
	    return;
	}
	startpath ++;
    } else startpath = path;
    endpath = strrchr(startpath, '\\');
    if(!endpath) {
        if(startpath == path) {
	    if(!strcmp(path, ".") || !strcmp(path, "..") || (len >= 2 && ((*path >= 'a' && *path <= 'z') || (*path >= 'A' && *path <= 'Z')) && path[1] == ':')) {
		*dir = path;
		*base = "";
		return;
	    }
	    *dir = ".";
	    *base = path;
	    return;
	}
	*base = startpath;
	endpath = startpath - 1;
	startpath = path;
    } else {
	*base = endpath + 1;
    }
    *dir = path;

    while(endpath >= startpath && *endpath == '\\') {
	    *endpath = '\0';
	    endpath--;
    }
}

char *__cdecl
basename(char *path)
{
	char *dir, *base;
	splitpath(path, &dir, &base);
	return base;
}

char *__cdecl
dirname (char *path) 
{
    char *dir, *base;
    splitpath(path, &dir, &base);
    return dir;
}






#endif