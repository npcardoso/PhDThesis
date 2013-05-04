#ifndef __MPI_H__
#define __MPI_H__

#include "diagnosis/structs/trie.h"
#include "stats.h"

#include <iostream>

void mpi_reduce_trie(diagnosis::structs::t_trie & trie,
                     bool hierarchical,
                     size_t buffer_size,
                     t_stats & stats);

#endif
