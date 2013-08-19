#ifndef __METRICS_H_d89493d817d7ef64219d0d614f7ef3b07eb21297__
#define __METRICS_H_d89493d817d7ef64219d0d614f7ef3b07eb21297__

#include "diagnosis/diagnosis_system.h"
#include "diagnosis/structs/candidate.h"
#include "diagnosis/structs/diagnosis_report.h"
#include "utils/boost.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>
#include "utils/iostream.h"

namespace diagnosis {
namespace benchmark {
class t_metric {
public:
    DEFINE_BOOST_SHARED_PTRS(t_metric);
    typedef std::map<std::string, std::string> t_ret_type;

    virtual void operator () (const structs::t_spectra & spectra,
                              const structs::t_candidate & correct,
                              const t_candidate_generator::t_ret_type & D,
                              const t_candidate_ranker::t_ret_type & probs,
                              const structs::t_diagnosis_report & dr,
                              t_ret_type & ret) = 0;
};

class t_Cd : public t_metric {
public:
    virtual void operator () (const structs::t_spectra & spectra,
                              const structs::t_candidate & correct,
                              const t_candidate_generator::t_ret_type & D,
                              const t_candidate_ranker::t_ret_type & probs,
                              const structs::t_diagnosis_report & dr,
                              t_ret_type & ret);
};


template <class LT>
double Cd_multi (const structs::t_candidate & candidate,
                 const LT & candidate_list) {
    typename LT::const_iterator it = candidate_list.begin();
    typename LT::value_type::t_score current_score = NAN;

    t_count elements = 0, old_size = 0, old_remaining_size = candidate.size();
    structs::t_candidate remaining(candidate), inspected;

    while (it != candidate_list.end()) {
        if (!(it->get_score() == current_score)) {
            if (remaining.size() == 0)
                break;

            current_score = it->get_score();
            elements = 0;
        }


        std::vector<t_component_id> v(std::min(remaining.size(), it->get_element()->size()));
        std::vector<t_component_id>::iterator tmp_it = std::set_intersection(remaining.begin(), remaining.end(),
                                                                             it->get_element()->begin(), it->get_element()->end(),
                                                                             v.begin());

        // Removing components from remaining
        old_remaining_size = remaining.size();

        for (tmp_it = v.begin(); tmp_it != v.end(); tmp_it++)
            remaining.erase(*tmp_it);

        // Adding components to inspected
        old_size = inspected.size();

        inspected.insert(it->get_element()->begin(), it->get_element()->end());

        elements += inspected.size() - old_size + (remaining.size() - old_remaining_size);
        it++;
    }

    // Candidate not even considered
    if (remaining.size())
        return NAN;


    assert((inspected.size() - candidate.size()) - (elements / 2.0) >= 0);

    return (inspected.size() - candidate.size()) - (elements / 2.0);
}
}
}

#endif