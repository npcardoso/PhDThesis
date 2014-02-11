#include "execution_controller.h"
#include "diagnosis/benchmark/status.h"

#include <boost/foreach.hpp>

namespace diagnosis {
namespace benchmark {
class t_generator_job : public t_job {
public:
    t_generator_job (t_execution_controller & controller,
                     t_id generator_id,
                     const t_benchmark_settings & settings,
                     const t_status_iteration_init::t_const_ptr & status);

    virtual void operator () () const;
private:
    t_id generator_id;
    const t_benchmark_settings & settings;
    const t_status_iteration_init::t_const_ptr status;
};

class t_ranker_job : public t_job {
public:
    t_ranker_job (t_execution_controller & controller,
                  t_id ranker_id,
                  const t_benchmark_settings & settings,
                  const t_status_post_gen::t_const_ptr & status);

    virtual void operator () () const;
private:
    t_id ranker_id;
    const t_benchmark_settings & settings;
    const t_status_post_gen::t_const_ptr status;
};


t_execution_controller & t_job::get_controller () const {
    return controller;
}

t_generator_job::t_generator_job (t_execution_controller & controller,
                                  t_id generator_id,
                                  const t_benchmark_settings & settings,
                                  const t_status_iteration_init::t_const_ptr & status) : t_job(controller), generator_id(generator_id), settings(settings), status(status) {}


void t_generator_job::operator () () const {
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
        get_controller().add_ranker_job(ranker_id,
                                        settings,
                                        gen_status);
    }
}

t_ranker_job::t_ranker_job (t_execution_controller & controller,
                            t_id ranker_id,
                            const t_benchmark_settings & settings,
                            const t_status_post_gen::t_const_ptr & status) : t_job(controller), ranker_id(ranker_id), settings(settings), status(status) {}


void t_ranker_job::operator () () const {
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

bool t_execution_controller::launch_job () {
    if (jobs.size() == 0)
        return false;

    t_priority_job j = jobs.top();
    jobs.pop();
    (*j.second)();
    return true;
}

void t_execution_controller::add_generator_job (t_id generator_id,
                                                const t_benchmark_settings & settings,
                                                const t_status_iteration_init::t_const_ptr & status) {
    t_job::t_const_ptr job(new t_generator_job(*this,
                                               generator_id,
                                               settings,
                                               status));


    jobs.push(t_priority_job(1, job));
}

void t_execution_controller::add_ranker_job (t_id & ranker_id,
                                             const t_benchmark_settings & settings,
                                             const t_status_post_gen::t_const_ptr & status) {
    t_job::t_const_ptr job(new t_ranker_job(*this,
                                            ranker_id,
                                            settings,
                                            status));


    jobs.push(t_priority_job(2, job)); // The priority values influence the order in which jobs are executed
}
}
}