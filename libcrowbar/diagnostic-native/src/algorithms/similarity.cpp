#include "similarity.h"

#include <boost/foreach.hpp>

namespace diagnostic {
namespace algorithms {
void t_similarity::operator () (const t_spectrum & spectrum,
                                const t_trie & D,
                                t_ret_type & probs,
                                const t_spectrum_filter * filter) const {
    BOOST_FOREACH(auto & d,
                  D) {
        t_score score = NAN;


        if (d.size() == 1) {
            t_component_id c = *d.begin();
            score = (*this)(spectrum, c, filter);
        }

        probs.push_back(score);
    }
}

t_ptr<t_single_component_ranking> t_similarity::operator () (const t_spectrum & spectrum,
                                                             const t_spectrum_filter * filter) const {
    t_spectrum_iterator it(spectrum.get_component_count(),
                           spectrum.get_transaction_count(),
                           filter);


    t_ptr<t_single_component_ranking> rank(new t_single_component_ranking(spectrum.get_component_count(filter)));
    t_id i = 0;

    while (it.component.next()) {
        t_score score = (*this)(spectrum,
                                it.component.get(),
                                filter);

        rank->set(i++, score, it.component.get());
    }

    return rank;
}

t_score t_similarity::operator () (const t_spectrum & spectrum,
                                   t_component_id comp,
                                   const t_spectrum_filter * filter) const {
    t_spectrum_iterator it(spectrum.get_component_count(),
                           spectrum.get_transaction_count(),
                           filter);

    t_count n[2][2];


    memset(n, 0, sizeof(t_count) * 4);

    while (it.transaction.next()) {
        bool activity = spectrum.is_active(comp, it.transaction.get());
        bool error = spectrum.is_error(it.transaction.get());
        n[activity ? 1 : 0][error ? 1 : 0]++;
    }

    assert(n[0][0] + n[0][1] + n[1][0] + n[1][1] == spectrum.get_transaction_count(filter));
    return similarity_coefficient(n);
}

void t_similarity::json_configs (t_configs & out) const {
    out["type"] = get_similarity_type();
}
}
}