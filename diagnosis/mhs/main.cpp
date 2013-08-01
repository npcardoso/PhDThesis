#include "configure.h"
#include "diagnosis/spectra/count_spectra.h"
#include "mpi.h"
#include "opt.h"
#include "stats.h"
#include "utils/time.h"


#include <mpi.h>

using namespace diagnosis;

int main(int argc, char ** argv){
  t_mhs_options options(argv[0]);
      
  if(options.configure(argc, argv))
    return 1;
      
  t_count_spectra<t_count> spectra;
  structs::t_trie D;
  algorithms::t_mhs  mhs(options.mhs);
  
  /* Initialize MPI */
  MPI_Init(NULL, NULL);
  
  /* Find out my identity in the default communicator */
  int ntasks, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  t_stats stats;
  
  if(rank == 0)
    options.debug() << options << std::endl;
  
  options.input() >> spectra;

  t_time_interval time_begin = time_interval();

  if(ntasks > 1){
    t_count mpi_level = 1;
    if(options.mpi_level)
      mpi_level = options.mpi_level;

    t_heuristic  heuristic = mhs.get_heuristic(mpi_level);
    mhs.set_heuristic(mpi_level + 1, heuristic);

    int seed = time(NULL);
    MPI_Bcast(&seed, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
    
    boost::random::mt19937 gen(seed);
    if(options.mpi_stride)
      heuristic.push(new heuristics::t_divide(rank, ntasks, options.mpi_stride));
    else
      heuristic.push(new heuristics::t_random_divide(rank, ntasks, gen));

    mhs.set_heuristic(mpi_level, heuristic);
  }

  t_time_interval time = time_interval();

  mhs.calculate(spectra, D);
  
  stats.items_generated = D.size();
  stats.total_calc = (time_interval() - time);
  time = time_interval();

  if(ntasks > 1){
    mpi_reduce_trie(D, options.mpi_hierarchical, options.mpi_buffer, stats);
    
    stats.total_transfer = (time_interval() - time);
    time = time_interval();
  }  

  stats.runtime = (time_interval() - time_begin);
  
  if(rank == 0) {
    options.output() << D;
    
    MPI_Status status;

    for(t_count i = ntasks; --i;) {
      t_stats tmp_stats;
      MPI_Recv(&tmp_stats, sizeof(t_stats), MPI::BYTE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
      tmp_stats.print(options.debug(), i, false);
    }
    stats.print(options.debug(), 0, true);
    options.debug() << "Candidates: " << D.size() << std::endl;
  } else {
    MPI_Send(&stats, sizeof(t_stats), MPI::BYTE, 0, 1, MPI_COMM_WORLD);
  }

  /* Shut down MPI */
  MPI_Finalize();

  return 0;
}
