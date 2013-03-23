#include "transaction.h"

#include "lib.h"
#include "utils/debug.h"

extern "C"{
t_construct_id _instr_transaction_register(){
  t_construct_id id = 0;
  return id;
}
}

void _instr_transaction_start(t_construct_id c_id) {
  t_transaction_observation::t_ptr obs(new t_transaction_observation(getTimeInterval(), c_id));
  getDataStore()->observation(obs);
  releaseDataStore();
}

void _instr_transaction_end(t_construct_id c_id) {
  pthread_t id = pthread_self();
  getDataStore()->transaction_end(getTimeInterval(), c_id);
  releaseDataStore();
}
