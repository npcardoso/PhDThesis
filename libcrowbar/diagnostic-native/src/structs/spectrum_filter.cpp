#include "spectrum_filter.h"

#include <boost/foreach.hpp>

namespace diagnostic {

void t_spectrum_filter::strip (const t_candidate & candidate,
                              const t_spectrum & spectrum) {
    t_spectrum_iterator it(spectrum.get_component_count(),
                          spectrum.get_transaction_count(),
                          this);


    while (it.transaction.next()) {
        BOOST_FOREACH(t_component_id c,
                      candidate) {
            t_transaction_id transaction = it.transaction.get();


            if (spectrum.is_active(c, transaction)) {
                transactions.filter(transaction);
                break;
            }
        }
    }

    BOOST_FOREACH(t_component_id c,
                  candidate) {
        components.filter(c);
    }
}

void t_spectrum_filter::strip (t_component_id component,
                              const t_spectrum & spectrum) {
    t_spectrum_iterator it(spectrum.get_component_count(),
                          spectrum.get_transaction_count(),
                          this);


    while (it.transaction.next()) {
        t_transaction_id transaction = it.transaction.get();
        bool activity = spectrum.is_active(component, transaction);

        if (activity)
            transactions.filter(transaction);
    }

    components.filter(component);
}

t_spectrum_iterator::t_spectrum_iterator (const t_spectrum & spectrum,
                                        const t_spectrum_filter * filter) :
    t_spectrum_iterator(spectrum.get_component_count(),
                       spectrum.get_transaction_count(),
                       filter) {}

t_spectrum_iterator::t_spectrum_iterator (t_count max_components,
                                        t_count max_transactions,
                                        const t_spectrum_filter * filter) : component(max_components, filter ? &filter->components : NULL), transaction(max_transactions, filter ? &filter->transactions : NULL) {}


bool t_spectrum_iterator::next (bool transaction_oriented) {
    if (transaction_oriented) {
        if (!(component.get() || component.next()))
            return false;

        if (!transaction.next())
            return component.next() && transaction.next();
    }
    else {
        if (!(transaction.get() || transaction.next()))
            return false;

        if (!component.next())
            return transaction.next() && component.next();
    }

    return true;
}
}
