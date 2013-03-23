#include "utils/time.h"

#include <sys/time.h>

t_time_interval time_interval(){
  timeval time;
  gettimeofday(&time, NULL);
  return time.tv_sec * 10e6 + time.tv_usec;
}

