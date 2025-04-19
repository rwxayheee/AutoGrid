#ifdef _WIN32
#ifndef WIN_COMPAT_H
#define WIN_COMPAT_H

#include <io.h>
#include <direct.h>
#include <process.h>
#include <ctime>
#include <cstring>
#include <time.h>

// Portable function aliases
#define strcasecmp _stricmp
#define strdup     _strdup
#define strncasecmp _strnicmp
#define getcwd     _getcwd
#define chdir      _chdir
#define access     _access
#define unlink     _unlink
#define isatty     _isatty
#define fileno     _fileno

#ifndef bzero
#define bzero(ptr, size) memset((ptr), 0, (size))
#endif

#ifndef HAVE_STRUCT_TMS
#define HAVE_STRUCT_TMS
struct tms {
  clock_t tms_utime;
  clock_t tms_stime;
  clock_t tms_cutime;
  clock_t tms_cstime;
};
#endif

// Declarations only – definitions go in win_compat.cc
clock_t ad4_times(struct tms* buffer);
int gettimeofday(struct timeval* tp, void*);
int gethostname(char* name, size_t len);
long sysconf(int name);

#endif // WIN_COMPAT_H
#endif // _WIN32
