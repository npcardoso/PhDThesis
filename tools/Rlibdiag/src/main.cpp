#include <Rcpp.h>

#include "diagnosis/spectra/count_spectra.h"
#include "diagnosis/algorithms/barinel.h"
#include "diagnosis/algorithms/mhs.h"
#include "diagnosis/heuristics/similarity.h"
#include "diagnosis/heuristics/sort.h"

using namespace Rcpp;
using namespace diagnosis;
using namespace diagnosis::structs;

void matrix_to_spectra(NumericMatrix A, NumericVector e, t_count_spectra & spectra) {
  spectra.set_element_count(A.ncol(), A.nrow());
  for(t_transaction_id t = 0; t < A.nrow(); t++) {
    for(t_component_id c = 0; c < A.ncol(); c++)
      spectra.set_count(c+1, t+1, A.column(c)[t]);
    spectra.error(t+1, e[t]);
  }
}

void trie_to_list(t_trie & D, List & ret){
  t_trie::iterator it = D.begin();
  while(it != D.end()){
    NumericVector tmp(it->begin(), it->end());
    ret.push_back(tmp);
    it++;
  }
}

RcppExport SEXP mhs2(SEXP AA, SEXP ee, SEXP maxrt, SEXP maxc, SEXP maxcs) {
  NumericMatrix A(AA);
  NumericVector e(ee);
  
  structs::t_trie D;
  t_heuristic heuristic;
  heuristic.push(new heuristics::t_ochiai());
  heuristic.push(new heuristics::t_sort());
  algorithms::t_mhs  mhs(heuristic);
  
  mhs.max_time = NumericVector(maxrt)[0];
  mhs.max_candidates = NumericVector(maxc)[0];
  mhs.max_candidate_size = NumericVector(maxcs)[0];

  t_count_spectra spectra;

  matrix_to_spectra(A, e, spectra);
  mhs.calculate(spectra, D);
  
  List ret;
  trie_to_list(D, ret);

  return ret;
}

RcppExport SEXP fuzzinel(SEXP AA, SEXP ee, SEXP DD) {
  NumericMatrix A(AA);
  NumericVector e(ee);

  List D (DD);
  List::iterator it = D.begin();
  
  t_count_spectra spectra;
  matrix_to_spectra(A, e, spectra);
  
  diagnosis::algorithms::t_barinel barinel;
  
  t_goodness_mp total_ret;
  std::list<t_goodness_mp> ret_tmp;
  std::list<t_goodness_mp>::iterator ret_tmp_it;
  
  for(int i = 0; i < D.size(); i++) {
    NumericVector d = as<NumericVector>(D[i]);
    t_candidate tmp_d(d.begin(), d.end());
    t_goodness_mp tmp;
    
    barinel.calculate(spectra, tmp_d, tmp);
    ret_tmp.push_back(tmp);
    total_ret += tmp;
    it++;
  }
  
  NumericVector ret;
  ret_tmp_it = ret_tmp.begin();
  while(ret_tmp_it != ret_tmp.end()) {
    ret.push_back((*ret_tmp_it / total_ret).toDouble());
    ret_tmp_it++;
  }
  return ret;
}
// [[Rcpp::export]]
