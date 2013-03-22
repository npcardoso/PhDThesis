#ifndef __INSTR_TYPES_H__
#define __INSTR_TYPES_H__

#include <pthread.h>
#include <sys/types.h>

/* Time */

typedef long double t_time_interval;

/* Instrumentation Artifacts */

typedef unsigned long t_artifact_id;
typedef t_artifact_id t_probe_id;
typedef t_artifact_id t_variable_id;
typedef t_artifact_id t_transaction_gate_id;
typedef t_artifact_id t_oracle_id;

/* Execution Artifacts */

typedef pthread_t t_thread_id;
typedef t_artifact_id t_observation_id;
typedef t_artifact_id t_transaction_id;

/* Memory */

typedef unsigned long t_alloc_id;

#endif
