#ifndef __UTILS_H__
#define __UTILS_H__

#include <sys/time.h>

#define iterators(C) (C).begin(), (C).end()
#define riterators(C) (C).rbegin(), (C).rend()

#define foreach(var, container) \
    for(typeof((container).begin()) var = (container).begin(); \
            var != (container).end(); \
            ++var)

typedef long double time_interval_t;
inline time_interval_t get_time_interval(){
  timeval time;
  gettimeofday(&time, NULL);
  return time.tv_sec + time.tv_usec / 1e6;
}

#endif
