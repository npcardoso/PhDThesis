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
  t_heuristic_value cutoff;
public:
  t_similarity(t_similarity_coefficient * similarity_coefficient = NULL, t_heuristic_value cutoff = 0) {
    if(!similarity_coefficient)
      similarity_coefficient = new t_ochiai();
    this->similarity_coefficient = t_similarity_coefficient_ptr(similarity_coefficient);
    
    this->cutoff = cutoff;
  }

  virtual void order(const t_spectra <T_ACTIVITY> & spectra, 
                     t_rank_element * ret,
                     const t_spectra_filter * filter = NULL) const {

    assert(ret != NULL);

    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count(),
                          filter);
    t_count n[2][2];
    t_count total_transactions = spectra.get_transaction_count(filter);
    t_count total_components = spectra.get_component_count(filter);
    

    t_id i = 0;
    while (it.next_component()){
      memset(n, 0,  sizeof(t_count) * 4);
      while (it.next_transaction()){
        bool activity = spectra.get_count(it.get_component(), it.get_transaction());
        bool error = spectra.is_error(it.get_transaction());
        n[activity?1:0][error?1:0]++;
      }
      assert(n[0][0] + n[0][1] + n[1][0] + n[1][1] == total_transactions);
      t_heuristic_value value = (*similarity_coefficient)(n);
      if(value > cutoff)
        ret[i++] = t_rank_element (it.get_component(), value);
    }
    std::sort(ret, ret + i);
    while(i < total_components)
      ret[i++] = t_rank_element(0, -1);
  }

};

template <class T_ACTIVITY>
class t_parallel_similarity: public t_similarity<T_ACTIVITY> {
  t_count division, division_count, stride;
public:
  t_parallel_similarity(t_count division, 
                        t_count division_count, 
                        t_count stride = 1, 
                        t_similarity_coefficient * similarity_coefficient = NULL,
                        t_heuristic_value cutoff = 0) :
    t_similarity<T_ACTIVITY>(similarity_coefficient, cutoff) {
      assert(division < division_count);
      assert(division_count > 0);
      assert(stride > 0);
      this->division = division;
      this->division_count = division_count;
      this->stride = stride;
  }

  virtual void order(const t_spectra <T_ACTIVITY> & spectra, 
                     t_rank_element * ret,
                     const t_spectra_filter * filter = NULL) const {
    t_similarity<T_ACTIVITY>::order(spectra, ret, filter);

    t_count component_count = spectra.get_component_count(filter);

    for(t_id i = 0; i < component_count; i++)
      if((i / stride) % division_count != division)
        ret[i] = t_rank_element(ret[i].get_component(), -1);
  }

};


#endif
