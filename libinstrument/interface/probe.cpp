#include "probe.h"

#include "../lib.h"
#include "../utils/debug.h"

extern "C"{
probe_id_t _instr_probe_register(){
  probe_id_t id =  getDataStore()->registerProbe();
  releaseDataStore();
  return id;
}

void _instr_probe_register_location(probe_id_t id, 
                                    const char * val) {
  getDataStore()->registerProbeMetadata(id, "Location", val);
  releaseDataStore();
}
}

void _instr_probe_register_metadata(probe_id_t id, 
                                    const char * key, 
                                    const char * val) {
  getDataStore()->registerProbeMetadata(id, key, val);
  releaseDataStore();
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
