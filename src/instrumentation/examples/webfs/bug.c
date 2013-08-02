#include "bug.h"

#include <libinstrument/instrument.h>
#include <stdlib.h>


pthread_mutex_t leak_init_lock=PTHREAD_MUTEX_INITIALIZER;

void _leak_init(leak_t ** l, size_t min, size_t max, size_t total) {
  if(*l != NULL)
    return;

  *l = (leak_t *) malloc(sizeof(leak_t));

  (*l)->min = min;
  (*l)->max = max;
  (*l)->total = total;

  (*l)->leakage_total = 0;
  (*l)->leakage_count = 0;

  (*l)->start_time = time(NULL);
  pthread_mutex_init(&((*l)->lock), NULL);
}

void _leak(leak_t * l){
  l->leakage_total += l->min + (size_t) (rand() / (float) RAND_MAX * (l->max - l->min));
  l->leakage_count++;

  instr_oracle(l->leakage_total < l->total,
               1);
}
