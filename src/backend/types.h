#ifndef __TYPES_H_ebe417e55c605ed9ddbd3d7a3ecc6d3aa75cbd7c__
#define __TYPES_H_ebe417e55c605ed9ddbd3d7a3ecc6d3aa75cbd7c__

#include "interface/types.h"

typedef unsigned long t_count;

#include <sys/time.h>
#include <cstdlib>

inline t_time_interval time_interval () {
    timeval time;


    gettimeofday(&time, NULL);
    return time.tv_sec * 1e6 + time.tv_usec;
}

#endif