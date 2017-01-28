
#ifndef _P4MSVC_STDIO_H_
#define _P4MSVC_STDIO_H_

#include_next <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define	__SLBF	0x0001		/* line buffered */
#define	__SNBF	0x0002		/* unbuffered */
#define	__SRD	0x0004		/* OK to read */
#define	__SWR	0x0008		/* OK to write */
    /* RD and WR are never simultaneously asserted */
#define	__SRW	0x0010		/* open for reading & writing */
#define	__SEOF	0x0020		/* found EOF */
#define	__SERR	0x0040		/* found error */
#define	__SMBF	0x0080		/* _buf is from malloc */
#define	__SAPP	0x0100		/* fdopen()ed in append mode */
#define	__SSTR	0x0200		/* this is an sprintf/snprintf string */
#define	__SOPT	0x0400		/* do fseek() optimization */
#define	__SNPT	0x0800		/* do not do fseek() optimization */
#define	__SOFF	0x1000		/* set iff _offset is in fact correct */
#define	__SMOD	0x2000		/* true => fgetln modified _p text */
#define	__SALC	0x4000		/* allocate string space dynamically */
#define	__SIGN	0x8000		/* ignore this file in _fwalk */


FILE *__cdecl posix_fopen(const char *path, const char *mode);

//FILE *__cdecl posix_fdopen(int fd, const char *mode);

//FILE *__cdecl freopen(const char *path, const char *mode, FILE *stream);


#ifdef  __cplusplus
}
#endif


#endif /*_P4MSVC_STDIO_H_*/