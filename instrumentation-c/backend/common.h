#ifndef __COMMON_H_61113bd982d0e5ed5370ae9cb6cc9274d014e949__
#define __COMMON_H_61113bd982d0e5ed5370ae9cb6cc9274d014e949__


#include <pthread.h>

/* Environment dependent stuff */
typedef pthread_t t_thread_id;

inline t_thread_id thread_self () {
    return pthread_self();
}

#endif