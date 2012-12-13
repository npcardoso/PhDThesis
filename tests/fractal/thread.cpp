#include "thread.h"

#include <libinstrument/probe.h>
#include <libinstrument/oracle.h>
#include <libinstrument/transaction.h>

#include <pthread.h>
#include <stdlib.h>

#define load() {for(int j = rand() % 100000; j--;);}

void *threadify(void * v) {
  transaction_start();
  threadify_args_t * args = (threadify_args_t *) v;

  if(args->detach)
    pthread_detach(pthread_self());

  if(args->level > 0) {
    pthread_t * thread = new pthread_t[args->count];
    long level = (long) v;

    transaction_start();
    for(int i = 0; i < args->count; i++){
      int tmp = rand();
      probe_metadata(pstate(i, tmp), metadata_item("name", "Launcher loop"));
      oracle(i < 1, 1);
      pthread_create(&thread[i], NULL,  threadify, (void*)args->levelDown());
      if(args->load) load();
      if(args->load) 
        hit_probe();
    }
    transaction_end();

    if(args->load) load();

    if (!args->detach)
      for(int i = 0; i < args->count; i++){
        hit_probe();
        pthread_join(thread[i], NULL);
      }
    delete[] thread;
  }

  delete args;
  transaction_end();
  return NULL;
}

