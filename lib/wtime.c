#include <stdlib.h>
#include <sys/time.h>

double wtime() {
  static int sec = -1;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  if (sec < 0)
    sec = tv.tv_sec;
  return (tv.tv_sec - sec) + 1.0e-6 * tv.tv_usec;
}
