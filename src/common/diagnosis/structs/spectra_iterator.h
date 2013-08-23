#ifndef __SPECTRA_ITERATOR_H_98ba46a077f9ffe96964f4d39a96b1042d80379c__
#define __SPECTRA_ITERATOR_H_98ba46a077f9ffe96964f4d39a96b1042d80379c__

#include "diagnosis/structs/spectra_filter.h"
#include "types.h"

#include <cstdlib>

namespace diagnosis {
namespace structs {
class t_spectra_iterator {
private:
    t_count max_components;
    t_count max_transactions;

    t_transaction_id transaction;
    t_component_id component;

    const t_spectra_filter * filter;

    t_component_id _next_component (t_component_id component) const;
    t_transaction_id _next_transaction (t_transaction_id transaction) const;

public:
    t_spectra_iterator (t_count max_components,
                        t_count max_transactions,
                        const t_spectra_filter * filter=NULL);


    t_component_id next_component (bool wrap=false);
    t_transaction_id next_transaction (bool wrap=false);

    bool next (bool transaction_oriented=false);

    inline t_transaction_id get_transaction () const {
        return transaction;
    }

    inline t_component_id get_component () const {
        return component;
    }

    void set_transaction (t_transaction_id transaction);

    void set_component (t_component_id component);
};
}
}

#endif