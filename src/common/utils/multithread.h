#ifndef __MULTITHREAD_H_677d46b0ab295a6dfa90d69881758fbc2a48b498__
#define __MULTITHREAD_H_677d46b0ab295a6dfa90d69881758fbc2a48b498__

#include "types.h"
#include "utils/boost.h"
#include "utils/time.h"

#include <boost/thread.hpp>
#include <queue>

class t_execution_controller;

class t_job {
public:
    DEFINE_BOOST_SHARED_PTRS(t_job);

    virtual void operator () (t_execution_controller & controller) const = 0;
    virtual std::string get_type () const = 0;
    virtual bool operator < (const t_job & job) const = 0;

    inline virtual ~t_job () {};
};

class t_execution_report {
public:
    DEFINE_BOOST_SHARED_PTRS(t_execution_report);
    virtual void thread_init (t_id thread_id, std::string type) = 0;
    virtual void thread_start (t_id thread_id) = 0;
    virtual void thread_end (t_id thread_id) = 0;
    inline virtual ~t_execution_report () {}
};

class t_report_null : public t_execution_report {
public:
    inline virtual void thread_init (t_id thread_id, std::string type) {}
    inline virtual void thread_start (t_id thread_id) {}
    inline virtual void thread_end (t_id thread_id) {}
};

class t_report_csv : public t_execution_report {
public:
    virtual void thread_init (t_id thread_id, std::string type);
    virtual void thread_start (t_id thread_id);
    virtual void thread_end (t_id thread_id);
    virtual std::ostream & print (std::ostream & out) const;

private:
    class t_element {
public:
        t_time_interval init;
        t_time_interval start;
        t_time_interval end;
        std::string type;
    };

    typedef std::map<t_id, t_element> t_threads;
    t_threads threads;
};


class t_execution_controller {
public:

    t_execution_controller (t_count max_threads);
    t_execution_controller (t_count max_threads,
                            const t_execution_report::t_ptr & execution_report);


    void add_job (const t_job::t_const_ptr & job);
    t_count get_job_count ();
    bool launch_job ();
    void join_all ();

private:
    class t_queued_job {
public:
        inline t_queued_job (t_id id, const t_job::t_const_ptr & job) : job(job) {this->id = id;}
        inline t_id get_id () const {return id;}
        inline const t_job::t_const_ptr & get_job () const {return job;}
        inline bool operator < (const t_queued_job & job) const {return (*(this->job)) < (*job.job);}
private:
        t_id id;
        t_job::t_const_ptr job;
    };

    static void launch_job_fun (t_execution_controller * controller,
                                t_queued_job queued_job);

    void signal (t_queued_job queued_job);

    boost::mutex mutex;
    boost::condition_variable free_slot;

    t_execution_report::t_ptr report;


    t_count max_threads;
    t_count active_threads;
    t_count total_threads;

    std::priority_queue<t_queued_job> jobs;
};


#endif