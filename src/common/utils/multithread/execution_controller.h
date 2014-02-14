#ifndef __CONTROLLER_H_204f329ba442736d7cb8632c05d09694b718f8c2__
#define __CONTROLLER_H_204f329ba442736d7cb8632c05d09694b718f8c2__

#include "types.h"
#include "utils/boost.h"
#include "utils/multithread/job.h"

#include <boost/thread.hpp>

class t_execution_controller {
public:
    typedef void (* t_callback)(t_id);

    t_execution_controller (t_count max_threads);

    t_id launch (const t_const_ptr<t_job> & job,
                 t_callback callback=NULL);

    void join ();

private:
    void signal ();
    static void launch_job_fun (t_execution_controller * controller,
                                t_const_ptr<t_job> job,
                                t_callback callback,
                                t_id thread_id);

    boost::mutex mutex;
    boost::condition_variable free_slot;

    t_count max_threads;
    t_count active_threads;
    t_count total_threads;
};

#else
class t_execution_controller;
#endif