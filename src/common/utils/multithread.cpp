#include "multithread.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

void t_report_csv::thread_init (t_id thread_id, std::string type) {
    t_element e;


    e.init = time_interval();
    e.type = type;
    e.start = 0;
    e.end = 0;
    threads[thread_id] = e;
}

void t_report_csv::thread_start (t_id thread_id) {
    threads[thread_id].start = time_interval();
}

void t_report_csv::thread_end (t_id thread_id) {
    threads[thread_id].end = time_interval();
}

std::ostream & t_report_csv::print (std::ostream & out) const {
    out << "id, init, start, end, type\n";

    BOOST_FOREACH(t_threads::value_type e,
                  threads) {
        out << e.first << ", ";
        out << boost::lexical_cast<std::string> (e.second.init) << ", ";
        out << boost::lexical_cast<std::string> (e.second.start) << ", ";
        out << boost::lexical_cast<std::string> (e.second.end) << ", \"";
        out << e.second.type << "\"\n";
    }
    return out;
}

t_execution_controller::t_execution_controller (t_count max_threads) {
    report = t_execution_report::t_ptr(new t_report_null());
    this->max_threads = max_threads;
    active_threads = 0;
    total_threads = 0;
}

t_execution_controller::t_execution_controller (t_count max_threads,
                                                const t_execution_report::t_ptr & execution_report) : report(execution_report) {
    this->max_threads = max_threads;
    active_threads = 0;
    total_threads = 0;
}

void t_execution_controller::add_job (const t_job::t_const_ptr & job) {
    boost::mutex::scoped_lock lock(mutex);
    t_queued_job queued_job(total_threads++, job);


    report->thread_init(queued_job.get_id(), job->get_type());
    jobs.push(queued_job);
}

t_count t_execution_controller::get_job_count () {
    boost::mutex::scoped_lock lock(mutex);


    return jobs.size();
}

bool t_execution_controller::launch_job () {
    boost::mutex::scoped_lock lock(mutex);


    if (active_threads >= max_threads)
        free_slot.wait(lock);

    if (jobs.size() > 0) {
        t_queued_job j = jobs.top();
        jobs.pop();

        report->thread_start(j.get_id());
        active_threads++;
        boost::thread t(t_execution_controller::launch_job_fun,
                        this,
                        j);
        t.detach();
        return true;
    }

    return false;
}

void t_execution_controller::join_all () {
    boost::mutex::scoped_lock lock(mutex);


    while (active_threads > 0)
        free_slot.wait(lock);
}

void t_execution_controller::launch_job_fun (t_execution_controller * controller,
                                             t_queued_job queued_job) {
    (*queued_job.get_job())(*controller);
    controller->signal(queued_job);
}

void t_execution_controller::signal (t_queued_job queued_job) {
    boost::mutex::scoped_lock lock(mutex);


    assert(active_threads > 0);
    active_threads--;
    report->thread_end(queued_job.get_id());

    free_slot.notify_one();
}