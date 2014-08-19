#include "job_queue.h"

class t_callback_job : public t_job {
public:
    t_callback_job (const t_const_ptr<t_job> & job,
                    t_job_queue & queue) : job(job), queue(queue) {}

    virtual void operator () () const {
        (* job)();
        queue.notify();
    }

    virtual bool operator < (const t_job & job) const {
        return (*(this->job)) < job;
    }

private:
    t_const_ptr<t_job> job;
    t_job_queue & queue;
};

t_job_queue::t_job_queue () {
    running_jobs = 0;
}

void t_job_queue::add_job (const t_const_ptr<t_job> & job) {
    std::lock_guard<std::mutex> lock(mutex);


    assert(job.get());
    t_const_ptr<t_job> callback_job(new t_callback_job(job, *this));
    queue.push(callback_job);
    event.notify_all();
}

void t_job_queue::execute (t_execution_controller & controller,
                           bool break_on_free_slot) {
    std::unique_lock<std::mutex> lock(mutex);


    while (true) {
        if (queue.size()) {
            t_const_ptr<t_job> j = queue.top();
            queue.pop();
            running_jobs++;
            lock.unlock();
            controller.launch(j);
            lock.lock();
        }
        else if (break_on_free_slot) {
            break;
        }
        else if (running_jobs) {
            event.wait(lock);
        }
        else
            break;
    }
}

void t_job_queue::notify () {
    std::lock_guard<std::mutex> lock(mutex);


    running_jobs--;
    event.notify_all();
}