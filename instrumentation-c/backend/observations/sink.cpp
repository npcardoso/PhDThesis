#include "sink.h"

bool t_sink::probe (t_artifact_id id,
                    t_time_interval time) {
    probe_init(id, time);
    probe_commit();
    return true;
}