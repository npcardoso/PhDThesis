#ifndef __SPECTRA_H_14c9fb7ba96542f0d3780a3886d24ff0ff4ff469__
#define __SPECTRA_H_14c9fb7ba96542f0d3780a3886d24ff0ff4ff469__

#include "types.h"
#include "diagnosis/types.h"
#include "diagnosis/structs/candidate.h"
#include "diagnosis/structs/spectra_filter.h"
#include "diagnosis/structs/spectra_iterator.h"


#include "exceptions.h"

#include <cassert>
#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace diagnosis {
namespace structs {
class t_spectra {
public:
    typedef std::set<t_transaction_id> t_invalid_transactions;


    template <class G>
    void probability (const structs::t_candidate & candidate,
                      const G & goodnesses,
                      t_probability_mp & ret,
                      const t_spectra_filter * filter=NULL,
                      bool use_confidence=true,
                      bool use_fuzzy_error=true) const;

    inline virtual ~t_spectra () {}
    // Gets

    virtual t_count get_error_count (const t_spectra_filter * filter=NULL) const = 0;
    virtual t_count get_component_count (const t_spectra_filter * filter=NULL) const = 0;
    virtual t_count get_transaction_count (const t_spectra_filter * filter=NULL) const = 0;

    virtual t_count get_activations (t_component_id component,
                                     t_transaction_id transaction) const = 0;

    virtual t_error get_error (t_transaction_id transaction) const = 0;

    virtual t_confidence get_confidence (t_transaction_id transaction) const;

    virtual bool get_invalid (t_invalid_transactions & ret,
                              const t_spectra_filter * filter=NULL) const;

    // Sets

    virtual void set_count (t_count component_count,
                            t_count transaction_count) = 0;

    virtual void set_component_count (t_count component_count);

    virtual void set_transaction_count (t_count transaction_count);

    virtual void set_error (t_transaction_id transaction,
                            t_error error) = 0;

    virtual t_confidence set_confidence (t_transaction_id transaction,
                                         t_confidence confidence);

    // Checks

    virtual bool is_active (t_component_id component,
                            t_transaction_id transaction) const;

    virtual bool is_error (t_transaction_id transaction) const = 0;

    virtual bool is_candidate (const structs::t_candidate & candidate,
                               const t_spectra_filter * filter=NULL) const;

    virtual bool is_invalid (const t_spectra_filter * filter=NULL) const;

    // IO

    virtual std::ostream & print (std::ostream & out,
                                  const t_spectra_filter * filter=NULL) const;

    virtual std::ostream & write (std::ostream & out,
                                  const t_spectra_filter * filter=NULL) const;

    virtual std::istream & read (std::istream & in);

    // Metrics

    virtual t_probability get_activation_rate (const t_spectra_filter * filter=NULL) const;
    virtual t_probability get_error_rate (const t_spectra_filter * filter=NULL) const;

    // Cast

    template <class T>
    T & requires ();

    template <class T>
    const T & requires () const;
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

    // Gets

    virtual t_count get_error_count (const t_spectra_filter * filter=NULL) const;
    virtual t_count get_component_count (const t_spectra_filter * filter=NULL) const;
    virtual t_count get_transaction_count (const t_spectra_filter * filter=NULL) const;
    virtual t_error get_error (t_transaction_id transaction) const;
    virtual t_confidence get_confidence (t_transaction_id transaction) const;

    // Sets

    virtual void set_count (t_count component_count,
                            t_count transaction_count);
    virtual void set_error (t_transaction_id transaction,
                            t_error error);
    virtual t_confidence set_confidence (t_transaction_id transaction,
                                         t_confidence confidence);
    // Checks

    virtual bool is_error (t_transaction_id transaction) const;
};

template <class G>
void t_spectra::probability (const structs::t_candidate & candidate,
                             const G & goodnesses,
                             t_probability_mp & ret,
                             const t_spectra_filter * filter,
                             bool use_confidence,
                             bool use_fuzzy_error) const {
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
            t_count count = get_activations(*c_it, it.get_transaction());

            assert(goodnesses[comp] >= 0);
            assert(goodnesses[comp] <= 1);

            if (count)
                tmp *= pow(goodnesses[comp], count);

            c_it++;
            comp++;
        }

        // Fuzzy health
        t_error e = use_fuzzy_error ? get_error(it.get_transaction()) : is_error(it.get_transaction());


        tmp = e * (1 - tmp) + (1 - e) * tmp;

        // Confidence scaling
        if (use_confidence) {
            t_confidence c = get_confidence(it.get_transaction());
            tmp = (1 - c) + (c * tmp);
        }

        ret *= tmp;
    }

    assert(ret >= 0);
    assert(ret <= 1);
}

template <class T>
T & t_spectra::requires () {
    T * tmp = dynamic_cast<T *> (this);


    if (tmp)
        return *tmp;

    throw e_not_supported();
}

template <class T>
const T & t_spectra::requires () const {
    T * tmp = dynamic_cast<T *> (this);


    if (tmp)
        return *tmp;

    throw e_not_supported();
}
}
}
#endif