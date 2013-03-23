#include "probe.h"

#include "lib.h"
#include "utils/debug.h"

extern "C"{
t_construct_id _instr_probe_register(){
  t_construct_id id = 0;
  return id;
}
}

void _instr_probe_observation(t_construct_id p_id, ...){
  debug("Probe OBS");
  va_list ap;
  va_start(ap, p_id);

  while (true) {
    size_t size = va_arg (ap, size_t);
    debug("Variable with size %ld", size);
    if(!size)
      break;
    void * ptr = va_arg(ap, void *);
  }
  debug("EndVariables");
  va_end(ap);
}

/*void _instr_probe_observation_register(t_probe_id p_id){
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
}*/
void _instr_hit_probe_observation(t_construct_id c_id) {

}
