#ifndef __HEURISTIC_H__
#define __HEURISTIC_H__

#include "../spectra/spectra.h"
#include "../spectra/spectra_filter.h"

#include <boost/shared_ptr.hpp>

template <class T_ACTIVITY>
class t_heuristic_filter {
public:
  virtual void operator()(const t_spectra <T_ACTIVITY> & spectra, 
                          t_rank_element * ret,
                          const t_spectra_filter * filter = NULL) const = 0;
};

template <class T_ACTIVITY>
class t_heuristic {
  typedef boost::shared_ptr<const t_heuristic_filter <T_ACTIVITY> > t_filter_ptr;
  typedef std::vector<t_filter_ptr> t_filters;
  t_filters filters;

public:
 
  void push(const t_filter_ptr & filter) {
    filters.push_back(filter);
  }

  void push(const t_heuristic_filter <T_ACTIVITY> * filter) {
    filters.push_back(t_filter_ptr(filter));
  }

  void operator()(const t_spectra <T_ACTIVITY> & spectra, 
                  t_rank_element * ret,
                  const t_spectra_filter * filter = NULL) const {
    assert(ret != NULL);
    
    typename t_filters::const_iterator it = filters.begin();
    if(!spectra.get_transaction_count(filter))
      return;

    ret[0] = t_rank_element(0, 0);

    while(it != filters.end())
      (**(it++))(spectra, ret, filter);
  }
};



#endif
