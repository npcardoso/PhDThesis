#include "execution_controller.h"
#include "diagnosis/benchmark/status.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

namespace diagnosis {
namespace benchmark {
void t_report_csv::thread_init (t_id thread_id, std::string type) {
    t_element e;


    e.start = time_interval();
    e.desc = type;
    e.end = 0;
    threads[thread_id] = e;
}

void t_report_csv::thread_end (t_id thread_id) {
    threads[thread_id].end = time_interval();
}

std::ostream & t_report_csv::print (std::ostream & out) const {
    out << "id, start, end, type\n";

    BOOST_FOREACH(t_threads::value_type e,
                  threads) {
        out << e.first << ", ";
        out << boost::lexical_cast<std::string> (e.second.start) << ", ";
        out << boost::lexical_cast<std::string> (e.second.end) << ", \"";
        out << e.second.desc << "\"\n";
    }
    return out;
}

class t_generator_job : public t_job {
public:
    t_generator_job (t_id generator_id,
                     const t_benchmark_settings & settings,
                     const t_status_iteration_init::t_const_ptr & status);

    virtual void operator () (t_execution_controller & controller) const;
    virtual std::string get_description () const;

private:
    t_id generator_id;
    const t_benchmark_settings & settings;
    const t_status_iteration_init::t_const_ptr status;
};

class t_ranker_job : public t_job {
public:
    t_ranker_job (t_id ranker_id,
                  const t_benchmark_settings & settings,
                  const t_status_post_gen::t_const_ptr & status);

    virtual void operator () (t_execution_controller & controller) const;
    virtual std::string get_description () const;

private:
    t_id ranker_id;
    const t_benchmark_settings & settings;
    const t_status_post_gen::t_const_ptr status;
};


t_generator_job::t_generator_job (t_id generator_id,
                                  const t_benchmark_settings & settings,
                                  const t_status_iteration_init::t_const_ptr & status) : generator_id(generator_id), settings(settings), status(status) {}


void t_generator_job::operator () (t_execution_controller & controller) const {
    const t_candidate_generator::t_const_ptr & generator = settings.get_generator(generator_id);
    const t_benchmark_hook & hook = settings.get_hook();
    t_collector & collector = settings.get_collector();
    const t_benchmark_settings::t_ranker_list & connections = settings.get_connections(generator_id);


    if (connections.size() == 0)
        return;

    t_candidate_generator::t_ret_type * candidates_tmp =
        new t_candidate_generator::t_ret_type();

    t_candidate_generator::t_ret_type::t_const_ptr candidates(candidates_tmp);

    t_time_interval last_time = time_interval();
    (* generator)(status->get_spectra(), * candidates_tmp);

    // Hook: Post-gen
    t_status_post_gen::t_const_ptr gen_status
        (new t_status_post_gen(*status,
                               settings.get_generator_name(generator_id),
                               time_interval() - last_time,
                               candidates));
    hook.post_gen(collector,
                  *gen_status);


    BOOST_FOREACH(t_id ranker_id, connections) {
        controller.add_ranker_job(ranker_id,
                                  settings,
                                  gen_status);
    }
}

std::string t_generator_job::get_description () const {
    return "generator";
}

t_ranker_job::t_ranker_job (t_id ranker_id,
                            const t_benchmark_settings & settings,
                            const t_status_post_gen::t_const_ptr & status) : ranker_id(ranker_id), settings(settings), status(status) {}


void t_ranker_job::operator () (t_execution_controller & controller) const {
    const t_candidate_ranker::t_const_ptr & ranker = settings.get_ranker(ranker_id);
    const t_benchmark_hook & hook = settings.get_hook();
    t_collector & collector = settings.get_collector();

    t_candidate_ranker::t_ret_type * probs_ptr(new t_candidate_ranker::t_ret_type());
    t_candidate_ranker::t_ret_type::t_const_ptr probs(probs_ptr);


    t_time_interval last_time = time_interval();


    (* ranker)(status->get_spectra(),
               status->get_candidates(),
               * probs_ptr);

    // Hook: Post-rank
    t_status_post_rank rank_status(*status,
                                   settings.get_ranker_name(ranker_id),
                                   time_interval() - last_time,
                                   probs);
    hook.post_rank(collector,
                   rank_status);
}

std::string t_ranker_job::get_description () const {
    return "ranker";
}

t_execution_controller::t_execution_controller (t_count max_threads,
                                                const t_execution_report::t_ptr & execution_report) : report(execution_report) {
    this->max_threads = max_threads;
    active_threads = 0;
    total_threads = 0;
}

void t_execution_controller::launch_job_fun (t_id thread_id,
                                             t_execution_controller * controller,
                                             t_job::t_const_ptr & job) {
    controller->report->thread_init(thread_id, job->get_description());
    (* job)(* controller);
    controller->report->thread_end(thread_id);
    controller->signal();
}

bool t_execution_controller::launch_job () {
    boost::mutex::scoped_lock lock(mutex);
    std::cerr << "active_threads: " << active_threads;
    std::cerr << " max_threads: " << max_threads << std::endl;


    if (active_threads >= max_threads) {
        std::cerr << "Waiting..." << std::endl;
        free_slot.wait(lock);
        std::cerr << "Got condition!!!" << std::endl;
    }

    if (jobs.size() > 0) {
        t_priority_job j = jobs.top();
        jobs.pop();
        active_threads++;
        boost::thread t(t_execution_controller::launch_job_fun, total_threads++, this, j.second);
        t.detach();
        return true;
    }

    return false;
}

void t_execution_controller::signal () {
    boost::mutex::scoped_lock lock(mutex);


    assert(active_threads > 0);
    active_threads--;
    free_slot.notify_one();
}

t_count t_execution_controller::get_job_count () {
    boost::mutex::scoped_lock lock(mutex);


    return jobs.size();
}

void t_execution_controller::add_generator_job (t_id generator_id,
                                                const t_benchmark_settings & settings,
                                                const t_status_iteration_init::t_const_ptr & status) {
    t_job::t_const_ptr job(new t_generator_job(generator_id,
                                               settings,
                                               status));

    boost::mutex::scoped_lock lock(mutex);


    jobs.push(t_priority_job(1, job));
}

void t_execution_controller::add_ranker_job (t_id & ranker_id,
                                             const t_benchmark_settings & settings,
                                             const t_status_post_gen::t_const_ptr & status) {
    t_job::t_const_ptr job(new t_ranker_job(ranker_id,
                                            settings,
                                            status));

    boost::mutex::scoped_lock lock(mutex);


    jobs.push(t_priority_job(2, job)); // The priority values influence the order in which jobs are executed
}
}
}