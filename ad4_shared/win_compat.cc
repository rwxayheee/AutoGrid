#include <windows.h>
#include <time.h>
#include <string.h>
#include "win_compat.h"

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000
#endif

clock_t times(struct tms* buffer) {
  FILETIME createTime, exitTime, kernelTime, userTime;
  if (GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime, &kernelTime, &userTime)) {
    ULARGE_INTEGER u, k;
    k.LowPart = kernelTime.dwLowDateTime;
    k.HighPart = kernelTime.dwHighDateTime;
    u.LowPart = userTime.dwLowDateTime;
    u.HighPart = userTime.dwHighDateTime;
    buffer->tms_utime = (clock_t)(u.QuadPart / 10000 / (1000 / CLOCKS_PER_SEC));
    buffer->tms_stime = (clock_t)(k.QuadPart / 10000 / (1000 / CLOCKS_PER_SEC));
    buffer->tms_cutime = 0;
    buffer->tms_cstime = 0;
  } else {
    memset(buffer, 0, sizeof(*buffer));
  }
  return clock();
}

int gettimeofday(struct timeval* tp, void*) {
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);
  unsigned __int64 t = ((unsigned __int64)ft.dwHighDateTime << 32) + ft.dwLowDateTime;
  t -= 116444736000000000ULL;
  tp->tv_sec = (long)(t / 10000000ULL);
  tp->tv_usec = (long)((t % 10000000ULL) / 10);
  return 0;
}

int gethostname(char* name, size_t len) {
  DWORD dlen = static_cast<DWORD>(len);
  return GetComputerNameA(name, &dlen) ? 0 : -1;
}

long sysconf(int name) {
  if (name == _SC_CLK_TCK) {
    return CLOCKS_PER_SEC;
  } else {
    return -1L;
  }
}
