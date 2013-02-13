#ifndef __MPI_H__
#define __MPI_H__

#include "../../common/trie.h"

void mpi_reduce_trie(t_trie & trie, bool hierarchical, size_t buffer_size);

#endif
