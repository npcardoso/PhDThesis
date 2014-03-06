#ifndef __HEURISTIC_H_8de70ac6c5818c96d1805646e0bfa46dd7650ed3__
#define __HEURISTIC_H_8de70ac6c5818c96d1805646e0bfa46dd7650ed3__

#include "diagnosis/heuristics/rank_element.h"
#include "diagnosis/structs/spectra.h"
#include "diagnosis/structs/spectra_filter.h"

#include <vector>

namespace diagnosis {
namespace heuristics {
typedef double t_score;
typedef diagnosis::t_rank_element<t_component_id, t_score> t_rank_element;

class t_heuristic_filter {
public:
    virtual void operator () (const structs::t_spectra & spectra,
                              t_rank_element * ret,
                              const structs::t_spectra_filter * filter=NULL) const = 0;
    virtual std::ostream & print (std::ostream & out) const;
};

std::ostream & operator << (std::ostream & out, const t_heuristic_filter & filter);

class t_heuristic {
public:
    typedef boost::shared_ptr<const t_heuristic_filter> t_filter_ptr;
    typedef std::vector<t_filter_ptr> t_filters;
    typedef boost::shared_ptr<t_rank_element[]> t_order_buffer;


    void push (const t_filter_ptr & filter);

    void push (const t_heuristic_filter * filter);

    void operator () (const structs::t_spectra & spectra,
                      t_rank_element * ret,
                      const structs::t_spectra_filter * filter=NULL) const;

    virtual std::ostream & print (std::ostream & out) const;

    static t_order_buffer get_ordering_buffer (const structs::t_spectra & spectra,
                                               const structs::t_spectra_filter * filter=NULL);

    inline bool operator == (const t_heuristic & h) const {return filters == h.filters;}
    inline bool operator != (const t_heuristic & h) const {return filters != h.filters;}

private:
    t_filters filters;
};
}
}

namespace std {
std::ostream & operator << (std::ostream & out, const diagnosis::heuristics::t_heuristic & heuristic);
}

#endif