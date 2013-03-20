#include "oracle.h"

#include "lib.h"
#include "utils/debug.h"

extern "C"{
oracle_id_t _instr_oracle_register(){
  oracle_id_t id = 0;
  return id;
}
}

void _instr_oracle_register_metadata(oracle_id_t id,
                                     const char * key,
                                     const char * val) {
//  getDataStore()->registerOracleMetadata(id, key, val);
//  releaseDataStore();
}

void _instr_oracle_health(oracle_id_t o_id, float health, float confidence) {
//  pthread_t id = pthread_self();
//  getDataStore()->registerHealth(getTimeInterval(), id,  o_id, health, confidence);
//  releaseDataStore();
}

