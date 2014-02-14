#include "execution_controller.h"

t_execution_controller::t_execution_controller (t_count max_threads) {
    this->max_threads = max_threads;
    active_threads = 0;
    total_threads = 0;
}

t_id t_execution_controller::launch (const t_const_ptr<t_job> & job,
                                     t_callback callback) {
    boost::mutex::scoped_lock lock(mutex);


    if (active_threads >= max_threads)
        free_slot.wait(lock);

    total_threads++;
    active_threads++;
    boost::thread t(launch_job_fun, this, job, callback, total_threads);
    t.detach();

    return total_threads;
}

void t_execution_controller::join () {
    boost::mutex::scoped_lock lock(mutex);


    free_slot.wait(lock);
}

void t_execution_controller::launch_job_fun (t_execution_controller * controller,
                                             t_const_ptr<t_job> job,
                                             t_callback callback,
                                             t_id thread_id) {
    (* job)();
    controller->signal();

    if (callback)
        callback(thread_id);
}

void t_execution_controller::signal () {
    boost::mutex::scoped_lock lock(mutex);


    assert(active_threads > 0);
    active_threads--;
    free_slot.notify_all();
}