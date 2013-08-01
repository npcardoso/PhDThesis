#ifndef __CONVERTERS_OBSERVATION_SPECTRA_H__
#define __CONVERTERS_OBSERVATION_SPECTRA_H__

#include "instrumentation/transaction.h"
#include "diagnosis/spectra/count_spectra.h"

namespace converters {

class t_observations_to_count_spectra {
public:

  t_health threshold;
  bool ignore_unknown_components;

  inline t_observations_to_count_spectra() {
    threshold = 0.5;
    ignore_unknown_components = false;
  }

  void operator ()(const instrumentation::t_transaction_observation & tr,
                   diagnosis::t_count_spectra<t_count> & spectra);
};

}
#endif
