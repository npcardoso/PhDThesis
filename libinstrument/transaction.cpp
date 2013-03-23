#include "transaction.h"

#include "main.h"
#include "utils/debug.h"
#include "utils/time.h"

extern "C"{
t_construct_id _instr_transaction_register(){
  t_construct::t_ptr construct(new t_transaction_construct());
  t_construct_id id = tracker()->get()->register_construct(construct);
  return id;
}
}

void _instr_transaction_start(t_construct_id c_id) {
  t_transaction_observation::t_ptr obs(new t_transaction_observation(time_interval(), c_id));
  tracker()->get()->observation(obs);
}

void _instr_transaction_end(t_construct_id c_id) {
  pthread_t id = pthread_self();
  tracker()->get()->transaction_end(time_interval(), c_id);
}
