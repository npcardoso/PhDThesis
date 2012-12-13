#include "transaction.h"

#include "../lib.h"
#include "../utils/debug.h"

extern "C"{
transaction_gate_id_t _instr_transaction_gate_register(){
  transaction_gate_id_t id = getDataStore()->registerTransactionGate();
  releaseDataStore();
  return id;
}

void _instr_transaction_gate_register_location(transaction_gate_id_t id, 
                                               const char * val) {
  getDataStore()->registerTransactionGateMetadata(id, "Location", val);
  releaseDataStore();
}
}

void _instr_transaction_gate_register_metadata(probe_id_t id,
                                               const char * key, 
                                               const char * val) {
  getDataStore()->registerTransactionGateMetadata(id, key, val);
  releaseDataStore();
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
