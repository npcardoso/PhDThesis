#include "metrics.h"

#include <boost/foreach.hpp>

namespace diagnosis {
namespace benchmark {
void t_Cd::operator () (const structs::t_spectra & spectra,
                        const structs::t_candidate & correct,
                        const t_candidate_generator::t_ret_type & D,
                        const t_candidate_ranker::t_ret_type & probs,
                        const structs::t_diagnosis_report & dr,
                        t_metric::t_ret_type & ret) {
    t_candidate_ranker::t_ret_type::value_type current = NAN;
    t_count total_elements = 0, elements = 0;
    bool quit = false;


    for (t_id i = 1; i <= dr.size(); i++) {
        if (dr.get_probability(i) == current)
            elements++;
        else {
            if (quit)
                break;

            current = dr.get_probability(i);
            elements = 1;
        }

        total_elements++;

        if (correct == dr.get_candidate(i))
            quit = true;
    }

    if (!quit) {
        // Candidate not even considered
        ret["Cd"] = "NAN";
    }
    else {
        assert(((total_elements - 1) - ((elements - 1) / 2.0)) >= 0);
        ret["Cd"] = boost::lexical_cast<std::string> ((total_elements - 1) - ((elements - 1) / 2.0));
    }
}

void t_wasted_effort::operator () (const structs::t_spectra & spectra,
                                   const structs::t_candidate & correct,
                                   const t_candidate_generator::t_ret_type & D,
                                   const t_candidate_ranker::t_ret_type & probs,
                                   const structs::t_diagnosis_report & dr,
                                   t_metric::t_ret_type & ret) {
    t_count elements = 0;
    t_candidate_ranker::t_ret_type::value_type current = NAN;
    structs::t_candidate remaining(correct), healthy_components;


    t_id i = 0;


    while ((i = dr.next(i, healthy_components))) {
        if (!(dr.get_probability(i) == current)) {
            if (remaining.size() == 0)
                break;

            current = dr.get_probability(i);
            elements = 0;
        }

        const structs::t_candidate & candidate = dr.get_candidate(i);

        BOOST_FOREACH(structs::t_candidate::value_type c,
                      candidate) {
            if (remaining.erase(c)) ;
            else if (correct.count(c)) ;
            else if (healthy_components.insert(c).second) elements++;
        }
    }

    if (remaining.size()) {
        // Candidate not even considered
        ret["wasted_effort"] = "NAN";
    }
    else {
        ret["wasted_effort"] = boost::lexical_cast<std::string> (healthy_components.size() - elements / 2.0);
    }
}

void t_entropy::operator () (const structs::t_spectra & spectra,
                             const structs::t_candidate & correct,
                             const t_candidate_generator::t_ret_type & D,
                             const t_candidate_ranker::t_ret_type & probs,
                             const structs::t_diagnosis_report & dr,
                             t_metric::t_ret_type & ret) {
    auto e = dr.get_entropy();


    ret["entropy"] = e.toString();
}
}
}