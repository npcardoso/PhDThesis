#include "similarity.h"

#include "../spectra/spectra_iterator.h"
#include "../utils/utils.h"

#include <cstring>
#include <cmath>

#include <algorithm>


namespace heuristics {

void t_similarity::operator()(const t_spectra & spectra, 
                              t_rank_element * ret,
                              const t_spectra_filter * filter) const {

  t_spectra_iterator it(spectra.get_component_count(),
                        spectra.get_transaction_count(),
                        filter);
  t_count n[2][2];

  t_id i = 0;
  while (it.next_component()){
    memset(n, 0,  sizeof(t_count) * 4);
    while (it.next_transaction()){
      bool activity = spectra.get_count(it.get_component(), it.get_transaction());
      bool error = spectra.is_error(it.get_transaction());
      n[activity?1:0][error?1:0]++;
    }
    assert(n[0][0] + n[0][1] + n[1][0] + n[1][1] == spectra.get_transaction_count(filter));
    t_heuristic_value value = similarity_coefficient(n);
    ret[i++] = t_rank_element (it.get_component(), value);
  }
}

t_heuristic_value t_ochiai::similarity_coefficient(const t_count n[2][2]) const {
  t_heuristic_value tmp = (n[1][1] + n[0][1]) * (n[1][1] + n[1][0]);
  if(tmp)
    return n[1][1] / sqrt(tmp);
  return 0;
}

std::ostream & t_ochiai::print(std::ostream & out) const {
  return out << "t_ochiai";
}

}

