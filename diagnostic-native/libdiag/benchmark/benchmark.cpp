#include "benchmark.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <boost/foreach.hpp>


namespace diagnostic {
namespace benchmark {
class t_generator_job : public t_job {
public:
    t_generator_job (t_id generator_id,
                     const t_benchmark_settings & settings,
                     const t_const_ptr<t_status_iteration_init> & status);

    void operator () () const;
    virtual bool operator < (const t_job & job) const;

private:
    t_id generator_id;
    const t_benchmark_settings & settings;
    const t_const_ptr<t_status_iteration_init> status;
};

class t_ranker_job : public t_job {
public:
    t_ranker_job (t_id ranker_id,
                  const t_benchmark_settings & settings,
                  const t_const_ptr<t_status_post_gen> & status);

    void operator () () const;
    virtual bool operator < (const t_job & job) const;

private:
    t_id ranker_id;
    const t_benchmark_settings & settings;
    const t_const_ptr<t_status_post_gen> status;
};

t_generator_job::t_generator_job (t_id generator_id,
                                  const t_benchmark_settings & settings,
                                  const t_const_ptr<t_status_iteration_init> & status) : generator_id(generator_id), settings(settings), status(status) {}

void t_generator_job::operator () () const {
    assert("TODO: Implement this!!" == NULL);
/*    const t_diagnostic_system & diag = settings.get_diagnostic_system();
    const t_const_ptr<t_candidate_generator> & generator = diag.get_generators[generator_id];
    const t_diagnostic_system::t_ranker_list & connections = diag.get_connections(generator_id);


    if (connections.size() == 0)
        return;

    t_candidate_generator::t_ret_type * candidates_tmp =
        new t_candidate_generator::t_ret_type();

    t_const_ptr<t_candidate_generator::t_ret_type> candidates(candidates_tmp);

    t_time_interval start_time = time_interval();
    (* generator)(status->get_spectra(), * candidates_tmp);

    // Hook: Post-gen
    t_const_ptr<t_status_post_gen> gen_status
        (new t_status_post_gen(*status,
                               diag.get_generator_name(generator_id),
                               start_time,
                               time_interval(),
                               candidates));
    settings.get_hook().trigger_event(settings.get_collector(),
                                      *gen_status);


    BOOST_FOREACH(t_id ranker_id, connections) {
        t_const_ptr<t_job> job(new t_ranker_job(ranker_id,
                                                settings,
                                                gen_status));


        settings.get_job_queue().add_job(job);
    }
*/
}

bool t_generator_job::operator < (const t_job & job) const {
    if (dynamic_cast<const t_ranker_job *> (&job))
        return true;

    return this < &job;
}

t_ranker_job::t_ranker_job (t_id ranker_id,
                            const t_benchmark_settings & settings,
                            const t_const_ptr<t_status_post_gen> & status) : ranker_id(ranker_id), settings(settings), status(status) {}


void t_ranker_job::operator () () const {
    const t_diagnostic_system & diag = settings.get_diagnostic_system();
    const t_const_ptr<t_candidate_ranker> & ranker = diag.get_rankers()[ranker_id];

    t_candidate_ranker::t_ret_type * probs_ptr(new t_candidate_ranker::t_ret_type());


    t_const_ptr<t_candidate_ranker::t_ret_type> probs(probs_ptr);


    t_time_interval start_time = time_interval();


    (* ranker)(status->get_spectra(),
               status->get_candidates(),
               * probs_ptr);

    // Hook: Post-rank
    t_status_post_rank rank_status(*status,
                                   std::to_string(ranker_id),
                                   start_time,
                                   time_interval(),
                                   probs);
    settings.get_hook().trigger_event(settings.get_collector(),
                                      rank_status);
}

bool t_ranker_job::operator < (const t_job & job) const {
    if (dynamic_cast<const t_generator_job *> (&job))
        return false;

    return this < &job;
}

void run_benchmark (const t_benchmark_settings & settings,
                    t_spectra_generator & generator,
                    std::mt19937 & gen,
                    t_execution_controller & controller) {
    const t_diagnostic_system & diag = settings.get_diagnostic_system();
    t_id iteration_id = 1;


    while (true) {
        t_candidate * correct_tmp = new t_candidate();
        t_const_ptr<t_candidate> correct(correct_tmp);
        t_const_ptr<t_spectra> spectra(generator(gen, *correct_tmp));

        if (spectra.get() == NULL)
            break;

        // Hook: Iteration Init
        t_const_ptr<t_status_iteration_init> it_status
            (new t_status_iteration_init(iteration_id++,
                                         time_interval(),
                                         spectra,
                                         correct));
        settings.get_hook().trigger_event(settings.get_collector(),
                                          *it_status);

        for (t_id gen_id = 1;
             gen_id <= diag.get_generators().size();
             gen_id++) {
            t_const_ptr<t_job> job(new t_generator_job(gen_id,
                                                       settings,
                                                       it_status));

            settings.get_job_queue().add_job(job);

            settings.get_job_queue().execute(controller,
                                             true);
        }
    }

    settings.get_job_queue().execute(controller,
                                     false);
}
}
}
