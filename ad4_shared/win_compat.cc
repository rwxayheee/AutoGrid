#include "win_compat.h"
#include <windows.h>
#include <time.h>
#include <string.h>

clock_t ad4_times(struct tms* buffer) {
  FILETIME createTime, exitTime, kernelTime, userTime;
  if (GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime, &kernelTime, &userTime)) {
    ULARGE_INTEGER u, k;
    k.LowPart = kernelTime.dwLowDateTime;
    k.HighPart = kernelTime.dwHighDateTime;
    u.LowPart = userTime.dwLowDateTime;
    u.HighPart = userTime.dwHighDateTime;
    buffer->tms_utime = (clock_t)(u.QuadPart / 10000 / (1000 / CLK_TCK));
    buffer->tms_stime = (clock_t)(k.QuadPart / 10000 / (1000 / CLK_TCK));
    buffer->tms_cutime = 0;
    buffer->tms_cstime = 0;
  } else {
    memset(buffer, 0, sizeof(*buffer));
  }
  return clock();
}
