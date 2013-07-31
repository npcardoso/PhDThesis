#include "instrumentation/transaction.h"

#include <boost/foreach.hpp>
namespace instrumentation {
void t_transaction_observation::observation (const t_transaction_observation::t_ptr & obs) {
    transactions.push_back(obs);
}

void t_transaction_observation::observation (const t_oracle_observation::t_ptr & obs) {
    oracles.push_back(obs);
}

void t_transaction_observation::observation (const t_probe_observation::t_ptr & obs) {
    probes.push_back(obs);
}

void t_transaction_observation::flatten () {
    BOOST_FOREACH(t_transaction_observation::t_ptr tr,
                  transactions)
    tr->flatten(probes, oracles);
    transactions.clear();
}

void t_transaction_observation::flatten (t_probes probes,
                                         t_oracles & oracles) {
    probes.splice(probes.end(), this->probes);
    oracles.splice(oracles.end(), this->oracles);

    BOOST_FOREACH(t_transaction_observation::t_ptr tr,
                  this->transactions)
    tr->flatten(probes, oracles);
}
}