#ifndef __SIMILARITY_H__
#define __SIMILARITY_H__

#include "heuristic.h"
#include "spectra_iterator.h"
#include "utils.h"

#include <cstring>
#include <cmath>

#include <algorithm>
#include <vector>

template <class T_ACTIVITY>
class t_similarity: public t_heuristic <T_ACTIVITY> {

public:
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
      ret[i++] = t_rank_element (it.get_component(), similarity(n));
    }
    std::sort(ret, ret + i);
  }

  virtual t_heuristic_value similarity(const t_count n[2][2]) const = 0;

};

template <class T_ACTIVITY>
class t_ochiai: public t_similarity <T_ACTIVITY> {

public:
  virtual t_heuristic_value similarity(const t_count n[2][2]) const {
    t_heuristic_value tmp = (n[1][1] + n[0][1]) * (n[1][1] + n[1][0]);
    if(tmp)
      return n[1][1] / sqrt(tmp);
    return 0;
  }

};


#endif
