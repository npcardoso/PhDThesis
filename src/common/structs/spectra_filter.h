#ifndef __SPECTRA_FILTER_H_e9a80d3bebd598ce4b69b89f95213e4cf28b497e__
#define __SPECTRA_FILTER_H_e9a80d3bebd598ce4b69b89f95213e4cf28b497e__

#include "types.h"
#include "structs/candidate.h"
#include "structs/filter.h"
#include "structs/spectra.h"

#include <cassert>
#include <iostream>
#include <vector>

namespace diagnosis {
namespace structs {
class t_spectra_filter {
public:
    /* Filters all components and transactions in which the components were active */
    void strip (const t_candidate & candidate,
                const t_spectra & spectra);
    void strip (t_component_id component,
                const t_spectra & spectra);


public:
    t_filter components;
    t_filter transactions;
};

class t_spectra_iterator {
public:
    t_spectra_iterator (t_count max_components,
                        t_count max_transactions,
                        const t_spectra_filter * filter=NULL);


    bool next (bool transaction_oriented=false);

public:

    t_filter_iterator component;
    t_filter_iterator transaction;
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