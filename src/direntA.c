#undef _UNICODE
#undef UNICODE

#define _TDIR DIR
#define _tdirent dirent
#define _tfdopendir fdopendir 
#define _topendir opendir
#define _treaddir readdir
#define _tclosedir closedir
#define _trewinddir rewinddir
#define _ttelldir telldir
#define _tseekdir seekdir
#define _tdirfd dirfd
#include "_tdirent.c"
