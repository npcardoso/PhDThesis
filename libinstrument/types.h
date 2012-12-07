#ifndef __INSTR_TYPES_H__
#define __INSTR_TYPES_H__

#include <pthread.h>
#include <sys/types.h>

typedef long double time_interval_t;


/* Instrumentation Artifacts */

typedef unsigned long probe_id_t;
typedef unsigned long variable_id_t;
typedef unsigned long transaction_gate_id_t;
typedef unsigned long oracle_id_t;

/* Execution Artifacts */

typedef pthread_t thread_id_t;
typedef unsigned long observation_id_t;
typedef unsigned long transaction_id_t;

/* Memory */

typedef unsigned long alloc_id_t;
#endif
