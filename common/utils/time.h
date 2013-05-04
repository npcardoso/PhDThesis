#ifndef __COMMON_UTIL_TIME_H__
#define __COMMON_UTIL_TIME_H__

#include "types.h"

typedef long double t_time_interval;

t_time_interval time_interval();

void msleep(t_time_interval milliseconds);

#endif
