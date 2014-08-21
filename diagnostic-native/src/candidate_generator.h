#ifndef __CANDIDATE_GENERATOR_H_32821d9e678a484910545976c2e3012816ba179d__
#define __CANDIDATE_GENERATOR_H_32821d9e678a484910545976c2e3012816ba179d__

#include "algorithm.h"
#include "structs/spectrum.h"
#include "structs/spectrum_filter.h"
#include "structs/trie.h"

namespace diagnostic {

class t_candidate_generator : public t_algorithm {
public:
    typedef t_trie t_ret_type;

    virtual void operator () (const t_spectrum & spectrum,
                              t_ret_type & result,
                              const t_spectrum_filter * filter=NULL) const = 0;

    inline virtual ~t_candidate_generator () {}
};

}

#endif
