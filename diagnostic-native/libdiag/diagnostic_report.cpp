#include "diagnostic_report.h"

#include "utils/iostream.h"

#include <numeric>
#include <boost/foreach.hpp>

namespace diagnostic {

t_diagnostic_report::t_diagnostic_report(const t_const_ptr<t_diagnostic_system> & diagnostic_system) :
    diagnostic_system(diagnostic_system),
    gen_results(diagnostic_system->get_generators().size()),
    rank_results(diagnostic_system->get_connections().size()) {
}

t_ranking * t_diagnostic_report::create_ranking(t_id connection_id) const {
    auto & c = diagnostic_system->get_connections()[connection_id];

    if(rank_results[c.get_to()] == NULL)
        return NULL;
    if(gen_results[c.get_from()] == NULL)
        return NULL;
    return new t_ranking(*gen_results[c.get_from()],
                         *rank_results[c.get_to()],
                         diagnostic_system->get_rankers()[c.get_to()]->get_score_type());
}

void t_diagnostic_report::add(t_id generator_id,
                              t_const_ptr<t_candidate_generator::t_ret_type> result) {
    assert(result.get() != NULL);
    assert(generator_id >= 0 );
    assert(generator_id < diagnostic_system->get_generators().size());
    assert(gen_results[generator_id].get() == NULL);

    gen_results[generator_id] = result;
}

void t_diagnostic_report::add (t_id connection_id,
                               t_const_ptr<t_candidate_ranker::t_ret_type> result) {
    assert(result.get() != NULL);
    assert(connection_id >= 0 );
    assert(connection_id < diagnostic_system->get_connections().size());

    t_connection c = diagnostic_system->get_connections()[connection_id];

    assert(gen_results[c.get_from()].get() != NULL);
    assert(rank_results[connection_id].get() == NULL);

    rank_results[connection_id] = result;
}

t_const_ptr<t_diagnostic_system> t_diagnostic_report::get_diagnostic_system() const {
    return diagnostic_system;
}

const t_diagnostic_report::t_gen_results & t_diagnostic_report::get_generator_results () const {
    return gen_results;
}

const t_diagnostic_report::t_rank_results & t_diagnostic_report::get_ranker_results() const {
    return rank_results;
}

}
