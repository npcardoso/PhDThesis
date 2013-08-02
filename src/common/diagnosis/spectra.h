#ifndef __DIAGNOSIS_SPECTRA_H__
#define __DIAGNOSIS_SPECTRA_H__

#include "diagnosis/candidate.h"
#include "diagnosis/spectra_filter.h"
#include "diagnosis/spectra_iterator.h"

#include "utils/mpreal.h"

#include "exceptions.h"
#include "types.h"

#include <cassert>
#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace diagnosis {
typedef mpfr::mpreal t_probability_mp;
typedef t_probability_mp t_goodness_mp;

class t_spectra {
public:

    virtual t_count get_error_count (const t_spectra_filter * filter=NULL) const = 0;
    virtual t_count get_transaction_count (const t_spectra_filter * filter=NULL) const = 0;
    virtual t_count get_component_count (const t_spectra_filter * filter=NULL) const = 0;

    virtual t_count get_count (t_component_id component,
                               t_transaction_id transaction) const = 0;

    template <class G>
    void probability (const t_candidate & candidate,
                      const G & goodnesses,
                      t_probability_mp & ret,
                      const t_spectra_filter * filter=NULL) const;

    virtual bool is_error (t_transaction_id transaction) const = 0;
    virtual t_error get_error (t_transaction_id transaction) const = 0;

    virtual bool is_candidate (const t_candidate & candidate,
                               const t_spectra_filter * filter=NULL) const;

    virtual std::ostream& write (std::ostream & out,
                                 const t_spectra_filter * filter=NULL) const;

    virtual std::istream& read (std::istream & in);
};

std::istream & operator >> (std::istream & in, t_spectra & spectra);
std::ostream & operator << (std::ostream & out, const t_spectra & spectra);

class t_basic_spectra : public t_spectra {
    typedef std::vector<t_error> t_errors;
    t_errors errors;
    t_count component_count;
    t_count transaction_count;


public:

    t_basic_spectra ();

    t_basic_spectra (t_count component_count,
                     t_count transaction_count);

    virtual t_count get_error_count (const t_spectra_filter * filter=NULL) const;
    virtual t_count get_component_count (const t_spectra_filter * filter=NULL) const;
    virtual t_count get_transaction_count (const t_spectra_filter * filter=NULL) const;

    virtual void set_element_count (t_count component_count,
                                    t_count transaction_count);
    virtual void set_component_count (t_count component_count);
    virtual void set_transaction_count (t_count transaction_count);


    virtual t_error get_error (t_transaction_id transaction) const;
    virtual bool is_error (t_transaction_id transaction) const;
    virtual void error (t_transaction_id transaction,
                        t_error set=1);
};

template <class G>
void t_spectra::probability (const t_candidate & candidate,
                             const G & goodnesses,
                             t_probability_mp & ret,
                             const t_spectra_filter * filter) const {
    assert(candidate.size() > 0);

    t_goodness_mp tmp(goodnesses[0]);

    ret = 1;

    t_spectra_iterator it(get_component_count(),
                          get_transaction_count(),
                          filter);

    while (it.next_transaction()) {
        tmp = 1;
        t_candidate::const_iterator c_it = candidate.begin();
        t_id c = 0;

        while (c_it != candidate.end()) {
            t_count count = get_count(*c_it, it.get_transaction());

            if (count)
                tmp *= count * goodnesses[c];

            c_it++;
            c++;
        }

        if (is_error(it.get_transaction()))
            tmp = 1 - tmp;

        ret *= tmp;
    }
}
}

#endif