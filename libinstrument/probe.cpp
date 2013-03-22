#include "probe.h"

#include "lib.h"
#include "utils/debug.h"

extern "C"{
t_probe_id _instr_probe_register(){
  t_probe_id id =  getDataStore()->register_probe_construct();
  releaseDataStore();
  return id;
}
}

void _instr_probe_register_metadata(t_probe_id id, 
                                    const char * key, 
                                    const char * val) {
  getDataStore()->register_probe_metadata(id, key, val);
  releaseDataStore();
}

void _instr_probe_observation_register_atomic(t_probe_id p_id){
  t_datastore * ds = getDataStore();
  pthread_t id = pthread_self();
  ds->start_probe(getTimeInterval(), id, p_id);
}

void _instr_probe_observation_register(t_probe_id p_id){
  t_datastore * ds = getDataStore();
  pthread_t id = pthread_self();
  ds->start_probe(getTimeInterval(), id, p_id);
  releaseDataStore();
}

void _instr_probe_read(void * ptr, size_t width){
  t_datastore * ds = getDataStore();
  pthread_t id = pthread_self();
  ds->read_variable(id, ptr, width);
  releaseDataStore();
}

void _instr_probe_observation_commit(){
  t_datastore * ds = getDataStore();
  pthread_t id = pthread_self();
  ds->commit_observation(id);
  releaseDataStore();
}
