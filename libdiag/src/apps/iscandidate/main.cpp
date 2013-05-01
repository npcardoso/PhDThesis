#include "main.h"
#include "../../spectra/count_spectra.h"
#include "../../types.h"


int main_iscandidate(const t_iscandidate_options & options) {
  t_count_spectra spectra;
    
  options.spectra_input() >> spectra;
  
  while(true){
    t_candidate c;
    options.input() >> c;
    if(!c.size())
      break;
    options.output() << spectra.is_candidate(c) << std::endl;
  }
  return 0;
}

