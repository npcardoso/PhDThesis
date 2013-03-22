#include "datastore/probe.h"

t_probe_observation::t_probe_observation(t_time_interval time, t_probe_id p_id) {
  this->time = time;
  this->p_id = p_id;
  alloc_id = 0;
  state = NULL;
}

void t_probe_observation::read_variable(const void * var, size_t bytes) {
  if(!state)
    state = new t_state();
  state->read_variable(var, bytes);
}

size_t t_probe_observation::size() const {
  if(state)
    return sizeof(t_state) + state->size();
  return 0;
}

t_probe_observation::~t_probe_observation() {
  if(state)
    delete state;
}
