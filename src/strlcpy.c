#include <bsd/string.h>

#if !HAVE_STRLCPY
/*
 * Copy string src to buffer dst of size dsize.  At most dsize-1
 * chars will be copied.  Always NUL terminates (unless dsize == 0).
 * Returns strlen(src); if retval >= dsize, truncation occurred.
 */
size_t __cdecl
strlcpy(char *dst, const char *src, size_t dsize)
{
	const char *osrc = src;
	size_t nleft = dsize;

	/* Copy as many bytes as will fit. */
	if (nleft != 0) {
		while (--nleft != 0) {
			if ((*dst++ = *src++) == '\0')
				break;
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src. */
	if (nleft == 0) {
		if (dsize != 0)
			*dst = '\0';		/* NUL-terminate dst */
		while (*src++)
			;
	}

	return(src - osrc - 1);	/* count does not include NUL */
}
#include<tchar.h>
size_t
_wcslcpy(wchar_t *dst, const wchar_t *src, size_t dsize)
{
	const wchar_t *osrc = src;
	size_t nleft = dsize;
	if (nleft != 0) {
		while (--nleft != 0) {
			if ((*dst++ = *src++) == L'\0')
				break;
		}
	}
	if (nleft == 0) {
		if (dsize != 0)
			*dst = L'\0';		/* NUL-terminate dst */
		while (*src++)
			;
	}
	return(src - osrc - 1);	/* count does not include NUL */
}
#endif