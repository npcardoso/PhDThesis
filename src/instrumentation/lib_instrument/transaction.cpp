#include "instrumentation/transaction.h"

#include <boost/foreach.hpp>
namespace instrumentation {
void t_transaction_observation::observation (const t_ptr<t_transaction_observation> & obs) {
    transactions.push_back(obs);
}

void t_transaction_observation::observation (const t_ptr<t_oracle_observation> & obs) {
    oracles.push_back(obs);
}

void t_transaction_observation::observation (const t_ptr<t_probe_observation> & obs) {
    probes.push_back(obs);
}

void t_transaction_observation::flatten () {
    BOOST_FOREACH(auto & tr, transactions)
    tr->flatten(probes, oracles);
    transactions.clear();
}

void t_transaction_observation::flatten (t_probes & probes,
                                         t_oracles & oracles) {
    probes.splice(probes.end(), probes);
    oracles.splice(oracles.end(), oracles);

    BOOST_FOREACH(auto & tr, transactions) {
        tr->flatten(probes, oracles);
    }
}
}