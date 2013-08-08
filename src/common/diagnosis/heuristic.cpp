#include "heuristic.h"

namespace diagnosis {
std::ostream & t_heuristic_filter::print (std::ostream & out) const {
    return out << "Not Implemented";
}

std::ostream & operator << (std::ostream & out, const t_heuristic_filter & filter) {
    return filter.print(out);
}

void t_heuristic::push (const t_filter_ptr & filter) {
    filters.push_back(filter);
}

void t_heuristic::push (const t_heuristic_filter * filter) {
    filters.push_back(t_filter_ptr(filter));
}

void t_heuristic::operator () (const t_spectra & spectra,
                               t_rank_element * ret,
                               const t_spectra_filter * filter) const {
    assert(ret != NULL);

    if (!spectra.get_transaction_count(filter))
        return;

    if (filters.size()) {
        t_filters::const_iterator it = filters.begin();
        ret[0] = t_rank_element(0, 0);

        while (it != filters.end())
            (**(it++))(spectra, ret, filter);
    }
    else {
        t_spectra_iterator it(spectra.get_component_count(),
                              spectra.get_transaction_count(),
                              filter);
        t_id i = 0;

        while (it.next_component())
            ret[i++] = t_rank_element(it.get_component(), 1);
    }
}

std::ostream & t_heuristic::print (std::ostream & out) const {
    t_filters::const_iterator it = filters.begin();


    out << "t_heuristic(";

    if (it != filters.end())
        out << **(it++);

    while (it != filters.end())
        out << ", " << **(it++);

    return out << ")";
}

t_heuristic::t_order_buffer t_heuristic::get_ordering_buffer (const t_spectra & spectra,
                                                              const t_spectra_filter * filter) {
    return t_order_buffer(new t_rank_element[spectra.get_component_count(filter) + 1]);
}
}
std::ostream & std::operator << (std::ostream & out, const diagnosis::t_heuristic & heuristic) {
    return heuristic.print(out);
}