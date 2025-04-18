#ifdef _WIN32
#ifndef WIN_COMPAT_H
#define WIN_COMPAT_H

#include <io.h>
#include <direct.h>
#include <process.h>
#include <windows.h>
#include <ctime>
#include <cstring>

#define strcasecmp _stricmp
#define strdup     _strdup
#define strncasecmp _strnicmp
#define getcwd     _getcwd
#define chdir      _chdir
#define access     _access
#define unlink     _unlink
#define isatty     _isatty
#define fileno     _fileno

// timeval replacement
struct timeval {
  long tv_sec;
  long tv_usec;
};

inline int gettimeofday(struct timeval* tp, void*) {
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);
  unsigned __int64 t = ((unsigned __int64)ft.dwHighDateTime << 32) + ft.dwLowDateTime;
  t -= 116444736000000000ULL;
  tp->tv_sec = (long)(t / 10000000ULL);
  tp->tv_usec = (long)((t % 10000000ULL) / 10);
  return 0;
}

// tms & times() replacement
#include <sys/types.h> // for clock_t
#ifndef CLK_TCK
#define CLK_TCK CLOCKS_PER_SEC
#endif

struct tms {
  clock_t tms_utime;  // user time
  clock_t tms_stime;  // system time
  clock_t tms_cutime; // user time, children
  clock_t tms_cstime; // system time, children
};

inline clock_t times(struct tms* buffer) {
  FILETIME createTime, exitTime, kernelTime, userTime;
  if (GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime, &kernelTime, &userTime)) {
    ULARGE_INTEGER u, k;
    k.LowPart = kernelTime.dwLowDateTime;
    k.HighPart = kernelTime.dwHighDateTime;
    u.LowPart = userTime.dwLowDateTime;
    u.HighPart = userTime.dwHighDateTime;
    buffer->tms_utime = (clock_t)(u.QuadPart / 10000 / (1000 / CLK_TCK)); // Convert to ticks
    buffer->tms_stime = (clock_t)(k.QuadPart / 10000 / (1000 / CLK_TCK));
    buffer->tms_cutime = 0;
    buffer->tms_cstime = 0;
  } else {
    memset(buffer, 0, sizeof(*buffer));
  }
  return clock(); // fallback for elapsed time
}

#ifndef _SC_CLK_TCK
#define _SC_CLK_TCK 3
#endif

inline long sysconf(int name) {
    if (name == _SC_CLK_TCK) {
        return CLOCKS_PER_SEC;
    } else {
        return -1L;
    }
}

inline int gethostname(char *name, size_t len) {
    DWORD dlen = static_cast<DWORD>(len);
    return GetComputerNameA(name, &dlen) ? 0 : -1;
}


#endif // WIN_COMPAT_H
#endif // _WIN32
