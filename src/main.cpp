#include "mhs.h"
#include "mpi.h"
#include "sandbox.h"
#include "similarity.h"


#include <cstring>
#include <cstdio>

#include <libgen.h>

int main(int argc, char ** argv){
  time_interval_t time_begin = get_time_interval();
  char * exec_name = basename(argv[0]);
  std::cout << "dev.a.out" << std::endl;
  std::cerr << exec_name << std::endl;

  if(!strcmp(exec_name, "sandbox"))
    sandbox(argc, argv);
  else if(!strcmp(exec_name, "mpi"))
    mpi_main(argc, argv);
  else {
    t_count_spectra count_spectra;
    t_mhs<t_count> mhs(new t_similarity<t_count>());
    t_trie D;
    
    std::cin >> count_spectra;
    std::cout << count_spectra;
    
    mhs.calculate(count_spectra, D);
    D.print(std::cout);
  }
  time_interval_t time_end = get_time_interval();
  std::cerr << "Run Time: " << (time_end - time_begin) << std::endl;
}


