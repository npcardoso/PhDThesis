#include "oracle.h"

#include "lib.h"
#include "utils/debug.h"

extern "C"{
t_oracle_id _instr_oracle_register(){
  t_oracle_id id =  getDataStore()->register_oracle_construct();
  releaseDataStore();
  return id;
}
}

void _instr_oracle_register_metadata(t_oracle_id id,
                                     const char * key,
                                     const char * val) {
  getDataStore()->register_oracle_metadata(id, key, val);
  releaseDataStore();
}

void _instr_oracle_health(t_oracle_id o_id, float health, float confidence) {
  pthread_t id = pthread_self();
  getDataStore()->register_health(getTimeInterval(), id,  o_id, health, confidence);
  releaseDataStore();
}

