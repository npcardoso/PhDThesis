#include "transaction.h"

#include "lib.h"
#include "utils/debug.h"

extern "C"{
t_transaction_gate_id _instr_transaction_gate_register(){
  t_transaction_gate_id id = getDataStore()->register_transaction_construct();
  releaseDataStore();
  return id;
}
}

void _instr_transaction_gate_register_metadata(t_probe_id id,
                                               const char * key, 
                                               const char * val) {
  getDataStore()->register_transaction_metadata(id, key, val);
  releaseDataStore();
}

void _instr_transaction_start(t_transaction_gate_id tr_id,
                              char explicit_end) {
  pthread_t id = pthread_self();
  getDataStore()->register_transaction(getTimeInterval(), id, tr_id, explicit_end);
  releaseDataStore();
}

void _instr_transaction_end(t_transaction_gate_id tr_id) {
  pthread_t id = pthread_self();
  getDataStore()->register_transaction_end(getTimeInterval(), id, tr_id);
  releaseDataStore();
}
