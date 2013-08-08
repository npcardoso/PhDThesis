#ifndef __DIAGNOSIS_SPECTRA_H__
#define __DIAGNOSIS_SPECTRA_H__

#include "diagnosis/structs/candidate.h"
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
    void probability (const structs::t_candidate & candidate,
                      const G & goodnesses,
                      t_probability_mp & ret,
                      const t_spectra_filter * filter=NULL) const;

    virtual bool is_error (t_transaction_id transaction) const = 0;
    virtual t_error get_error (t_transaction_id transaction) const = 0;

    virtual t_confidence get_confidence (t_transaction_id transaction) const = 0;

    virtual bool is_candidate (const structs::t_candidate & candidate,
                               const t_spectra_filter * filter=NULL) const;

    virtual bool is_valid (const t_spectra_filter * filter=NULL) const;

    virtual std::ostream & write (std::ostream & out,
                                  const t_spectra_filter * filter=NULL) const;

    virtual std::istream & read (std::istream & in);
};

std::istream & operator >> (std::istream & in, t_spectra & spectra);
std::ostream & operator << (std::ostream & out, const t_spectra & spectra);

class t_basic_spectra : public t_spectra {
    typedef std::vector<t_confidence> t_confidences;
    typedef std::vector<t_error> t_errors;
    t_confidences confidences;
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
    virtual void set_error (t_transaction_id transaction,
                            t_error error=1);

    virtual t_confidence get_confidence (t_transaction_id transaction) const;
    virtual t_confidence set_confidence (t_transaction_id transaction,
                                         t_confidence confidence);
};

template <class G>
void t_spectra::probability (const structs::t_candidate & candidate,
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
        structs::t_candidate::const_iterator c_it = candidate.begin();
        t_id comp = 0;

        while (c_it != candidate.end()) {
            t_count count = get_count(*c_it, it.get_transaction());

            if (count)
                tmp *= count * goodnesses[comp];

            c_it++;
            comp++;
        }

        // Fuzzy health
        t_error e = get_error(it.get_transaction());
        tmp = e * (1 - tmp) + (1 - e) * tmp;

        // Confidence scaling
        t_confidence c = get_confidence(it.get_transaction());
        tmp = (1 - c) + (c * tmp);

        ret *= tmp;
    }
}
}

#endif