#include "similarity.h"

#include "diagnosis/structs/spectra_iterator.h"

#include <cstring>
#include <cmath>

#include <algorithm>

namespace diagnosis {
namespace heuristics {
using namespace structs;


void t_similarity::operator () (const structs::t_spectra & spectra,
                                const structs::t_trie & D,
                                t_ret_type & probs,
                                const structs::t_spectra_filter * filter) const {
    t_trie::iterator it = D.begin();

    t_count n[2][2];


    while (it != D.end()) {
        t_rank_element::t_score score = NAN;

        if (it->size() == 1) {
            t_component_id c = *it->begin();
            score = similarity_coefficient(spectra, c, filter);
        }

        probs.push_back(score);

        it++;
    }
}

void t_similarity::operator () (const t_spectra & spectra,
                                t_rank_element * ret,
                                const t_spectra_filter * filter) const {
    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          filter);

    t_id i = 0;


    while (it.next_component()) {
        t_rank_element::t_score score = similarity_coefficient(spectra,
                                                               it.get_component(),
                                                               filter);
        ret[i++] = t_rank_element(it.get_component(), score);
    }
}

t_rank_element::t_score t_similarity::similarity_coefficient (const structs::t_spectra & spectra,
                                                              t_component_id comp,
                                                              const structs::t_spectra_filter * filter) const {
    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          filter);

    t_count n[2][2];


    memset(n, 0, sizeof(t_count) * 4);

    while (it.next_transaction()) {
        bool activity = spectra.is_active(comp, it.get_transaction());
        bool error = spectra.is_error(it.get_transaction());
        n[activity ? 1 : 0][error ? 1 : 0]++;
    }

    assert(n[0][0] + n[0][1] + n[1][0] + n[1][1] == spectra.get_transaction_count(filter));
    return similarity_coefficient(n);
}

t_rank_element::t_score t_ochiai::similarity_coefficient (const t_count n[2][2]) const {
    t_rank_element::t_score tmp = (n[1][1] + n[0][1]) * (n[1][1] + n[1][0]);


    if (tmp)
        return n[1][1] / sqrt(tmp);

    return 0;
}

std::ostream & t_ochiai::print (std::ostream & out) const {
    return out << "t_ochiai";
}
}
}