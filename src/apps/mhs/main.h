#ifndef __MAIN_MHS_H__
#define __MAIN_MHS_H__
#include "configure.h"
#include "mpi.h"

#include "../../common/opt.h"
#include "../../spectra/count_spectra.h"

#include <mpi.h>

template <class T_ACTIVITY>
int main_mhs(const t_mhs_options<T_ACTIVITY> & options) {
  t_count_spectra spectra;
  t_trie D;
  t_mhs <T_ACTIVITY> mhs(options.mhs);
  
  /* Initialize MPI */
  MPI_Init(NULL, NULL);
  
  /* Find out my identity in the default communicator */
  int ntasks, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  
  if(rank == 0)
    options.debug() << options << std::endl;
  
  options.input() >> spectra;

  t_time_interval time_begin = get_time_interval();

  if(ntasks > 1){
    t_count mpi_level = 1;
    if(options.mpi_level)
      mpi_level = options.mpi_level;

    t_heuristic <T_ACTIVITY> heuristic = mhs.get_heuristic(mpi_level);
    mhs.set_heuristic(mpi_level + 1, heuristic);

    int seed = time(NULL);
    MPI_Bcast(&seed, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
    
    boost::random::mt19937 gen(seed);
    if(options.mpi_stride)
      heuristic.push(new heuristics::t_divide<t_count>(rank, ntasks, options.mpi_stride));
    else
      heuristic.push(new heuristics::t_random_divide<t_count>(rank, ntasks, gen));

    mhs.set_heuristic(mpi_level, heuristic);
  }

  t_time_interval time = get_time_interval();

  mhs.calculate(spectra, D);
  options.debug() << "Process " << rank << " Total Generated: " << D.size() << std::endl;
  options.debug() << "Process " << rank << " Calculation Time: " << (get_time_interval() - time) << std::endl;
  time = get_time_interval();

  if(ntasks > 1){
    mpi_reduce_trie(D, options.mpi_hierarchical, options.mpi_buffer, options.debug());

    options.debug() << "Process " << rank << " Transfer Time: " << (get_time_interval() - time) << std::endl;
    time = get_time_interval();
  }  

  if(rank == 0) {
    t_time_interval time_end = get_time_interval();
    options.debug() << "Candidates: " << D.size() << std::endl;
    options.debug() << "Run Time: " << (time_end - time_begin) << std::endl;
    options.output() << D;
  }

  /* Shut down MPI */
  MPI_Finalize();

  return 0;
}

#endif
