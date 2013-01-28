#ifndef __SIMILARITY_H__
#define __SIMILARITY_H__

#include "heuristic.h"
#include "spectra_iterator.h"
#include "utils.h"

#include <cstring>
#include <cmath>

#include <algorithm>
#include <vector>

#include <boost/shared_ptr.hpp>

class t_similarity_coefficient {
public:
  virtual t_heuristic_value operator()(const t_count n[2][2]) const = 0;
};

class t_ochiai: public t_similarity_coefficient {

public:
  virtual t_heuristic_value operator()(const t_count n[2][2]) const {
    t_heuristic_value tmp = (n[1][1] + n[0][1]) * (n[1][1] + n[1][0]);
    if(tmp)
      return n[1][1] / sqrt(tmp);
    return 0;
  }

};

template <class T_ACTIVITY>
class t_similarity: public t_heuristic <T_ACTIVITY> {
  typedef boost::shared_ptr<t_similarity_coefficient> t_similarity_coefficient_ptr;
  
  t_similarity_coefficient_ptr similarity_coefficient;

public:
  inline t_similarity(t_similarity_coefficient * similarity_coefficient = new t_ochiai()): similarity_coefficient(similarity_coefficient){
    assert(similarity_coefficient != NULL);
  }

  inline virtual void order(const t_spectra <T_ACTIVITY> & spectra, 
                            const t_spectra_filter * filter, 
                            t_rank_element * ret) const{

    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          filter);
    t_count n[2][2];
    t_count total_transactions = spectra.get_transaction_count();
    
    if(filter)
      total_transactions -= filter->get_filtered_transaction_count();

    t_id i = 0;
    while (it.next_component()){
      memset(n, 0,  sizeof(t_count) * 4);
      while (it.next_transaction()){
        bool activity = spectra.get_count(it.get_component(), it.get_transaction());
        bool error = spectra.is_error(it.get_transaction());
        n[activity?1:0][error?1:0]++;
      }
      assert(n[0][0] + n[0][1] + n[1][0] + n[1][1] == total_transactions);
      ret[i++] = t_rank_element (it.get_component(), (*similarity_coefficient)(n));
    }
    std::sort(ret, ret + i);
  }

};



#endif
