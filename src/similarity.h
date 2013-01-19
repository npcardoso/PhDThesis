#ifndef __SIMILARITY_H__
#define __SIMILARITY_H__

#include "heuristic.h"
#include "spectra_iterator.h"


template <class T_ACTIVITY>
class t_similarity: t_heuristic <T_ACTIVITY> {

public:
  inline virtual void order(const t_spectra <T_ACTIVITY> & spectra, 
                            const t_spectra_filter * filter, 
                            t_component_id * ret) const{

    t_spectra_iterator it(spectra.get_component_count(),
                          spectra.get_transaction_count,
                          filter);
    t_transaction_id transaction = 1;
    t_component_id component = 1;
    t_count n[2][2];
    
    while (it.next_component()){
      memset(n, 0,  sizeof(t_count) * 4);
      while (it.next_transaction()){
        bool activity = spectra.get_count(it.get_component(), it.get_transaction());
        bool error = spectra.is_error(transaction++);
        n[activity?1:0][error?1:0]++;
        similarity(n, ret);
      }
    }
    //  while()


  }

  virtual float similarity(const t_count n[2][2]) const = 0;

};

template <class T_ACTIVITY>
class t_ochiai: t_similarity <T_ACTIVITY> {

public:
  virtual float similarity(const t_count n[2][2]) const;

};


#endif
