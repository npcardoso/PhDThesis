#include "cutoff.h"

namespace diagnosis {
namespace heuristics {
using namespace structs;

t_cutoff::t_cutoff (t_rank_element::t_score value_cutoff, float lambda) {
    this->value_cutoff = value_cutoff;
    this->lambda = lambda;
}

void t_cutoff::operator () (const t_spectra & spectra,
                            t_rank_element * ret,
                            const t_spectra_filter * filter) const {
    t_count component_count = spectra.get_component_count(filter);
    t_count max_component = lambda * component_count;


    max_component = std::min(max_component, component_count);

    t_id i = 0;

    while (i < max_component) {
        t_rank_element::t_score value = ret[i].get_score();

        if (value >= 0 && value < value_cutoff)
            ret[i] = t_rank_element(0, -1);

        i++;
    }

    while (i < component_count)
        ret[i++] = t_rank_element(0, -1);
}

std::ostream & t_cutoff::print (std::ostream & out) const {
    return out << "t_cutoff(l:" << lambda << ", v:" << value_cutoff << ")";
}
}
}