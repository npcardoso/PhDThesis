#ifndef __INSTR_PTHREAD_H__
#define __INSTR_PTHREAD_H__

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif
void _instr_pthread_register_main(pthread_t thread);
#ifdef __cplusplus
}
#endif
#endif
