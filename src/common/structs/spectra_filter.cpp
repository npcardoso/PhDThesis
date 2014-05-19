#include "spectra_filter.h"

#include <boost/foreach.hpp>

namespace diagnosis {
namespace structs {
void t_spectra_filter::strip (const t_candidate & candidate,
                              const t_spectra & spectra) {
    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          this);


    while (it.transaction.next()) {
        BOOST_FOREACH(t_component_id c,
                      candidate) {
            t_transaction_id transaction = it.transaction.get();


            if (spectra.is_active(c, transaction)) {
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

void t_spectra_filter::strip (t_component_id component,
                              const t_spectra & spectra) {
    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          this);


    while (it.transaction.next()) {
        t_transaction_id transaction = it.transaction.get();
        bool activity = spectra.is_active(component, transaction);

        if (activity)
            transactions.filter(transaction);
    }

    components.filter(component);
}

t_spectra_iterator::t_spectra_iterator (t_count max_components,
                                        t_count max_transactions,
                                        const t_spectra_filter * filter) : component(max_components, filter ? &filter->components : NULL), transaction(max_transactions, filter ? &filter->transactions : NULL) {}

bool t_spectra_iterator::next (bool transaction_oriented) {
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
}