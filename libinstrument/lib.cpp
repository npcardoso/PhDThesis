#include "lib.h"

#include "pthread.h"
#include "utils/debug.h"

#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include <algorithm>
#include <sstream>
using namespace std;

time_interval_t getTimeInterval(){
  timeval time;
  gettimeofday(&time, NULL);
  return time.tv_sec * 10e6 + time.tv_usec;
}


void init() __attribute__((constructor));
void init() {
  pthread_t t;

  debug("Instrumentation Init");
  _instr_pthread_register_main(pthread_self());

//  pthread_create(&t, NULL, networkInterface, NULL);
}

void finish() __attribute__((destructor));
void finish() {
  debug("Instrumentation Finalize");
}
