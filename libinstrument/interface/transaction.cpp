#include "transaction.h"

#include "../lib.h"
#include "../utils/debug.h"

static transaction_gate_id_t num_transaction_gates = 0;

extern "C"{
transaction_gate_id_t _instr_transaction_gate_register(){
  DataStore * ds = getDataStore();
  transaction_gate_id_t tg_id = num_transaction_gates++;
  debug("Registering Transaction Gate with tg_id %ld", tg_id);
  releaseDataStore();
  return tg_id;
}

void _instr_transaction_gate_register_location(transaction_gate_id_t tg_id, const char * location) {
  DataStore * ds = getDataStore();
  debug("Registering Location of Transaction Gate %ld: '%s'", tg_id, location);
  releaseDataStore();
}

void _instr_transaction_gate_register_name(transaction_gate_id_t tg_id, const char * name) {
  DataStore * ds = getDataStore();
  debug("Registering Name of Transaction Gate %ld: '%s'", tg_id, name);
  releaseDataStore();
}
}

void _instr_transaction_start(transaction_gate_id_t tr_id,
                              char explicit_end) {
  pthread_t id = pthread_self();
  getDataStore()->registerTransaction(getTimeInterval(), id, tr_id, explicit_end);
  releaseDataStore();
}

void _instr_transaction_end(transaction_gate_id_t tr_id) {
  pthread_t id = pthread_self();
  getDataStore()->registerTransactionEnd(getTimeInterval(), id, tr_id);
  releaseDataStore();
}
