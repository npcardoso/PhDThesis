#include "probe.h"

#include "main.h"
#include "utils/debug.h"
#include "utils/time.h"

extern "C"{
t_construct_id _instr_probe_register(){
  t_construct::t_ptr construct(new t_probe_construct());
  t_construct_id id = tracker()->get()->register_construct(construct);
  return id;
}
}

void _instr_probe_observation(t_construct_id c_id, ...){
  va_list ap;
  va_start(ap, c_id);
  t_probe_observation::t_ptr obs(new t_probe_observation(time_interval(), c_id));

  while (true) {
    size_t bytes = va_arg (ap, size_t);
    if(!bytes)
      break;
    void * ptr = va_arg(ap, void *);
    obs->read_variable(ptr, bytes);
  }
  va_end(ap);
  tracker()->get()->observation(obs);
}

void _instr_hit_probe_observation(t_construct_id c_id) {
  debug("Hitprobe: %ld", c_id);
  t_probe_observation::t_ptr obs(new t_probe_observation(time_interval(), c_id));
  tracker()->get()->observation(obs);
}
