#ifndef __INSTRUMENT_H__
#define __INSTRUMENT_H__

#include "preprocessor.h"
#include "types.h"
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Transactions */

#ifdef NINSTR

#define _instr_transaction_start(c_id)
#define _instr_transaction_end(c_id)

#define transaction_start()
#define transaction_end()

#else

void _instr_transaction_start (t_artifact_id c_id);
void _instr_transaction_end (t_artifact_id c_id);

#define instr_transaction_start() _instr_transaction_start(0)
#define instr_transaction_end() _instr_transaction_end(0)

#endif

/* Probes */

#define instr_void (0)

#ifdef NINSTR

#define _instr_probe_observation(c_id, ...)
#define _instr_hit_probe_observation(c_id)

#define instr_pvar(item)
#define instr_probe(...)

#else

void _instr_probe_observation (t_artifact_id c_id, ...);
void _instr_hit_probe_observation (t_artifact_id c_id);


#define instr_pvar(item) sizeof(item), ((void *) &(item))
#define instr_probe(...) _instr_probe_observation(0, __VA_ARGS__, 0, 0)

#endif

/* Oracles */

#ifdef NINSTR

#define _instr_oracle_observation(c_id, error, confidence)

#define instr_oracle(error, confidence)
#define instr_assert_oracle(error, confidence) assert((error))

#else

void _instr_oracle_observation (t_artifact_id c_id,
                                t_error error,
                                t_confidence confidence);

#define instr_oracle(error, confidence) _instr_oracle_observation(0, (error), (confidence))
#define instr_assert_oracle(error, confidence)\
    {\
        instr_oracle((error), (confidence));\
        assert((error))\
    }

#endif

/* Metadata */

#ifdef NINSTR

#define _instr_metadata(c_id, key, val)

#define instr_metadata(key, value)

#else

void _instr_metadata (t_artifact_id c_id,
                      const char * key,
                      const char * val);


#define instr_metadata(key, value) _instr_metadata(0, (key), (value))

#endif


#ifdef __cplusplus
}
#endif
#endif