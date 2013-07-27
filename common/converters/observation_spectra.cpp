#include "converters/observation_spectra.h"

#include <boost/foreach.hpp>

using namespace instrumentation;
using namespace diagnosis;

namespace converters {

void t_observations_to_count_spectra::operator ()(const t_transaction_observation & tr,
                                                  t_count_spectra & spectra) {


  t_transaction_id tid = spectra.new_transaction();

  t_health health = 1;

  if (tr.oracles.size()) {
    health = 0;

    BOOST_FOREACH(t_oracle_observation::t_ptr o, 
                  tr.oracles)
      health += o->health * o->confidence;
    health /= tr.oracles.size();
  }
  spectra.error(tid, health < threshold);

  BOOST_FOREACH(t_probe_observation::t_ptr p, 
                tr.probes) {
    spectra.hit(p->c_id, tid, 1, ignore_unknown_components);
  }
}

}
