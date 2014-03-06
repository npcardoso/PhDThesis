#include "cutoff.h"

namespace diagnosis {
namespace heuristics {
using namespace structs;

t_cutoff::t_cutoff (t_rank_element::t_score cutoff, float lambda) {
    this->cutoff = cutoff;
    this->lambda = lambda;
}

void t_cutoff::operator () (const t_spectra & spectra,
                            t_rank_element * ret,
                            const t_spectra_filter * filter) const {
    t_count component_count = spectra.get_component_count(filter);
    t_count max_component = lambda * component_count;


    max_component = std::min(max_component, component_count);

    for (t_id i = 0; i < max_component; i++) {
        t_rank_element & e = ret[i];

        if (e.get_score() < cutoff) {
            ret[i] = t_rank_element(e.get_component(),
                                    e.get_score(),
                                    t_rank_element::STOP);
            break;
        }
    }
}

std::ostream & t_cutoff::print (std::ostream & out) const {
    return out << "t_cutoff(l:" << lambda << ", v:" << cutoff << ")";
}
}
}