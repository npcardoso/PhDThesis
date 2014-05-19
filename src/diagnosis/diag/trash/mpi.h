#ifndef __MPI_H__
#define __MPI_H__

#include "algorithms/mhs.h"
#include "structs/trie.h"
#include "stats.h"

#include <iostream>

void mhs2_reduce (diagnosis::structs::t_trie & trie,
                  bool hierarchical,
                  size_t buffer_size,
                  t_stats & stats);

void mhs2_map (const diagnosis::algorithms::t_mhs & mhs,
               const diagnosis::structs::t_spectra & spectra,
               diagnosis::structs::t_trie & D,
               t_stats & stats,
               const diagnosis::structs::t_spectra_filter * filter=NULL);

void mhs2_heuristic_setup (diagnosis::algorithms::t_mhs & mhs,
                           t_count mpi_level,
                           t_count mpi_stride);

void mhs2_collect_stats (std::ostream & out,
                         const diagnosis::structs::t_trie & D,
                         t_stats & stats);
#endif