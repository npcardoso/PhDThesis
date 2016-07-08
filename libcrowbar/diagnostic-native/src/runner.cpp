#include "runner.h"

namespace diagnostic {

t_const_ptr<t_diagnostic_report> t_basic_runner::run (const t_spectrum & spectrum,
                                                      t_const_ptr<t_diagnostic_system> ds) const {
    t_ptr<t_diagnostic_report> dr(new t_diagnostic_report(ds));
    auto & gen_results = dr->get_generator_results();
    auto & rank_results = dr->get_ranker_results();
    auto & connections = ds->get_connections();

    for (t_id i = 0; i < connections.size(); i++) {
        auto & c = connections[i];
        t_const_ptr<t_candidate_generator::t_ret_type> D;
        auto generator = ds->get_generators()[c.get_from()];
        auto ranker = ds->get_rankers()[c.get_to()];

        if(generator.get() == NULL || ranker.get() == NULL) {
            continue;
        }

        // If generators's result already exists, do not run it
        if(gen_results[c.get_from()].get() == NULL) {
            std::cout << "Running: " << generator->get_name() << std::endl;
            auto * tmp = new t_candidate_generator::t_ret_type();
            (*generator)(spectrum, *tmp);
            dr->add(c.get_from(), t_ptr<t_candidate_generator::t_ret_type> (tmp));
        }

        D = gen_results[c.get_from()];
        assert(D.get() != NULL);

        // If ranker's result for this particular connection exists, do not run it
        if(rank_results[i].get() == NULL) {
            std::cout << "Running: " << ranker->get_name() << std::endl;
            auto * tmp = new t_candidate_ranker::t_ret_type();
            (*ranker)(spectrum, *D, *tmp);
            dr->add(i, t_ptr<t_candidate_ranker::t_ret_type> (tmp));
        }

    }
    return dr;
}
}
