#include "datastore/transaction.h"

size_t t_transaction_observation::size() const {
  return sizeof(t_transaction_observation);
}
