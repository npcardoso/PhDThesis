#include "mhs.h"
#include "sandbox.h"


#include <cstring>
#include <cstdio>

#include <libgen.h>

int main(int argc, char ** argv){
  char * exec_name = basename(argv[0]);
  puts(exec_name);
  if(!strcmp(exec_name, "sandbox"))
    sandbox(argc, argv);
  else if(!strcmp(exec_name, "mpi"));
  else {
    t_count_spectra count_spectra;
    t_mhs<t_count> mhs(new t_similarity<t_count>());
    t_trie D;
    
    std::cin >> count_spectra;
    std::cout << count_spectra;
    
    mhs.calculate(count_spectra, D);
    D.print(std::cout);
  }
}


