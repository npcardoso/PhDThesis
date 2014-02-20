#ifndef __OBSERVATION_SPECTRA_H_2fe05f10ddb0cc3677c4a9014ab1d2bc0016f010__
#define __OBSERVATION_SPECTRA_H_2fe05f10ddb0cc3677c4a9014ab1d2bc0016f010__

#include "instrumentation/transaction.h"
#include "diagnosis/structs/count_spectra.h"

namespace converters {
class t_observations_to_count_spectra {
public:

    t_health threshold;
    bool ignore_unknown_components;

    inline t_observations_to_count_spectra () {
        threshold = 0.5;
        ignore_unknown_components = false;
    }

    void operator () (const instrumentation::t_transaction_observation & tr,
                      diagnosis::structs::t_count_spectra & spectra);
};
}

#endif