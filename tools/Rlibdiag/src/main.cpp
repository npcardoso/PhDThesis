#include <Rcpp.h>

#include "diagnosis/spectra/count_spectra.h"
#include "diagnosis/algorithms/mhs.h"
#include "diagnosis/heuristics/similarity.h"
#include "diagnosis/heuristics/sort.h"

using namespace Rcpp;
using namespace diagnosis;

void matrix_to_spectra(NumericMatrix A, NumericVector e, t_count_spectra & spectra) {
  spectra.set_element_count(A.ncol(), A.nrow());
  for(t_transaction_id t = 0; t < A.nrow(); t++) {
    for(t_component_id c = 0; c < A.ncol(); c++)
      spectra.set_count(c+1, t+1, A.column(c)[t]);
    spectra.error(t+1, e[t]);
  }
  std::cout << spectra;
}

RcppExport SEXP test(SEXP AA, SEXP ee) {
  
  NumericMatrix A(AA);
  NumericVector e(ee);
  
  structs::t_trie D;
  t_heuristic heuristic;
  heuristic.push(new heuristics::t_ochiai());
  heuristic.push(new heuristics::t_sort());
  algorithms::t_mhs  mhs(heuristic);
  
  t_count_spectra spectra;

  matrix_to_spectra(A, e, spectra);

  return A;
}
// [[Rcpp::export]]
