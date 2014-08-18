#include "diagnostic_report.h"

#include "utils/iostream.h"

#include <numeric>
#include <boost/foreach.hpp>

namespace diagnostic {

t_diagnostic_report::t_diagnostic_report(const t_const_ptr<t_diagnostic_system> & diagnostic_system) :
    diagnostic_system(diagnostic_system) {}

t_ranking * t_diagnostic_report::create_ranking(t_connection c) const {
    auto it_score = scores.find(c);
    if(it_score == scores.end())
        return NULL;
    auto it_candidate_set = candidate_sets.find(c.get_from());
    if(it_candidate_set == candidate_sets.end())
        return NULL;
    return new t_ranking(*it_candidate_set->second,
                         *it_score->second,
                         diagnostic_system->get_rankers()[c.get_to()]->get_score_type());
}

void t_diagnostic_report::add(t_id generator_id,
                              t_const_ptr<t_candidate_generator::t_ret_type> D) {
    assert(D.get() != NULL);
    assert(diagnostic_system->get_generators()[generator_id] != NULL);
    assert(candidate_sets.count(generator_id) == false);

    candidate_sets[generator_id] = D;
}

void t_diagnostic_report::add (const t_connection & c,
                               t_const_ptr<t_candidate_ranker::t_ret_type> scores) {
    assert(scores.get() != NULL);
    assert(diagnostic_system->get_generators()[c.get_from()] != NULL);
    assert(diagnostic_system->get_rankers()[c.get_to()] != NULL);
    assert(candidate_sets.count(c.get_from()));
    assert(this->scores.count(c) == 0);

    auto candidate_set = candidate_sets[c.get_from()];
    auto & ranker = *diagnostic_system->get_rankers()[c.get_to()];
    this->scores[c] = scores;
}

std::ostream & t_diagnostic_report::json_write(std::ostream & out) const {
    return out;
}

}


namespace std {
std::ostream & operator << (std::ostream & out,
                            const diagnostic::t_diagnostic_report & dr) {
    return dr.json_write(out);
}
}
