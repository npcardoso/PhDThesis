#include  "instrument.h"

#include "../globals.h"

#include <stdarg.h>

extern "C" {
/* Transactions */

    t_artifact_id _instr_transaction_register () {
        //t_transaction_artifact::t_ptr artifact(new t_transaction_artifact());
        //register_artifact(artifact);
        return 123; //artifact->id;
    }

    void _instr_transaction_start (t_artifact_id id) {
        (tracker->get()).transaction_start(id, time_interval());
    }

    void _instr_transaction_end (t_artifact_id id) {
        (tracker->get()).transaction_end(id, time_interval());
    }

/* Probes */

    t_artifact_id _instr_probe_register () {
        //t_probe_artifact::t_ptr artifact(new t_probe_artifact());
        //register_artifact(artifact);
        return 124; //artifact->id;
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

            void * ptr = va_arg(ap, void*);
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
        //t_oracle_artifact::t_ptr artifact(new t_oracle_artifact());
        //register_artifact(artifact);
        return 125; //artifact->id;
    }

    void _instr_oracle_observation (t_artifact_id id,
                                    t_error error,
                                    t_confidence confidence) {
        (tracker->get()).oracle(id, time_interval(), error, confidence);
    }

    void _instr_metadata (t_artifact_id id,
                          const char * key,
                          const char * val) {}


}
