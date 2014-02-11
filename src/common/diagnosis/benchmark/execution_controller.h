#ifndef __EXECUTION_CONTROLLER_H_4a8d6f3f0d958425a90466924c1ec28cddfce318__
#define __EXECUTION_CONTROLLER_H_4a8d6f3f0d958425a90466924c1ec28cddfce318__

#include "diagnosis/diagnosis_system.h"
#include "diagnosis/benchmark/benchmark_settings.h"
#include "diagnosis/benchmark/status.h"


#include <queue>

namespace diagnosis {
namespace benchmark {
class t_execution_controller;
typedef t_id t_priority;

class t_job {
public:
    DEFINE_BOOST_SHARED_PTRS(t_job);

    inline t_job (t_execution_controller & controller) : controller(controller) {}
    virtual void operator () () const = 0;

    inline virtual ~t_job () {}

protected:
    t_execution_controller & get_controller () const;
private:
    t_execution_controller & controller;
};


class t_execution_controller {
public:
    bool launch_job ();

    void add_generator_job (t_id generator_id,
                            const t_benchmark_settings & settings,
                            const t_status_iteration_init::t_const_ptr & status);


    void add_ranker_job (t_id & ranker_id,
                         const t_benchmark_settings & settings,
                         const t_status_post_gen::t_const_ptr & status);

private:


    typedef std::pair<t_id, t_job::t_const_ptr> t_priority_job;
    std::priority_queue<t_priority_job> jobs;
};
}
}
#endif