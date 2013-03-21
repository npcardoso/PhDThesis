#include "datastore/observation.h"

Observation::Observation(time_interval_t time, probe_id_t p_id) {
  this->time = time;
  this->p_id = p_id;
  alloc_id = 0;
  state = NULL;
}

void Observation::readVariable(const void * var, size_t bytes) {
  if(!state)
    state = new State();
  state->readVariable(var, bytes);
}

size_t Observation::size() const {
  if(state)
    return sizeof(State) + state->size();
  return 0;
}

Observation::~Observation() {
  if(state)
    delete state;
}
