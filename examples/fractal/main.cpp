#include "thread.h"

#include <pthread.h>

#define MAX_THREADS 3
#define LEVELS 4

int main() {
  pthread_t thread[2];
  pthread_create(thread, NULL,  threadify, new threadify_args_t(2, 4, false, false));
//pthread_create(thread + 1, NULL,  threadify, new threadify_args_t(3, 3, true, false));
//  pthread_join(thread[1], NULL);
  pthread_join(thread[0], NULL);
  return 0;
}


