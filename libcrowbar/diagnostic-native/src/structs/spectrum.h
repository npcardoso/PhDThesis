#ifndef __SPECTRUM_H_14c9fb7ba96542f0d3780a3886d24ff0ff4ff469__
#define __SPECTRUM_H_14c9fb7ba96542f0d3780a3886d24ff0ff4ff469__

#include "spectrum_filter.h"
#include "../types.h"
#include "../utils/boost.h"

#include <cassert>
#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace diagnostic {
class t_spectrum {
public:
    typedef std::set<t_transaction_id> t_invalid_transactions;
    inline virtual ~t_spectrum () {}
    // Gets

    virtual t_count get_error_count (const t_spectrum_filter * filter=NULL) const = 0;
    virtual t_count get_component_count (const t_spectrum_filter * filter=NULL) const = 0;
    virtual t_count get_transaction_count (const t_spectrum_filter * filter=NULL) const = 0;

    virtual t_count get_activations (t_component_id component,
                                     t_transaction_id transaction) const = 0;

    virtual t_error get_error (t_transaction_id transaction) const = 0;

    virtual t_confidence get_confidence (t_transaction_id transaction) const;

    virtual bool get_invalid (t_invalid_transactions & ret,
                              const t_spectrum_filter * filter=NULL) const;

    virtual void get_minimal_conflicts (t_spectrum_filter & f) const;
    virtual t_ptr<t_spectrum_filter> get_minimal_conflicts (const t_spectrum_filter * filter=NULL) const;

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

    virtual bool is_candidate (const t_candidate & candidate,
                               const t_spectrum_filter * filter=NULL) const;

    virtual bool is_minimal_candidate (const t_candidate & candidate,
                                       const t_spectrum_filter * filter=NULL) const;


    virtual bool is_invalid (const t_spectrum_filter * filter=NULL) const;

    virtual bool is_all_pass (const t_spectrum_filter * filter=NULL) const;
    // IO

    virtual std::ostream & print (std::ostream & out,
                                  const t_spectrum_filter * filter=NULL) const;

    virtual std::ostream & write (std::ostream & out,
                                  const t_spectrum_filter * filter=NULL) const;

    virtual std::istream & read (std::istream & in);

    // Metrics

    virtual t_probability get_activation_rate (const t_spectrum_filter * filter=NULL) const;
    virtual t_probability get_error_rate (const t_spectrum_filter * filter=NULL) const;


    virtual t_count get_suspicious_components_count (const t_spectrum_filter * filter=NULL) const; // Number of components involved in failing transactions

    virtual t_count get_suspicious_components_count (t_candidate & suspicious,
                                                     const t_spectrum_filter * filter=NULL) const;

    // Cast

    template <class T>
    T & requires ();

    template <class T>
    const T & requires () const;
};


template <class T>
T & t_spectrum::requires () {
    T * tmp = dynamic_cast<T *> (this);


    if (tmp)
        return *tmp;

    throw;
}

template <class T>
const T & t_spectrum::requires () const {
    T * tmp = dynamic_cast<T *> (this);


    if (tmp)
        return *tmp;

    throw;
}
}

namespace std {
std::istream & operator >> (std::istream & in,
                            diagnostic::t_spectrum & spectrum);
std::ostream & operator << (std::ostream & out,
                            const diagnostic::t_spectrum & spectrum);
}

#else
namespace diagnostic {
class t_spectrum;
}
#endif