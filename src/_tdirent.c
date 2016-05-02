/*
 * dirent.c
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER within the package.
 *
 * Derived from DIRLIB.C by Matt J. Weinstein
 * This note appears in the DIRLIB.H
 * DIRLIB.H by M. J. Weinstein   Released to public domain 1-Jan-89
 *
 * Updated by Jeremy Bettis <jeremy@hksys.com>
 * Significantly revised and rewinddir, seekdir and telldir added by Colin
 * Peters <colin@fu.is.saga-u.ac.jp>
 *	
 */

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <io.h>
#include <direct.h>
#include <dirent.h>
#include <fcntl.h>

#include <windows.h> /* for GetFileAttributes */

#include <tchar.h>
#define SUFFIX	_T("*")
#define	SLASH	_T("\\")

#include <crtdbg.h>
#include <p4msvc_utils.h>

#ifndef INSTALL_INVALID_PARAM_HANDLER
 #define INSTALL_INVALID_PARAM_HANDLER(fun)\
    _invalid_parameter_handler newHandler;\
	newHandler = (fun);\
	_set_invalid_parameter_handler(newHandler);\
	_CrtSetReportMode(_CRT_ASSERT, 0);
 #endif
 
 


#ifndef _tmemmove
#ifdef _UNICODE
#define _tmemmove wmemmove
#else
#define _tmemmove memmove
#endif
#endif /* _tmemmove */

/*
 * fdopendir
 *
 * Returns a pointer to a DIR structure appropriately filled in to begin
 * searching a directory.
 */
_TDIR *
_tfdopendir(int fd)
{
	HANDLE hFile;
	unsigned int rc;
	_TCHAR szFullPath[MAX_PATH];
	_TCHAR szBasePath[MAX_PATH];
	int bRet;
	BY_HANDLE_FILE_INFORMATION info;

	/* we don't want an exception if _get_osfhandle receives a bad parameter. */
	INSTALL_INVALID_PARAM_HANDLER(my_osfhandle_invalid_handler);

	/* First check we can get a native handle from it file descriptor. */
	hFile = (HANDLE)_get_osfhandle(fd);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		_set_errno(EBADF);
		return (_TDIR *)0;
	}

	/* Check it's a normal file and not a directory. */
	memset(&info, 0, sizeof(BY_HANDLE_FILE_INFORMATION));
	bRet = GetFileInformationByHandle(hFile, &info);
	if (!bRet || (FILE_ATTRIBUTE_ARCHIVE != info.dwFileAttributes)) {
		_set_errno(EBADF);
		return (_TDIR *)0;
	}

	rc = GetFinalPathNameByHandle(hFile, szFullPath, MAX_PATH, FILE_NAME_NORMALIZED);
	if (rc > 4)
	{
		if (_tcsncmp(szFullPath, _T("\\\\?\\"), _tcslen(_T("\\\\?\\"))) == 0) {
			//_tmemmove(szFullPath, szFullPath + 4, _tcslen(szFullPath + 4) + 1);
			_tcsncpy(szBasePath, szFullPath + (4 * sizeof(TCHAR)), _tcslen(szFullPath + 4) + 1);
			TCHAR* pLastSlash = _tcsrchr(szBasePath, _T('\\'));
			if (pLastSlash) 
			{
				*pLastSlash = _T('\0');
				return _topendir(szBasePath);
			}
		}
	}
	return (_TDIR *)0;
}

/*
 * dirfd
 *
 * The function dirfd() returns the file descriptor associated with the
 *      directory stream dirp.
 */
int __cdecl 
_tdirfd(_TDIR *dirp)
{
   int fd;
  _TCHAR szFullPath[MAX_PATH];
   size_t ulLen;
   HANDLE hDir;
  
  fd = -1;
  errno = 0;
	
  /* Check for valid DIR struct. */
  if (!dirp)
    {
      errno = EINVAL;
      return -1;
    }
  
  if (dirp->dd_stat <= 0)
    {
      errno = EINVAL;
      return -1;
    }
	
	ulLen = _tcslen(dirp->dd_name);
	_tcscpy (szFullPath, dirp->dd_name);
	if (ulLen > 3)
	  {
		if (szFullPath[ulLen - 1] == _T('*'))
		  {
		    szFullPath[ulLen - 1] = _T('\0');
			_tcscat (szFullPath, _T("\\"));
			
			if ((_tcsncmp(dirp->dd_dir.d_name, _T("."), 1) != 0) &&
			    (_tcsncmp(dirp->dd_dir.d_name, _T(".."), 2) != 0))
				{
			      _tcscat(szFullPath, dirp->dd_dir.d_name);
				}
			    
				hDir = CreateFile(szFullPath, FILE_GENERIC_READ,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL,
					OPEN_ALWAYS,
					FILE_FLAG_BACKUP_SEMANTICS,
					NULL);
				if (hDir != INVALID_HANDLE_VALUE)
				  {
					  fd = _open_osfhandle((intptr_t)hDir, _O_RDONLY);
					  if (fd > 0)
					  {
						  //TODO - register this fd inside dirp->dd_fds
						  //For now we might leak some handles
					  }
				  }	
		  }
	  }
	
	
  return fd;
}


/*
 * opendir
 *
 * Returns a pointer to a DIR structure appropriately filled in to begin
 * searching a directory.
 */
_TDIR *
_topendir (const _TCHAR *szPath)
{
  _TDIR *nd;
  unsigned int rc;
  _TCHAR szFullPath[MAX_PATH];

  errno = 0;

  if (!szPath)
    {
      errno = EFAULT;
      return (_TDIR *) 0;
    }

  if (szPath[0] == _T('\0'))
    {
      errno = ENOTDIR;
      return (_TDIR *) 0;
    }

  /* Attempt to determine if the given path really is a directory. */
  rc = GetFileAttributes (szPath);
  if (rc == INVALID_FILE_ATTRIBUTES)
    {
      /* call GetLastError for more error info */
      errno = ENOENT;
      return (_TDIR *) 0;
    }
  if (!(rc & FILE_ATTRIBUTE_DIRECTORY))
    {
      /* Error, entry exists but not a directory. */
      errno = ENOTDIR;
      return (_TDIR *) 0;
    }

  /* Make an absolute pathname.  */
  _tfullpath (szFullPath, szPath, MAX_PATH);

  /* Allocate enough space to store DIR structure and the complete
   * directory path given. */
  nd = (_TDIR *) malloc (sizeof (_TDIR) + (_tcslen (szFullPath)
					   + _tcslen (SLASH)
					   + _tcslen (SUFFIX) + 1)
					  * sizeof (_TCHAR));

  if (!nd)
    {
      /* Error, out of memory. */
      errno = ENOMEM;
      return (_TDIR *) 0;
    }

  /* Create the search expression. */
  _tcscpy (nd->dd_name, szFullPath);

  /* Add on a slash if the path does not end with one. */
  if (nd->dd_name[0] != _T('\0') &&
      nd->dd_name[_tcslen (nd->dd_name) - 1] != _T('/') &&
      nd->dd_name[_tcslen (nd->dd_name) - 1] != _T('\\'))
    {
      _tcscat (nd->dd_name, SLASH);
    }

  /* Add on the search pattern */
  _tcscat (nd->dd_name, SUFFIX);

  /* Initialize handle to -1 so that a premature closedir doesn't try
   * to call _findclose on it. */
  nd->dd_handle = -1;
  //memset(nd->dd_fds, -1, sizeof(nd->dd_fds) / sizeof(nd->dd_fds[0]));


  /* Initialize the status. */
  nd->dd_stat = 0;

  /* Initialize the dirent structure. ino and reclen are invalid under
   * Win32, and name simply points at the appropriate part of the
   * findfirst_t structure. */
  nd->dd_dir.d_ino = 0;
  nd->dd_dir.d_reclen = 0;
  nd->dd_dir.d_namlen = 0;
  memset (nd->dd_dir.d_name, 0, 260 /*FILENAME_MAX*/);

  return nd;
}


/*
 * readdir
 *
 * Return a pointer to a dirent structure filled with the information on the
 * next entry in the directory.
 */
struct _tdirent *
_treaddir (_TDIR * dirp)
{
  errno = 0;

  /* Check for valid DIR struct. */
  if (!dirp)
    {
      errno = EFAULT;
      return (struct _tdirent *) 0;
    }

  if (dirp->dd_stat < 0)
    {
      /* We have already returned all files in the directory
       * (or the structure has an invalid dd_stat). */
      return (struct _tdirent *) 0;
    }
  else if (dirp->dd_stat == 0)
    {
      /* We haven't started the search yet. */
      /* Start the search */
      dirp->dd_handle = _tfindfirst (dirp->dd_name, &(dirp->dd_dta));

      if (dirp->dd_handle == -1)
	{
	  /* Whoops! Seems there are no files in that
	   * directory. */
	  dirp->dd_stat = -1;
	}
      else
	{
	  dirp->dd_stat = 1;
	}
    }
  else
    {
      /* Get the next search entry. */
      if (_tfindnext (dirp->dd_handle, &(dirp->dd_dta)))
	{
	  /* We are off the end or otherwise error.
	     _findnext sets errno to ENOENT if no more file
	     Undo this. */
	  DWORD winerr = GetLastError ();
	  if (winerr == ERROR_NO_MORE_FILES)
	    errno = 0;
	  _findclose (dirp->dd_handle);
	  dirp->dd_handle = -1;
	  dirp->dd_stat = -1;
	}
      else
	{
	  /* Update the status to indicate the correct
	   * number. */
	  dirp->dd_stat++;
	}
    }

  if (dirp->dd_stat > 0)
    {
      /* Successfully got an entry. Everything about the file is
       * already appropriately filled in except the length of the
       * file name. */
      dirp->dd_dir.d_namlen = _tcslen (dirp->dd_dta.name);
      _tcscpy (dirp->dd_dir.d_name, dirp->dd_dta.name);
      return &dirp->dd_dir;
    }

  return (struct _tdirent *) 0;
}


/*
 * closedir
 *
 * Frees up resources allocated by opendir.
 */
int
_tclosedir (_TDIR * dirp)
{
  int rc;

  errno = 0;
  rc = 0;

  if (!dirp)
    {
      errno = EFAULT;
      return -1;
    }

  if (dirp->dd_handle != -1)
    {
      rc = _findclose (dirp->dd_handle);
    }

  /* Delete the dir structure. */
  free (dirp);

  return rc;
}

/*
 * rewinddir
 *
 * Return to the beginning of the directory "stream". We simply call findclose
 * and then reset things like an opendir.
 */
void
_trewinddir (_TDIR * dirp)
{
  errno = 0;

  if (!dirp)
    {
      errno = EFAULT;
      return;
    }

  if (dirp->dd_handle != -1)
    {
      _findclose (dirp->dd_handle);
    }

  dirp->dd_handle = -1;
  dirp->dd_stat = 0;
}

/*
 * telldir
 *
 * Returns the "position" in the "directory stream" which can be used with
 * seekdir to go back to an old entry. We simply return the value in stat.
 */
long
_ttelldir (_TDIR * dirp)
{
  errno = 0;

  if (!dirp)
    {
      errno = EFAULT;
      return -1;
    }
  return dirp->dd_stat;
}

/*
 * seekdir
 *
 * Seek to an entry previously returned by telldir. We rewind the directory
 * and call readdir repeatedly until either dd_stat is the position number
 * or -1 (off the end). This is not perfect, in that the directory may
 * have changed while we weren't looking. But that is probably the case with
 * any such system.
 */
void
_tseekdir (_TDIR * dirp, long lPos)
{
  errno = 0;

  if (!dirp)
    {
      errno = EFAULT;
      return;
    }

  if (lPos < -1)
    {
      /* Seeking to an invalid position. */
      errno = EINVAL;
      return;
    }
  else if (lPos == -1)
    {
      /* Seek past end. */
      if (dirp->dd_handle != -1)
	{
	  _findclose (dirp->dd_handle);
	}
      dirp->dd_handle = -1;
      dirp->dd_stat = -1;
    }
  else
    {
      /* Rewind and read forward to the appropriate index. */
      _trewinddir (dirp);

      while ((dirp->dd_stat < lPos) && _treaddir (dirp))
	;
    }
}

