#include "probe.h"

#include "../lib.h"
#include "../utils/debug.h"


static probe_id_t num_probes = 0;

extern "C"{
probe_id_t _instr_probe_register(){
  DataStore * ds = getDataStore();
  probe_id_t p_id = num_probes++;
  debug("Registering Probe with p_id %ld", p_id);
  releaseDataStore();
  return p_id;
}

void _instr_probe_register_location(probe_id_t p_id, const char * location) {
  DataStore * ds = getDataStore();
  debug("Registering Location of Probe %ld: '%s'", p_id, location);
  releaseDataStore();
}

void _instr_probe_register_name(probe_id_t p_id, const char * name) {
  DataStore * ds = getDataStore();
  debug("Registering Name of Probe %ld: '%s'", p_id, name);
  releaseDataStore();
}
}

void _instr_probe_observation_register_atomic(probe_id_t p_id){
  DataStore * ds = getDataStore();
  pthread_t id = pthread_self();
  ds->registerObservation(getTimeInterval(), id, p_id);
}

void _instr_probe_observation_register(probe_id_t p_id){
  DataStore * ds = getDataStore();
  pthread_t id = pthread_self();
  ds->registerObservation(getTimeInterval(), id, p_id);
  releaseDataStore();
}

void _instr_probe_read(void * ptr, size_t width){
  DataStore * ds = getDataStore();
  pthread_t id = pthread_self();
  ds->readVariable(id, ptr, width);
  releaseDataStore();
}

void _instr_probe_observation_commit(){
  DataStore * ds = getDataStore();
  pthread_t id = pthread_self();
  ds->commitObservation(id);
  releaseDataStore();
}
