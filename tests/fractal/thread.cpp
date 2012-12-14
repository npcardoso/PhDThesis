#include "thread.h"

#include <libinstrument/instrument.h>

#include <pthread.h>
#include <stdlib.h>

#define load() {for(int j = rand() % 100000; j--;);}

void *threadify(void * v) {
  instr_transaction_start(instr_void);
  threadify_args_t * args = (threadify_args_t *) v;

  if(args->detach)
    pthread_detach(pthread_self());

  if(args->level > 0) {
    instr_probe(instr_void);
    pthread_t * thread = new pthread_t[args->count];
    long level = (long) v;

    instr_transaction_start(instr_void);
    for(int i = 0; i < args->count; i++){
      int tmp = rand();
      instr_probe(instr_var(i), 
                  instr_var(tmp),
                  instr_pmetadata("Name", "Launcher loop"),
                  instr_pmetadata("ComponentID", "1234"));

      pthread_create(&thread[i], NULL,  threadify, (void*)args->levelDown());
      
      instr_oracle(i < 1, 1, instr_void);
      if(args->load) load();
      if(args->load) 
        instr_probe(instr_void);
    }
    instr_transaction_end(instr_void);

    if(args->load) load();

    if (!args->detach)
      for(int i = 0; i < args->count; i++){
        instr_probe(instr_void);
        pthread_join(thread[i], NULL);
      }
    delete[] thread;
  }

  delete args;
  instr_transaction_end(instr_void);
  return NULL;
}

