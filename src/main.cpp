#include "algorithms/mhs.h"
#include "heuristic/parallelization.h"
#include "heuristic/similarity.h"
#include "heuristic/sort.h"
#include "spectra/count_spectra.h"
#include "mpi.h"
#include "sandbox.h"


#include <cstring>
#include <cstdio>

#include <libgen.h>

int main(int argc, char ** argv){
  t_time_interval time_begin = get_time_interval();
  char * exec_name = basename(argv[0]);
  std::cerr << exec_name << std::endl;

  if(!strcmp(exec_name, "sandbox"))
    sandbox(argc, argv);
  else if(!strcmp(exec_name, "mpi"))
    mpi_main(argc, argv);
  else {
    t_count_spectra count_spectra;
    t_heuristic<t_count> heuristic;
    heuristic.push(new t_filter_ochiai<t_count>());
    //  heuristic.push(new t_filter_cutoff<t_count>());
    heuristic.push(new t_filter_sort<t_count>());

    t_mhs<t_count> mhs(heuristic);
    
    t_trie D;
    
    std::cin >> count_spectra;
    std::cout << count_spectra;
    
    mhs.calculate(count_spectra, D);
    std::cout << D;
  }
  t_time_interval time_end = get_time_interval();
  std::cerr << "Run Time: " << (time_end - time_begin) << std::endl;
}


