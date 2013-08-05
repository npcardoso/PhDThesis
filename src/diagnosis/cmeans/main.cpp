#include "diagnosis/spectra/count_spectra.h"
#include "diagnosis/algorithms/cmeans.h"
#include "configure.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace diagnosis;

void print(std::ostream & out, t_count nrows, t_count ncols, algorithms::t_value_const_ptr p) {
  for (t_id i = 0; i < nrows; i++) {
    out << (i +1) << '\t';
    for (t_id j = 0; j < ncols; j++)
      out << MATRIX_CELL(p,i,j,ncols) << '\t';
    out << std::endl;
  }
}

int main(int argc, char ** argv) {
  t_cmeans_options options(argv[0]);
  
  if (options.configure(argc, argv))
      return 1;
  
  t_count_spectra spectra;
  options.input() >> spectra;

  algorithms::t_cmeans cm(spectra);
  algorithms::t_membership_ptr fuzzy_cluster = cm.clustering(spectra,options.configs);

  print(options.output(), spectra.get_transaction_count(), 2, fuzzy_cluster);
  
  return 0;
}