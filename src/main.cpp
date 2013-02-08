#include "algorithms/mhs.h"
#include "heuristic/parallelization.h"
#include "heuristic/similarity.h"
#include "heuristic/sort.h"
#include "spectra/count_spectra.h"
#include "mpi.h"
#include "sandbox.h"
#include "opts.h"

#include <cstring>
#include <cstdio>

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


int main(int argc, char ** argv){

  t_options<t_count> options;
  if(configure(argc, argv, options))
    return 1;

  std::cerr << options << std::endl;
  
  t_count_spectra spectra;
  
  if(read_spectra(spectra, options))
    return 1;

  t_time_interval time_begin = get_time_interval();
  t_trie D;

  if(options.mpi)
    mpi_main(argc, argv, options, spectra, D);
  else {
    options.mhs.calculate(spectra, D);
    std::cout << D;
  }
  
  t_time_interval time_end = get_time_interval();
  std::cerr << "Run Time: " << (time_end - time_begin) << std::endl;
}


