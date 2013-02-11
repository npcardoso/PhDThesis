#include "algorithms/mhs.h"
#include "heuristic/parallelization.h"
#include "heuristic/similarity.h"
#include "heuristic/sort.h"
#include "spectra/count_spectra.h"
#include "mpi.h"
#include "sandbox.h"
#include "opts.h"

#include <mpi.h>

#include <cstring>
#include <cstdio>

#include <iostream>
#include <fstream>

template <class T_ACTIVITY>
bool read_spectra(t_spectra<T_ACTIVITY> & spectra,
                  const t_options<T_ACTIVITY> & options) {
  if(options.input) {
    std::ifstream in(options.input);
    in >> spectra;
    in.close();
  }
  else
    std::cin >> spectra;
  return false;
}

template <class T_ACTIVITY>
bool write_candidates(const t_trie & trie,
                      const t_options<T_ACTIVITY> & options) {
  if(options.output) {
    std::ofstream out(options.output);
    out << trie;
    out.close();
  }
  else
    std::cout << trie;
  return false;
}

template <class T_ACTIVITY>
int main_trie(const t_options<T_ACTIVITY> & options) {

  t_trie trie;
  {
    std::ifstream in(options.trie);
    in >> trie;
    in.close();
  }
  
  std::istream * in = &std::cin;
  std::ifstream fin;
  
  std::ostream * out = &std::cout;
  std::ofstream fout;
  
  if(options.input) {
    fin.open(options.input);
    in = &fin;
  }
  
  if(options.output) {
    fout.open(options.output);
    out = &fout;
  }
  
  while(true){
    t_candidate c;
    (*in) >> c;
    if(!c.size())
      break;
    (*out) << trie.is_composite(c) << std::endl;
  }
  return 0;
}

template <class T_ACTIVITY>
int main_mhs(const t_options<T_ACTIVITY> & options) {
  t_count_spectra spectra;
  if(read_spectra(spectra, options))
    return 1;

  t_trie D;


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

    if(options.mpi_stride)
      heuristic.push(new t_filter_divide<t_count>(rank, ntasks, options.mpi_stride));
    else
      heuristic.push(new t_filter_divide<t_count>(rank, ntasks));

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
    write_candidates(D, options);
  
  /* Shut down MPI */
  MPI_Finalize();
  
  return 0;
}

int main(int argc, char ** argv){
  int ret = 0;
  t_time_interval time_begin = get_time_interval();

  t_options<t_count> options;
  if(configure(argc, argv, options))
    return 1;

  options.debug() << options << std::endl;

  if(options.trie) // Trie mode
    ret = main_trie(options);
  else // MHS mode
    ret = main_mhs(options);

  t_time_interval time_end = get_time_interval();
  options.debug() << "Run Time: " << (time_end - time_begin) << std::endl;
}
