#ifndef __LIBINSTRUMENT_MAIN_H__
#define __LIBINSTRUMENT_MAIN_H__

#include "threads.h"

#include "instrumentation/sinks/tcp.h"

extern t_thread_tracker * tracker;
extern t_construct_id construct_id;
extern boost::mutex mutex;

template <class CTR>
void register_construct(CTR ctr) {
  mutex.lock();
  ctr->c_id = ++construct_id;
//  (*tcp_sink()) << ctr;
  mutex.unlock();
}

#endif
