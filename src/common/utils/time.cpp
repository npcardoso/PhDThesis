#include "utils/time.h"

#include <sys/time.h>
#include <cstdlib>

t_time_interval time_interval () {
    timeval time;


    gettimeofday(&time, NULL);
    return time.tv_sec * 1e6 + time.tv_usec;
}