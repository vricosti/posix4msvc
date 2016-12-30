#ifndef _BSD_STRING_H_
#define	_BSD_STRING_H_

//#include <sys/cdefs.h>
#include_next "string.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t	 strlcat(char *, const char *, size_t);
size_t	 strlcpy(char *, const char *, size_t);

#ifdef __cplusplus
}
#endif

#endif /*_BSD_STRING_H_*/
