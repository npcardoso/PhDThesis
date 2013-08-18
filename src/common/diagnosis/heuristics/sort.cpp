#include "sort.h"

#include <algorithm>


namespace diagnosis {
namespace heuristics {
using namespace structs;

void t_sort::operator () (const t_spectra & spectra,
                          t_rank_element * ret,
                          const t_spectra_filter * filter) const {
    t_count component_count = spectra.get_component_count(filter);
    std::sort(ret, ret + component_count);
}

std::ostream & t_sort::print (std::ostream & out) const {
    return out << "t_sort";
}
}
}