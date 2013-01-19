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
    t_transaction_id transaction = 0;
    t_component_id component = 0;
    t_count n[2][2];
    //  while()


    similarity(n, ret);
  }

  virtual float similarity(const t_count n[2][2]) const = 0;

};

template <class T_ACTIVITY>
class t_ochiai: t_similarity <T_ACTIVITY> {

public:
  virtual float similarity(const t_count n[2][2]) const;

};


#endif
