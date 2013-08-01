#include "diagnosis/spectra/count_spectra.h"
#include "types.h"
#include "configure.h"


int main(int argc, char ** argv){
  diagnosis::t_count_spectra<t_count> spectra;
  t_iscandidate_options options(argv[0]);
      
  if(options.configure(argc, argv))
    return 1;
      
  
  options.spectra_input() >> spectra;
  while(true){
    diagnosis::t_candidate c;
    options.input() >> c;
    if(!c.size())
      break;
    options.output() << spectra.is_candidate(c) << std::endl;
  }
  return 0;
}

