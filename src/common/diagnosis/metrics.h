#ifndef __DIAGNOSIS_METRICS_H__
#define __DIAGNOSIS_METRICS_H__

#include "diagnosis/rank_element.h"
#include "diagnosis/structs/spectra.h"
#include "diagnosis/structs/spectra_filter.h"

#include <cmath>
#include <vector>

namespace diagnosis {
namespace metrics {
typedef double t_diagnosis_cost;
typedef double t_diagnosis_quality;

template <class T, class LT>
t_diagnosis_cost Cd (const T & candidate,
                     const LT & candidate_list);

t_diagnosis_quality quality (t_diagnosis_cost cost, t_count total);


template <class T, class LT>
t_diagnosis_cost Cd (const T & candidate,
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

    return (total_elements - 1) - ((elements - 1) / 2.0);
}
}
}

#endif