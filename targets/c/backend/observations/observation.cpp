#include "observation.h"

t_observation_single::t_observation_single () : id(0), time(-1) {}

t_observation_single::t_observation_single (t_artifact_id id,
                                            t_time_interval time) : id(id), time(time) {}

bool t_observation_single::operator > (const t_time_interval & time) const {
    return this->time > time;
}

bool t_observation_single::operator < (const t_time_interval & time) const {
    return this->time < time;
}

t_observation_window::t_observation_window () {
    this->time_start = -1;
    this->time_end = -1;
    this->id_start = 0;
    this->id_end = 0;
}

t_observation_window::t_observation_window (t_artifact_id id,
                                            t_time_interval time) {
    this->time_start = time;
    this->time_end = -1;
    this->id_start = id;
    this->id_end = 0;
}

t_observation_window::t_observation_window (t_artifact_id id_start,
                                            t_time_interval time_start,
                                            t_artifact_id id_end,
                                            t_time_interval time_end) {
    this->time_start = time_start;
    this->time_end = time_end;
    this->id_start = id_start;
    this->id_end = id_end;
}

bool t_observation_window::ended () const {
    return time_start < time_end || id_end;
}

bool t_observation_window::operator > (const t_time_interval & time) const {
    return this->time_start > time;
}

bool t_observation_window::operator < (const t_time_interval & time) const {
    return this->time_end < time;
}