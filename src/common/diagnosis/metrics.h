#ifndef __DIAGNOSIS_METRICS_H__
#define __DIAGNOSIS_METRICS_H__

#include "diagnosis/rank_element.h"
#include "diagnosis/structs/candidate.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>
#include "utils/iostream.h"

namespace diagnosis {
namespace metrics {
typedef double t_diagnosis_cost;
typedef double t_diagnosis_quality;

t_diagnosis_quality quality (t_diagnosis_cost cost, t_count total);


template <class LT>
t_diagnosis_cost Cd_single (const t_component_id & candidate,
                            const LT & candidate_list) {
    typename LT::const_iterator it = candidate_list.begin();
    typename LT::value_type::t_score current_score = NAN;
    t_count total_elements = 0, elements = 0;
    bool quit = false;

    while (it != candidate_list.end()) {
        if (it->get_score() == current_score)
            elements++;
        else {
            if (quit)
                break;

            current_score = it->get_score();
            elements = 1;
        }

        total_elements++;

        if (candidate == it->get_element())
            quit = true;

        it++;
    }

    // Candidate not even considered
    if (!quit)
        return NAN;

    assert(((total_elements - 1) - ((elements - 1) / 2.0)) >= 0);
    return (total_elements - 1) - ((elements - 1) / 2.0);
}

template <class LT>
t_diagnosis_cost Cd_multi (const structs::t_candidate & candidate,
                           const LT & candidate_list) {
    typename LT::const_iterator it = candidate_list.begin();
    typename LT::value_type::t_score current_score = NAN;

    t_count elements = 0, old_size = 0, old_remaining_size = candidate.size();
    structs::t_candidate remaining(candidate), inspected;

    while (it != candidate_list.end()) {
        if (it->get_score() != current_score) {
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