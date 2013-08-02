#include "spectra.h"

namespace diagnosis {
bool t_spectra::is_candidate (const t_candidate & candidate,
                              const t_spectra_filter * filter) const {
    t_spectra_iterator it(get_component_count(),
                          get_transaction_count(),
                          filter);


    while (it.next_transaction()) {
        bool hit = false;

        if (!is_error(it.get_transaction()))
            continue;

        t_candidate::iterator candidate_it = candidate.begin();

        while (candidate_it != candidate.end())
            if (get_count(*(candidate_it++), it.get_transaction())) {
                hit = true;
                break;
            }

        if (!hit)
            return false;
    }

    return true;
}

std::ostream & t_spectra::write (std::ostream & out,
                                 const t_spectra_filter * filter) const {
    throw e_not_implemented();
}

std::istream & t_spectra::read (std::istream & in) {
    throw e_not_implemented();
}

std::istream & operator >> (std::istream & in, t_spectra & spectra) {
    return spectra.read(in);
}

std::ostream & operator << (std::ostream & out, const t_spectra & spectra) {
    return spectra.write(out);
}

t_basic_spectra::t_basic_spectra () {
    set_transaction_count(0);
    set_component_count(0);
}

t_basic_spectra::t_basic_spectra (t_count component_count,
                                  t_count transaction_count) {
    set_transaction_count(transaction_count);
    this->transaction_count = transaction_count;
    set_component_count(component_count);
}

t_count t_basic_spectra::get_error_count (const t_spectra_filter * filter) const {
    // FIXME: Improve performance
    t_count total_errors = 0;


    if (filter) {
        for (t_id i = 1; i <= get_transaction_count(); i++)
            if (is_error(i) && !filter->is_transaction(i))
                total_errors++;
    }
    else {
        for (t_id i = 1; i <= get_transaction_count(); i++)
            if (is_error(i))
                total_errors++;
    }

    return total_errors;
}

t_count t_basic_spectra::get_component_count (const t_spectra_filter * filter) const {
    assert(!filter || filter->get_last_component() <= component_count);

    if (filter)
        return component_count - filter->get_filtered_component_count();

    return component_count;
}

t_count t_basic_spectra::get_transaction_count (const t_spectra_filter * filter) const {
    assert(!filter || filter->get_last_transaction() <= transaction_count);

    if (filter)
        return transaction_count - filter->get_filtered_transaction_count();

    return transaction_count;
}

void t_basic_spectra::set_element_count (t_count component_count,
                                         t_count transaction_count) {
    this->component_count = component_count;

    this->transaction_count = transaction_count;

    errors.resize(transaction_count, false);
}

void t_basic_spectra::set_component_count (t_count component_count) {
    set_element_count(component_count, get_transaction_count());
}

void t_basic_spectra::set_transaction_count (t_count transaction_count) {
    set_element_count(get_component_count(), transaction_count);
}

t_error t_basic_spectra::get_error (t_transaction_id transaction) const {
    assert(transaction > 0);
    assert(transaction <= transaction_count);

    return errors[transaction - 1];
}

bool t_basic_spectra::is_error (t_transaction_id transaction) const {
    assert(transaction > 0);
    assert(transaction <= transaction_count);

    return get_error(transaction) > 0.5; // TODO: arbitrary threshold
}

void t_basic_spectra::error (t_transaction_id transaction,
                             t_error set) {
    assert(transaction > 0);
    assert(transaction <= transaction_count);

    errors[transaction - 1] = set;
}
}