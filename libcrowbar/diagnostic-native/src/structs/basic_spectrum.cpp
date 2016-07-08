#include "basic_spectrum.h"

namespace diagnostic {
t_basic_spectrum::t_basic_spectrum () {
    set_transaction_count(0);
    set_component_count(0);
}

t_basic_spectrum::t_basic_spectrum (t_count component_count,
                                    t_count transaction_count) {
    set_transaction_count(transaction_count);
    this->transaction_count = transaction_count;
    set_component_count(component_count);
}

t_count t_basic_spectrum::get_error_count (const t_spectrum_filter * filter) const {
    // FIXME: Improve performance
    t_count total_errors = 0;

    t_spectrum_iterator it(get_component_count(),
                           get_transaction_count(),
                           filter);


    while (it.transaction.next())
        if (is_error(it.transaction.get()))
            total_errors++;

    return total_errors;
}

t_count t_basic_spectrum::get_component_count (const t_spectrum_filter * filter) const {
    assert(!filter || filter->components.size() <= component_count);

    if (filter)
        return component_count - filter->components.get_filtered_count();

    return component_count;
}

t_count t_basic_spectrum::get_transaction_count (const t_spectrum_filter * filter) const {
    assert(!filter || filter->transactions.size() <= transaction_count);

    if (filter)
        return transaction_count - filter->transactions.get_filtered_count();

    return transaction_count;
}

void t_basic_spectrum::set_count (t_count component_count,
                                  t_count transaction_count) {
    this->component_count = component_count;

    this->transaction_count = transaction_count;

    errors.resize(transaction_count, 0);
    confidences.resize(transaction_count, 1);
}

t_error t_basic_spectrum::get_error (t_transaction_id transaction) const {
    assert(transaction > 0);
    assert(transaction <= transaction_count);

    return errors[transaction - 1];
}

t_confidence t_basic_spectrum::get_confidence (t_transaction_id transaction) const {
    assert(transaction > 0);
    assert(transaction <= transaction_count);

    return confidences[transaction - 1];
}

bool t_basic_spectrum::is_error (t_transaction_id transaction) const {
    assert(transaction > 0);
    assert(transaction <= transaction_count);

    return get_error(transaction) * get_confidence(transaction) >= 1; // TODO: arbitrary threshold
}

void t_basic_spectrum::set_error (t_transaction_id transaction,
                                  t_error error) {
    assert(transaction > 0);
    assert(transaction <= transaction_count);
    assert(error >= 0);
    assert(error <= 1);

    errors[transaction - 1] = error;
}

t_confidence t_basic_spectrum::set_confidence (t_transaction_id transaction,
                                               t_confidence confidence) {
    assert(transaction > 0);
    assert(transaction <= transaction_count);
    assert(confidence >= 0);
    assert(confidence <= 1);

    return confidences[transaction - 1] = confidence;
}
}