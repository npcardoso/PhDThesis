#include "oracle.h"

#include "../lib.h"
#include "../utils/debug.h"


static oracle_id_t num_oracles = 0;

extern "C"{
oracle_id_t _instr_oracle_register(){
  DataStore * ds = getDataStore();
  oracle_id_t o_id = num_oracles++;
  debug("Registering Oracle with o_id %ld", o_id);
  releaseDataStore();
  return o_id;
}

void _instr_oracle_register_location(oracle_id_t o_id, const char * location) {
  DataStore * ds = getDataStore();
  debug("Registering Location of Oracle %ld: '%s'", o_id, location);
  releaseDataStore();
}

void _instr_oracle_register_name(oracle_id_t o_id, const char * name) {
  DataStore * ds = getDataStore();
  debug("Registering Name of Oracle %ld: '%s'", o_id, name);
  releaseDataStore();
}
}

void _instr_oracle_health(oracle_id_t o_id, float health, float confidence) {
  pthread_t id = pthread_self();
  getDataStore()->registerHealth(getTimeInterval(), id,  o_id, health, confidence);
  releaseDataStore();
}

