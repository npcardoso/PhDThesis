#include "oracle.h"

#include "lib.h"
#include "utils/debug.h"

extern "C"{
t_construct_id _instr_oracle_register(){
  t_construct::t_ptr construct(new t_oracle_construct());
  t_construct_id id = getDataStore()->register_construct(construct);
  releaseDataStore();
  return id;
}
}

void _instr_oracle_observation(t_construct_id c_id,
                               t_health health,
                               t_confidence confidence) {
  t_oracle_observation::t_ptr obs(new t_oracle_observation(getTimeInterval(), c_id, health, confidence));
  getDataStore()->observation(obs);
  releaseDataStore();
}

