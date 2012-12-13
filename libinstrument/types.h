#ifndef __INSTR_TYPES_H__
#define __INSTR_TYPES_H__

#include <pthread.h>
#include <sys/types.h>

/* Time */

typedef long double time_interval_t;

/* Instrumentation Artifacts */

typedef unsigned long artifact_id_t;
typedef artifact_id_t probe_id_t;
typedef artifact_id_t variable_id_t;
typedef artifact_id_t transaction_gate_id_t;
typedef artifact_id_t oracle_id_t;

/* Execution Artifacts */

typedef pthread_t thread_id_t;
typedef artifact_id_t observation_id_t;
typedef artifact_id_t transaction_id_t;

/* Memory */

typedef unsigned long alloc_id_t;

#endif
