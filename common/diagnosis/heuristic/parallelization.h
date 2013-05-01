#ifndef __HEURISTIC_PARALLELIZATION_H__
#define __HEURISTIC_PARALLELIZATION_H__

#include "heuristic.h"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

namespace heuristics {

class t_divide: public t_heuristic_filter{
  t_count self, division_count, stride;
public:
  t_divide(t_count self, 
           t_count division_count, 
           t_count stride = 1);

  virtual void operator()(const t_spectra & spectra, 
                          t_rank_element * ret,
                          const t_spectra_filter * filter = NULL) const;

  virtual std::ostream & print(std::ostream & out) const;
};

class t_random_divide: public t_heuristic_filter{
  t_count self, division_count;
  boost::random::mt19937 & gen;
public:
  t_random_divide(t_count self, 
                  t_count division_count,
                  boost::random::mt19937 & gen);

  virtual void operator()(const t_spectra & spectra, 
                          t_rank_element * ret,
                          const t_spectra_filter * filter = NULL) const;

  virtual std::ostream & print(std::ostream & out) const;
};

}
#endif
