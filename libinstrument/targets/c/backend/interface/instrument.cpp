#include  "instrument.h"

#include "../globals.h"

#include <stdarg.h>
#include <iostream>

extern "C" {
/* Transactions */

t_artifact_id _instr_transaction_register () {
    return database->register_artifact(t_database::TRANSACTION);
}

void _instr_transaction_start (t_artifact_id id) {
    (tracker->get()).transaction_start(id, time_interval());
}

void _instr_transaction_end (t_artifact_id id) {
    (tracker->get()).transaction_end(id, time_interval());
}

/* Probes */

t_artifact_id _instr_probe_register () {
    return database->register_artifact(t_database::PROBE);
}

void _instr_probe_observation (t_artifact_id id, ...) {
    t_sink & sink = tracker->get();
    va_list ap;


    va_start(ap, id);

    sink.probe_init(id, time_interval());

    while (true) {
        size_t bytes = va_arg(ap, size_t);

        if (!bytes)
            break;

        void * ptr = va_arg(ap, void *);
        sink.probe_read_state(ptr, bytes);
    }

    va_end(ap);
    sink.probe_commit();
}

void _instr_hit_probe_observation (t_artifact_id id) {
    (tracker->get()).probe(id, time_interval());
}

/* Oracles */

t_artifact_id _instr_oracle_register () {
    return database->register_artifact(t_database::ORACLE);
}

void _instr_oracle_observation (t_artifact_id id,
                                t_error error,
                                t_confidence confidence) {
    (tracker->get()).oracle(id, time_interval(), error, confidence);
}

/* Metadata */
void _instr_metadata (t_artifact_id id,
                      const char * key,
                      const char * val) {
    database->register_metadata(id, key, val);
}
}