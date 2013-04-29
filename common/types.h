#ifndef __INSTR_TYPES_H__
#define __INSTR_TYPES_H__

#include <pthread.h>
#include <sys/types.h>

/* Time */

typedef long double t_time_interval;

typedef unsigned long t_id;
typedef t_id t_construct_id;
typedef t_id t_observation_id;

typedef pthread_t t_thread_id;

typedef double t_health;
typedef double t_confidence;
typedef unsigned short t_port_no;

typedef size_t t_count;
#endif
