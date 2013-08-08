#ifndef __DIAGNOSIS_HEURISTICS_PARALLELIZATION_H__
#define __DIAGNOSIS_HEURISTICS_PARALLELIZATION_H__

#include "diagnosis/heuristic.h"

#include <boost/random/mersenne_twister.hpp>

namespace diagnosis {
namespace heuristics {
class t_divide : public t_heuristic_filter {
    t_count self, division_count, stride;
public:
    t_divide (t_count self,
              t_count division_count,
              t_count stride=1);

    virtual void operator () (const t_spectra & spectra,
                              t_rank_element * ret,
                              const t_spectra_filter * filter=NULL) const;

    virtual std::ostream & print (std::ostream & out) const;
};

class t_random_divide : public t_heuristic_filter {
    t_count self, division_count;
    boost::shared_ptr<boost::mt19937> gen;
public:
    t_random_divide (t_count self,
                     t_count division_count,
                     unsigned int seed);

    virtual void operator () (const t_spectra & spectra,
                              t_rank_element * ret,
                              const t_spectra_filter * filter=NULL) const;

    virtual std::ostream & print (std::ostream & out) const;
};
}
}
#endif