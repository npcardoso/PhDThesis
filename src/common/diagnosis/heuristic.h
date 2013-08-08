#ifndef __DIAGNOSIS_HEURISTIC_H__
#define __DIAGNOSIS_HEURISTIC_H__

#include "diagnosis/rank_element.h"
#include "diagnosis/spectra.h"
#include "diagnosis/spectra_filter.h"

#include <vector>

namespace diagnosis {
class t_heuristic_filter {
public:
    virtual void operator () (const t_spectra & spectra,
                              t_rank_element * ret,
                              const t_spectra_filter * filter=NULL) const = 0;
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

    void operator () (const t_spectra & spectra,
                      t_rank_element * ret,
                      const t_spectra_filter * filter=NULL) const;

    virtual std::ostream & print (std::ostream & out) const;

    static t_order_buffer get_ordering_buffer (const t_spectra & spectra,
                                               const t_spectra_filter * filter=NULL);

    inline bool operator == (const t_heuristic & h) const {return filters == h.filters;}
    inline bool operator != (const t_heuristic & h) const {return filters != h.filters;}

private:
    t_filters filters;
};
}

std::ostream & operator << (std::ostream & out, const diagnosis::t_heuristic & heuristic);

#endif