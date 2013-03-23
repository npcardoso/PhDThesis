#ifndef __INSTR_TYPES_H__
#define __INSTR_TYPES_H__

#include <pthread.h>
#include <sys/types.h>

/* Time */

typedef long double t_time_interval;

typedef unsigned long t_id;

/* Instrumentation constructs */

typedef t_id t_construct_id;
typedef t_construct_id t_probe_id;
typedef t_construct_id t_variable_id;
typedef t_construct_id t_transaction_id;
typedef t_construct_id t_oracle_id;

/* Execution constructs */

typedef pthread_t t_thread_id;
typedef t_id t_observation_id;

typedef double t_health;
typedef double t_confidence;

#endif
