#include "metrics.h"

#include <boost/foreach.hpp>

using namespace boost;
using namespace std;

namespace diagnostic {
namespace benchmark {
string t_Cd::__KEY__("Cd");
string t_wasted_effort::__KEY__("wasted_effort");
string t_entropy::__KEY__("entropy");
string t_quality::__KEY__("_quality");
string t_quality_fair::__KEY__("_quality_fair");

const string & t_Cd::key () const {
    return __KEY__;
}

const string & t_wasted_effort::key () const {
    return __KEY__;
}

const string & t_entropy::key () const {
    return __KEY__;
}

const string & t_quality::key () const {
    return result_metric;
}

const string & t_quality_fair::key () const {
    return result_metric;
}

string t_Cd::operator () (const t_spectra & spectra,
                          const t_candidate & correct,
                          const t_candidate_generator::t_ret_type & D,
                          const t_candidate_ranker::t_ret_type & probs,
                          const t_ranking & r,
                          const t_metric::t_arguments & arguments) const {
    t_candidate_ranker::t_ret_type::value_type current = NAN;
    t_count total_elements = 0, elements = 0;
    bool quit = false;


    BOOST_FOREACH(auto & el, r.get_elements()) {
        if (el.get_score() == current)
            elements++;
        else {
            if (quit)
                break;

            current = el.get_score();
            elements = 1;
        }

        total_elements++;

        if (correct == el.get_candidate())
            quit = true;
    }

    if (!quit) // Candidate not even considered
        return "NaN";

    assert(((total_elements - 1) - ((elements - 1) / 2.0)) >= 0);
    return lexical_cast<string> ((total_elements - 1) - ((elements - 1) / 2.0));
}



template <class InputIterator1, class InputIterator2>
bool intersects (InputIterator1 first1, InputIterator1 last1,
                 InputIterator2 first2, InputIterator2 last2) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) ++first1;
        else if (*first2 < *first1) ++first2;
        else
            return true;
    }

    return false;
}

t_id next_unhealhy (t_id current,
                    const t_ranking & r,
                    const t_candidate & healthy_comps) {
    auto & rank = r.get_elements();
    while (current++ < rank.size()) {
        auto & candidate = rank[current].get_candidate();
        if (!intersects(healthy_comps.begin(), healthy_comps.end(),
                        candidate.begin(), candidate.end()))
            return current;
    }

    return 0;
}


string t_wasted_effort::operator () (const t_spectra & spectra,
                                     const t_candidate & correct,
                                     const t_candidate_generator::t_ret_type & D,
                                     const t_candidate_ranker::t_ret_type & probs,
                                     const t_ranking & r,
                                     const t_metric::t_arguments & arguments) const {
    t_count elements = 0;
    t_candidate_ranker::t_ret_type::value_type current = NAN;
    t_candidate remaining(correct), healthy_components;
    auto & rank = r.get_elements();


    t_id i = 0;


    while ((i = next_unhealhy(i, r, healthy_components))) {
        if (!(rank[i].get_score() == current)) {
            if (remaining.size() == 0)
                break;

            current = rank[i].get_score ();
            elements = 0;
        }

        const t_candidate & candidate = rank[i].get_candidate();

        BOOST_FOREACH(t_candidate::value_type c,
                      candidate) {
            if (remaining.erase(c)) ;
            else if (correct.count(c)) ;
            else if (healthy_components.insert(c).second) elements++;
        }
    }

    if (remaining.size())
        // Candidate not even considered
        return "NaN";

    return lexical_cast<string> (healthy_components.size() - elements / 2.0);
}

string t_entropy::operator () (const t_spectra & spectra,
                               const t_candidate & correct,
                               const t_candidate_generator::t_ret_type & D,
                               const t_candidate_ranker::t_ret_type & probs,
                               const t_ranking & r,
                               const t_metric::t_arguments & arguments) const {
    return lexical_cast<string> (r.get_entropy());
}

t_quality::t_quality (string target_metric) : target_metric(target_metric), result_metric(target_metric + __KEY__) {}

string t_quality::operator () (const t_spectra & spectra,
                               const t_candidate & correct,
                               const t_candidate_generator::t_ret_type & D,
                               const t_candidate_ranker::t_ret_type & probs,
                               const t_ranking & r,
                               const t_metric::t_arguments & arguments) const {
    double metric_value = get_argument<double> (target_metric, arguments);
    t_count remaining_components = (spectra.get_component_count() - correct.size());


    if (remaining_components)
        return lexical_cast<string> (1 - (metric_value / remaining_components));

    return lexical_cast<string> (1);
}

t_quality_fair::t_quality_fair (string target_metric) : target_metric(target_metric), result_metric(target_metric + __KEY__) {}

string t_quality_fair::operator () (const t_spectra & spectra,
                                    const t_candidate & correct,
                                    const t_candidate_generator::t_ret_type & D,
                                    const t_candidate_ranker::t_ret_type & probs,
                                    const t_ranking & r,
                                    const t_metric::t_arguments & arguments) const {
    double metric_value = get_argument<double> (target_metric, arguments);
    t_count remaining_components = (spectra.get_suspicious_components_count() - correct.size());


    if (remaining_components)
        return lexical_cast<string> (1 - (metric_value / remaining_components));

    return lexical_cast<string> (1);
}
}
}
