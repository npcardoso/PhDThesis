#include "verbose.h"

t_verbose::t_verbose (std::ostream & out) : out(out) {}

bool t_verbose::transaction_start (t_artifact_id id,
                                   t_time_interval time) {
    out << "* transaction_start (" << id << ", " << time << ")" << std::endl;
    return true;
}

bool t_verbose::transaction_end (t_artifact_id id,
                                 t_time_interval time) {
    out << "* transaction_end (" << id << ", " << time << ")" << std::endl;
    return true;
}

// Probe with state
bool t_verbose::probe_init (t_artifact_id id,
                            t_time_interval time) {
    out << "** probe_init (" << id << ", " << time << ")" << std::endl;
    return true;
}

bool t_verbose::probe_read_state (void * ptr,
                                  t_count size) {
    out << "*** probe_read_state(" << size << ")" << std::endl; // TODO: print data
    return true;
}

bool t_verbose::probe_commit () {
    out << "** probe_commit ()" << std::endl;
    return true;
}

bool t_verbose::oracle (t_artifact_id id,
                        t_time_interval time,
                        t_error error,
                        t_confidence confidence) {
    out << "** oracle (" << id << ", " << time << ", " << error << ", " << confidence << ")" << std::endl;
    return true;
}