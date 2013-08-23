#ifndef __PARALLELIZATION_H_79117851b7b65151a09fd26a8a2bd425f8d0952e__
#define __PARALLELIZATION_H_79117851b7b65151a09fd26a8a2bd425f8d0952e__

#include "diagnosis/heuristics/heuristic.h"

#include <boost/random/mersenne_twister.hpp>

namespace diagnosis {
namespace heuristics {
class t_divide : public t_heuristic_filter {
    t_count self, division_count, stride;
public:
    t_divide (t_count self,
              t_count division_count,
              t_count stride=1);

    virtual void operator () (const structs::t_spectra & spectra,
                              t_rank_element * ret,
                              const structs::t_spectra_filter * filter=NULL) const;

    virtual std::ostream & print (std::ostream & out) const;
};

class t_random_divide : public t_heuristic_filter {
    t_count self, division_count;
    boost::shared_ptr<boost::mt19937> gen;
public:
    t_random_divide (t_count self,
                     t_count division_count,
                     unsigned int seed);

    virtual void operator () (const structs::t_spectra & spectra,
                              t_rank_element * ret,
                              const structs::t_spectra_filter * filter=NULL) const;

    virtual std::ostream & print (std::ostream & out) const;
};
}
}
#endif