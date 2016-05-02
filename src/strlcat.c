#include <bsd/string.h>
#include <string.h>

#if !HAVE_STRLCAT
/*
 * Appends src to string dst of size dsize (unlike strncat, dsize is the
 * full size of dst, not space left).  At most dsize-1 characters
 * will be copied.  Always NUL terminates (unless dsize <= strlen(dst)).
 * Returns strlen(src) + MIN(dsize, strlen(initial dst)).
 * If retval >= dsize, truncation occurred.
 */
size_t __cdecl
strlcat(char *dst, const char *src, size_t dsize)
{
	const char *odst = dst;
	const char *osrc = src;
	size_t n = dsize;
	size_t dlen;

	/* Find the end of dst and adjust bytes left but don't go past end. */
	while (n-- != 0 && *dst != '\0')
		dst++;
	dlen = dst - odst;
	n = dsize - dlen;

	if (n-- == 0)
		return(dlen + strlen(src));
	while (*src != '\0') {
		if (n != 0) {
			*dst++ = *src;
			n--;
		}
		src++;
	}
	*dst = '\0';

	return(dlen + (src - osrc));	/* count does not include NUL */
}
#include<tchar.h>
size_t __cdecl
_wcslcat(wchar_t *dst, const wchar_t *src, size_t dsize)
{
	const wchar_t *odst = dst;
	const wchar_t *osrc = src;
	size_t n = dsize;
	size_t dlen;
	while (n-- != 0 && *dst != L'\0')
		dst++;
	dlen = dst - odst;
	n = dsize - dlen;
	if (n-- == 0)
		return(dlen + wcslen(src));
	while (*src != L'\0') {
		if (n != 0) {
			*dst++ = *src;
			n--;
		}
		src++;
	}
	*dst = L'\0';
	return(dlen + (src - osrc));	/* count does not include NUL */
}
#endif