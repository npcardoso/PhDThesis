#ifndef __SPECTRA_FILTER_H_e9a80d3bebd598ce4b69b89f95213e4cf28b497e__
#define __SPECTRA_FILTER_H_e9a80d3bebd598ce4b69b89f95213e4cf28b497e__

#include "types.h"
#include "diagnosis/structs/candidate.h"
#include "diagnosis/structs/spectra.h"

#include <cassert>
#include <iostream>
#include <vector>

namespace diagnosis {
namespace structs {
class t_spectra_filter {
    std::vector<t_component_id> f_component;
    std::vector<t_transaction_id> f_transaction;

    t_count filtered_component_count;
    t_count filtered_transaction_count;

public:

    t_spectra_filter ();

    t_transaction_id next_transaction (t_transaction_id transaction) const;
    t_component_id next_component (t_component_id component) const;

    t_transaction_id next_filtered_transaction (t_transaction_id transaction) const;
    t_component_id next_filtered_component (t_component_id component) const;

    t_component_id get_last_component () const;
    t_transaction_id get_last_transaction () const;

    t_component_id get_filtered_component_count () const;
    t_transaction_id get_filtered_transaction_count () const;

    bool is_component (t_component_id component) const;
    bool is_transaction (t_transaction_id transaction) const;

    void filter_all_components (const structs::t_candidate & candidate);
    void filter_all_components_but (const structs::t_candidate & candidate);
    void filter_component (t_component_id component);
    void filter_transaction (t_transaction_id transaction);

    /* Filters all components and transactions in which the components were active */
    void strip (const t_candidate & candidate,
                const t_spectra & spectra);
    void strip (t_component_id component,
                const t_spectra & spectra);


    void unfilter_component (t_component_id component);
    void unfilter_transaction (t_transaction_id transaction);

    void resize_components (t_component_id size);
    void resize_transactions (t_component_id size);
};
}
}
#else
namespace diagnosis {
namespace structs {
class t_spectra_filter;
}
}

#endif