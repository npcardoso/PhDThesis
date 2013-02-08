#ifndef __MPI_H__
#define __MPI_H__

#include "common/trie.h"
#include "heuristic/parallelization.h"
#include "opts.h"

#include <mpi.h>
#include <iostream>

void send_trie(const t_trie & trie, t_count chunk_size,
               int destination, int tag,
               MPI_Comm communicator);

void receive_trie(t_trie & trie, t_count chunk_size,
                  int destination, int tag,
                  MPI_Comm communicator);

template <class T_ACTIVITY>
int mpi_main(int argc, char **argv, const t_options<T_ACTIVITY> & options, 
             const t_spectra <T_ACTIVITY> & spectra, t_trie & D) {
  /* Initialize MPI */
  MPI_Init(&argc, &argv);

  /* Find out my identity in the default communicator */
  int ntasks, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  t_mhs <T_ACTIVITY> mhs(options.mhs);
  
  t_count mpi_level = 1;
  if(options.mpi_level)
    mpi_level = options.mpi_level;
  
  t_heuristic <T_ACTIVITY> heuristic = mhs.get_heuristic(mpi_level);
  mhs.set_heuristic(mpi_level + 1, heuristic);
  
  if(options.mpi_stride)
    heuristic.push(new t_filter_divide<t_count>(rank, ntasks, options.mpi_stride));
  else
    heuristic.push(new t_filter_divide<t_count>(rank, ntasks));

  mhs.set_heuristic(mpi_level, heuristic);
  
  t_time_interval time_begin = get_time_interval();
  
  mhs.calculate(spectra, D);

  t_time_interval time_end_calculate = get_time_interval();
  std::cerr << "Process " << rank << " Calculation Time: " << (time_end_calculate - time_begin) << std::endl;

  if(rank == 0) {
    for(t_count i = 1; i < ntasks; i++)
      receive_trie(D, 102400, i, 0, MPI_COMM_WORLD);
  } else
    send_trie(D, 102400, 0, 0, MPI_COMM_WORLD);

  t_time_interval time_end_transfer = get_time_interval();
  std::cerr << "Process " << rank << " Transfer Time: " << (time_end_transfer - time_end_calculate) << std::endl;
  
  /* Shut down MPI */
  MPI_Finalize();
  
  return rank;
}

#endif
