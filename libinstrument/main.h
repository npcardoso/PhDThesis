#ifndef __LIBINSTRUMENT_MAIN_H__
#define __LIBINSTRUMENT_MAIN_H__

#include "threads.h"

extern t_thread_tracker * tracker;
void register_construct(t_construct::t_ptr construct);

#endif
