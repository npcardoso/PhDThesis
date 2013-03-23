#include "probe.h"

#include "lib.h"
#include "utils/debug.h"

extern "C"{
t_construct_id _instr_probe_register(){
  t_construct_id id = 0;
  return id;
}
}

void _instr_probe_observation(t_construct_id c_id, ...){
  va_list ap;
  va_start(ap, c_id);
  t_transaction_factory * datastore = getDataStore(); 
  t_probe_observation::t_ptr obs(new t_probe_observation(getTimeInterval(), c_id));

  while (true) {
    size_t bytes = va_arg (ap, size_t);
    if(!bytes)
      break;
    void * ptr = va_arg(ap, void *);
    obs->read_variable(ptr, bytes);
  }
  va_end(ap);
  datastore->observation(obs);
  releaseDataStore();
}

void _instr_hit_probe_observation(t_construct_id c_id) {
  debug("Hitprobe: %ld", c_id);
  t_probe_observation::t_ptr obs(new t_probe_observation(getTimeInterval(), c_id));
  getDataStore()->observation(obs);
  releaseDataStore();
}
