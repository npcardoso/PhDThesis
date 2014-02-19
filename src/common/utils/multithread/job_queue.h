#ifndef __JOB_QUEUE_H_ce796a9d2e67c9e939bb258dec22b3920ba4a1db__
#define __JOB_QUEUE_H_ce796a9d2e67c9e939bb258dec22b3920ba4a1db__


#include "utils/boost.h"
#include "utils/multithread/job.h"
#include "utils/multithread/execution_controller.h"
#include "utils/stl.h"
#include "utils/time.h"

#include <queue>


class t_callback_job;

class t_job_queue {
    friend class t_callback_job;
public:

    t_job_queue ();

    void add_job (const t_const_ptr<t_job> & job);

    void execute (t_execution_controller & controller,
                  bool break_on_free_slot=false);
private:
    void notify ();

    std::mutex mutex;
    std::condition_variable event;

    t_count running_jobs;

    typedef std::priority_queue<t_const_ptr<t_job>,
                                std::vector<t_const_ptr<t_job> >,
                                t_ptr_compare<t_const_ptr<t_job> > > t_priority_queue;
    t_priority_queue queue;
};


#endif