#include "instrumentation/probe.h"

#include <cassert>
#include <cstdlib>
#include <cstring>

namespace instrumentation {
t_probe_observation::t_probe_observation () : t_observation_single() {
    state = NULL;
}

t_probe_observation::t_probe_observation (t_time_interval time, t_construct_id c_id) : t_observation_single(time, c_id) {
    state = NULL;
}

t_probe_observation::t_probe_observation (const t_probe_observation & p) : t_observation_single(p) {
    if (p.state)
        state = new t_state(*p.state);
    else
        state = NULL;
}

void t_probe_observation::read_variable (const void * var, size_t bytes) {
    if (!state)
        state = new t_state();

    state->read(var, bytes);
}

void t_probe_observation::clear_state () {
    if (state) {
        delete state;
        state = NULL;
    }
}

t_probe_observation::~t_probe_observation () {
    clear_state();
}

t_probe_observation & t_probe_observation::operator = (const t_probe_observation & p) {
    clear_state();
    memcpy((void*) this, (void*) &p, sizeof(t_probe_observation));

    if (state)
        state = new t_state(*state);

    return *this;
}
}