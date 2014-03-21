#include "spectra_iterator.h"

#include <cassert>

namespace diagnosis {
namespace structs {
t_spectra_iterator::t_spectra_iterator (t_count max_components,
                                        t_count max_transactions,
                                        const t_spectra_filter * filter) {
    this->max_components = max_components;
    this->max_transactions = max_transactions;
    this->filter = filter;
    component = 0;
    transaction = 0;
}

t_component_id t_spectra_iterator::_next_component (t_component_id component) const {
    t_component_id next = 0;


    if (filter == NULL)
        next = component + 1;
    else
        next = filter->next_component(component);

    if (next > max_components)
        return 0;

    return next;
}

t_transaction_id t_spectra_iterator::_next_transaction (t_transaction_id transaction) const {
    t_transaction_id next = 0;


    if (filter == NULL)
        next = transaction + 1;
    else
        next = filter->next_transaction(transaction);

    if (next > max_transactions)
        return 0;

    return next;
}

t_component_id t_spectra_iterator::next_component (bool wrap) {
    component = _next_component(component);

    if (!component && wrap)
        transaction = _next_transaction(transaction);

    assert(component <= max_components);
    assert(transaction <= max_transactions);
    return component;
}

t_transaction_id t_spectra_iterator::next_transaction (bool wrap) {
    transaction = _next_transaction(transaction);

    if (!transaction && wrap)
        component = _next_component(component);

    assert(component <= max_components);
    assert(transaction <= max_transactions);
    return transaction;
}

bool t_spectra_iterator::next (bool transaction_oriented) {
    if (transaction_oriented) {
        if (!(transaction || next_transaction()))
            return false;

        if (!next_component())
            return next_transaction() && next_component();
    }
    else {
        if (!(component || next_component()))
            return false;

        if (!next_transaction())
            return next_component() && next_transaction();
    }

    return true;
}

void t_spectra_iterator::set_transaction (t_transaction_id transaction) {
    assert(transaction <= max_transactions);
    assert(!filter || !transaction || !filter->is_transaction(transaction));
    this->transaction = transaction;
}

void t_spectra_iterator::set_component (t_component_id component) {
    assert(component <= max_components);
    assert(!filter || !component || !filter->is_component(component));
    this->component = component;
}
}
}