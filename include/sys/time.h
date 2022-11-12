/*
 * TIME.H
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 */

#ifndef _SYS_TIME_H_
#define _SYS_TIME_H_

/* All the headers include this file. */
//#include <sys/cdefs.h>
#include <sys/timeb.h>
#include <sys/types.h>

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
#ifndef _WINSOCKAPI_
    typedef struct timeval {
        long    tv_sec;         /* seconds */
        long    tv_usec;        /* and microseconds */
    } timeval;
#endif /* !_WINSOCKAPI_ */
#endif /* !_MSC_VER */

    /* Structure crudely representing a timezone.
       This is obsolete and should never be used.  */
    struct timezone
    {
        int tz_minuteswest;		/* Minutes west of GMT.  */
        int tz_dsttime;		/* Nonzero if DST is ever in effect.  */
    };

    int __cdecl gettimeofday(struct timeval* tp, struct timezone* tzp);


#ifdef  __cplusplus
}
#endif



#endif	/* Not _SYS_TIME_H_ */

