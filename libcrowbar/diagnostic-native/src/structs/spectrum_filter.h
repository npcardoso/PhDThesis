#ifndef __SPECTRUM_FILTER_H_e9a80d3bebd598ce4b69b89f95213e4cf28b497e__
#define __SPECTRUM_FILTER_H_e9a80d3bebd598ce4b69b89f95213e4cf28b497e__

#include "filter.h"
#include "spectrum.h"

#include <cassert>
#include <iostream>
#include <vector>

namespace diagnostic {

class t_spectrum_filter {
public:
    /* Filters all components and transactions in which the components were active */
    void strip (const t_candidate & candidate,
                const t_spectrum & spectrum);
    void strip (t_component_id component,
                const t_spectrum & spectrum);


public:
    t_filter components;
    t_filter transactions;
};

class t_spectrum_iterator {
public:
    t_spectrum_iterator (const t_spectrum & spectrum,
                        const t_spectrum_filter * filter=NULL);

    t_spectrum_iterator (t_count max_components,
                        t_count max_transactions,
                        const t_spectrum_filter * filter=NULL);


    bool next (bool transaction_oriented=false);

public:

    t_filter_iterator component;
    t_filter_iterator transaction;
};
}

#else

namespace diagnostic {
class t_spectrum_filter;
}

#endif
