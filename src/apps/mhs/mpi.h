#ifndef __MPI_H__
#define __MPI_H__

#include "../../common/trie.h"

#include <iostream>

void mpi_reduce_trie(t_trie & trie, bool hierarchical, size_t buffer_size, std::ostream & debug=std::cerr);

#endif
