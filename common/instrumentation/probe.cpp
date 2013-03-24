#include "instrumentation/probe.h"

#include <cassert>
#include <cstdlib>
#include <cstring>

t_state::t_state() {
  n_vars = 0;
  data = NULL;
  offset_end = NULL;
}

void t_state::read_variable(const void * var, size_t bytes) {
  assert(bytes > 0);

  size_t start;

  if(n_vars) {
    start = offset_end[n_vars - 1];
    offset_end = (size_t *) realloc(offset_end, (n_vars + 1) * sizeof(size_t));
    data = (unsigned char *) realloc(data, start + bytes);
  }
  else {
    start = 0;
    offset_end = (size_t *) malloc(sizeof(size_t));
    data = (unsigned char *) malloc(bytes);
  }
  assert(offset_end != NULL);
  assert(data != NULL);

  offset_end[n_vars] = start + bytes;
  memcpy(data + start, var, bytes);

  n_vars++;
}

t_state::~t_state(){
  if(data)
    free(data);
  if(offset_end)
    free(offset_end);
}


t_probe_observation::t_probe_observation(t_time_interval time, t_construct_id c_id):t_observation_single(time, c_id) {
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
