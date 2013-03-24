#include "instrumentation/transaction.h"

size_t t_transaction_observation::size() const {
  return sizeof(t_transaction_observation);
}

void t_transaction_observation::observation(const t_transaction_observation::t_ptr & obs) {
  transactions.push_back(obs);
}

void t_transaction_observation::observation(const t_oracle_observation::t_ptr & obs) {
  oracles.push_back(obs);
}

void t_transaction_observation::observation(const t_probe_observation::t_ptr & obs) {
  probes.push_back(obs);
}
