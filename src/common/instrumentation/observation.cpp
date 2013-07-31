#include "instrumentation/observation.h"

namespace instrumentation {
t_observation_single::t_observation_single () : time(-1), c_id(0) {}

t_observation_single::t_observation_single (t_time_interval time,
                                            t_construct_id c_id) : time(time), c_id(c_id) {}

bool t_observation_single::operator > (const t_time_interval & time) const {
    return this->time > time;
}

bool t_observation_single::operator < (const t_time_interval & time) const {
    return this->time < time;
}

t_observation_window::t_observation_window () {
    this->time_start = -1;
    this->time_end = -1;
    this->c_id_start = 0;
    this->c_id_end = 0;
}

t_observation_window::t_observation_window (t_time_interval time,
                                            t_construct_id c_id) {
    this->time_start = time;
    this->time_end = -1;
    this->c_id_start = c_id;
    this->c_id_end = 0;
}

t_observation_window::t_observation_window (t_time_interval time_start,
                                            t_construct_id c_id_start,
                                            t_time_interval time_end,
                                            t_construct_id c_id_end) {
    this->time_start = time_start;
    this->time_end = time_end;
    this->c_id_start = c_id_start;
    this->c_id_end = c_id_end;
}

bool t_observation_window::ended () const {
    return time_start < time_end || c_id_end;
}

bool t_observation_window::operator > (const t_time_interval & time) const {
    return this->time_start > time;
}

bool t_observation_window::operator < (const t_time_interval & time) const {
    return this->time_end < time;
}
}