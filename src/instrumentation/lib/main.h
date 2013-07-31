#ifndef __LIBINSTRUMENT_MAIN_H__
#define __LIBINSTRUMENT_MAIN_H__

#include "threads.h"


extern t_thread_tracker::t_ptr tracker;
extern t_construct_id construct_id;
extern boost::mutex mutex;

template < class CTR >
void register_construct (CTR ctr) {
    mutex.lock();
    ctr->c_id = ++construct_id;
    mutex.unlock();
}

#endif