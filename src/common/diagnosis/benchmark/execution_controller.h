#ifndef __EXECUTION_CONTROLLER_H_4a8d6f3f0d958425a90466924c1ec28cddfce318__
#define __EXECUTION_CONTROLLER_H_4a8d6f3f0d958425a90466924c1ec28cddfce318__

#include "diagnosis/diagnosis_system.h"
#include "diagnosis/benchmark/benchmark_settings.h"
#include "diagnosis/benchmark/status.h"

#include <boost/thread.hpp>
#include <queue>

namespace diagnosis {
namespace benchmark {
class t_execution_controller;
typedef t_id t_priority;

class t_job {
public:
    DEFINE_BOOST_SHARED_PTRS(t_job);

    virtual void operator () (t_execution_controller & controller) const = 0;
    virtual std::string get_description () const = 0;

    inline virtual ~t_job () {}

private:
};

class t_execution_report {
public:
    DEFINE_BOOST_SHARED_PTRS(t_execution_report);

    virtual void thread_init (t_id thread_id, std::string type) = 0;
    virtual void thread_end (t_id thread_id) = 0;
};

class t_report_null {
public:
    inline virtual void thread_init (t_id thread_id, std::string type) {}
    inline virtual void thread_end (t_id thread_id) {}
};

class t_report_csv : public t_execution_report {
public:
    virtual void thread_init (t_id thread_id, std::string type);
    virtual void thread_end (t_id thread_id);
    virtual std::ostream & print (std::ostream & out) const;

private:
    class t_element {
public:
        t_time_interval start;
        t_time_interval end;
        std::string desc;
    };

    typedef std::map<t_id, t_element> t_threads;
    t_threads threads;
};

class t_execution_controller {
public:

    t_execution_controller (t_count max_threads,
                            const t_execution_report::t_ptr & execution_report);

    bool launch_job ();


    t_count get_job_count ();


    void add_generator_job (t_id generator_id,
                            const t_benchmark_settings & settings,
                            const t_status_iteration_init::t_const_ptr & status);


    void add_ranker_job (t_id & ranker_id,
                         const t_benchmark_settings & settings,
                         const t_status_post_gen::t_const_ptr & status);

private:
    static void launch_job_fun (t_id thread_id,
                                t_execution_controller * controller,
                                t_job::t_const_ptr & job);

    void signal ();

    boost::mutex mutex;
    boost::condition_variable free_slot;

    t_execution_report::t_ptr report;


    t_count max_threads;
    t_count active_threads;
    t_count total_threads;

    typedef std::pair<t_id, t_job::t_const_ptr> t_priority_job;
    std::priority_queue<t_priority_job> jobs;
};
}
}
#endif