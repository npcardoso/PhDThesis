#ifndef __BUG_H__
#define __BUG_H__

#include <pthread.h>
#include <time.h>

typedef struct {
    // Parameters
    size_t min;
    size_t max;
    size_t total;

    // Counters
    size_t leakage_total;
    size_t leakage_count;

    time_t start_time;
    // Lock
    pthread_mutex_t lock;
} leak_t;

extern pthread_mutex_t leak_init_lock;


#define leak(min, max, total)\
    {\
        static leak_t * l;\
        leak_shared(l, min, max, total);\
    }

#define leak_shared(l, min, max, total)\
    {\
        pthread_mutex_lock(&leak_init_lock);\
        _leak_init(&(l), min, max, total);\
        pthread_mutex_unlock(&leak_init_lock);\
\
        pthread_mutex_lock(&(l->lock));\
\
        time_t t = time(NULL) - l->start_time;\
        instr_probe(instr_pvar(l->leakage_count),\
                    instr_pvar(t));\
        instr_metadata("Name", "Launcher loop");\
        instr_metadata("Name", "Bug");\
        _leak(l);\
        pthread_mutex_unlock(&(l->lock));\
    }

void _leak (leak_t * l);
void _leak_init (leak_t ** l, size_t min, size_t max, size_t total);

#endif