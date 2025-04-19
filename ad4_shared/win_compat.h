#ifdef _WIN32
#ifndef WIN_COMPAT_H
#define WIN_COMPAT_H

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define HAVE_SYS_TIME_H

#include <windows.h>
#include <winsock2.h>
#include <io.h>
#include <direct.h>
#include <process.h>
#include <ctime>
#include <cstring>
#include <time.h>

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

// Only declare here; implement in win_compat.cc
clock_t ad4_times(struct tms* buffer);

inline int gettimeofday(struct timeval* tp, void*) {
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);
  unsigned __int64 t = ((unsigned __int64)ft.dwHighDateTime << 32) + ft.dwLowDateTime;
  t -= 116444736000000000ULL;
  tp->tv_sec = (long)(t / 10000000ULL);
  tp->tv_usec = (long)((t % 10000000ULL) / 10);
  return 0;
}

#ifndef _SC_CLK_TCK
#define _SC_CLK_TCK 3
#endif

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000
#endif

inline long sysconf(int name) {
  if (name == _SC_CLK_TCK) {
    return CLOCKS_PER_SEC;
  } else {
    return -1L;
  }
}

inline int gethostname(char* name, size_t len) {
  DWORD dlen = static_cast<DWORD>(len);
  return GetComputerNameA(name, &dlen) ? 0 : -1;
}

#endif // WIN_COMPAT_H
#endif // _WIN32
