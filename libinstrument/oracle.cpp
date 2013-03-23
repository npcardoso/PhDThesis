#include "oracle.h"

#include "lib.h"
#include "utils/debug.h"

extern "C"{
t_construct_id _instr_oracle_register(){
  t_construct_id id = 0; /* TODO */
  return id;
}
}

void _instr_oracle_register_metadata(t_construct_id id,
                                     const char * key,
                                     const char * val) {
//  getDataStore()->register_oracle_metadata(id, key, val);
//  releaseDataStore();
}

void _instr_oracle_observation(t_construct_id c_id,
                               float health,
                               float confidence) {
  t_oracle_observation::t_ptr obs(new t_oracle_observation(getTimeInterval(), c_id, health, confidence));
  getDataStore()->observation(obs);
  releaseDataStore();
}

