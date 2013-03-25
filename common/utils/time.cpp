#include "utils/time.h"

#include <sys/time.h>
#include <boost/thread/thread.hpp>

t_time_interval time_interval(){
  timeval time;
  gettimeofday(&time, NULL);
  return time.tv_sec * 10e6 + time.tv_usec;
}

void msleep(t_time_interval milliseconds) {
  boost::this_thread::sleep(boost::posix_time::milliseconds(milliseconds));
}

