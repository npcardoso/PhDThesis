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
  
  options.input() >> spectra;


  /* Initialize MPI */
  MPI_Init(NULL, NULL);

  /* Find out my identity in the default communicator */
  int ntasks, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  t_mhs <T_ACTIVITY> mhs(options.mhs);
  
  if(ntasks > 1){
    t_count mpi_level = 1;
    if(options.mpi_level)
      mpi_level = options.mpi_level;

    t_heuristic <T_ACTIVITY> heuristic = mhs.get_heuristic(mpi_level);
    mhs.set_heuristic(mpi_level + 1, heuristic);

    std::default_random_engine gen(123);
    if(options.mpi_stride)
      heuristic.push(new heuristics::t_divide<t_count>(rank, ntasks, options.mpi_stride));
    else
      heuristic.push(new heuristics::t_random_divide<t_count>(rank, ntasks, gen));
      //heuristic.push(new heuristics::t_divide<t_count>(rank, ntasks));

    mhs.set_heuristic(mpi_level, heuristic);
  }

  t_time_interval time_begin = get_time_interval();
  
  mhs.calculate(spectra, D);

  t_time_interval time_end_calculate = get_time_interval();
  options.debug() << "Process " << rank << " Calculation Time: " << (time_end_calculate - time_begin) << std::endl;

  if(ntasks > 1){
    mpi_reduce_trie(D);

    t_time_interval time_end_transfer = get_time_interval();
    options.debug() << "Process " << rank << " Transfer Time: " << (time_end_transfer - time_end_calculate) << std::endl;
  }  

  if(rank == 0)
    options.output() << D;
  
  /* Shut down MPI */
  MPI_Finalize();
  
  return 0;
}

#endif
