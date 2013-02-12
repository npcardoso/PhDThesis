#ifndef __HEURISTIC_PARALLELIZATION_H__
#define __HEURISTIC_PARALLELIZATION_H__

#include "heuristic.h"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

namespace heuristics {

template <class T_ACTIVITY>
  class t_divide: public t_heuristic_filter<T_ACTIVITY> {
    t_count self, division_count, stride;
  public:
    t_divide(t_count self, 
             t_count division_count, 
             t_count stride = 1) {
      assert(self < division_count);
      assert(division_count > 0);
      assert(stride > 0);
      this->self = self;
      this->division_count = division_count;
      this->stride = stride;
    }

    virtual void operator()(const t_spectra <T_ACTIVITY> & spectra, 
                            t_rank_element * ret,
                            const t_spectra_filter * filter = NULL) const {
      t_count component_count = spectra.get_component_count(filter);

      for(t_id i = 0; i < component_count; i++)
        if((i / stride) % division_count != self)
          ret[i] = t_rank_element(ret[i].get_component(), -1);
    }

    virtual std::ostream & print(std::ostream & out) const {
      return out << "t_divide(s:" << self << ", #d:"  << division_count << ", s:"  << stride << ")";
    }

  };

template <class T_ACTIVITY>
  class t_random_divide: public t_heuristic_filter<T_ACTIVITY> {
    t_count self, division_count;
    boost::random::mt19937 & gen;
  public:
    t_random_divide(t_count self, 
                    t_count division_count,
                    boost::random::mt19937 & gen): gen(gen) {
      assert(self < division_count);
      assert(division_count > 0);
      this->self = self;
      this->division_count = division_count;
    }

    virtual void operator()(const t_spectra <T_ACTIVITY> & spectra, 
                            t_rank_element * ret,
                            const t_spectra_filter * filter = NULL) const {
      boost::random::uniform_int_distribution<t_component_id> distribution(0, division_count - 1);

      t_count component_count = spectra.get_component_count(filter);

      for(t_id i = 0; i < component_count; i++) {
        t_component_id owner = distribution(gen);
        if(owner != self)
          ret[i] = t_rank_element(ret[i].get_component(), -1);
      }
    }

    virtual std::ostream & print(std::ostream & out) const {
      return out << "t_random_divide(s:" << self << ", #d:"  << division_count << ")";
    }

  };

}
#endif
