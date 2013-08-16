#ifndef __DIAGNOSIS_HEURISTICS_SIMILARITY_H__
#define __DIAGNOSIS_HEURISTICS_SIMILARITY_H__

#include "diagnosis/heuristics/heuristic.h"
#include "diagnosis/diagnosis_system.h"
#include "utils/boost.h"

namespace diagnosis {
namespace heuristics {
class t_similarity : public t_heuristic_filter, public t_candidate_ranker {
public:
    DEFINE_BOOST_SHARED_PTRS(t_similarity);

    // From t_candidate_ranker
    virtual void operator () (const structs::t_spectra & spectra,
                              const structs::t_trie & D,
                              t_ret_type & probs,
                              const structs::t_spectra_filter * filter=NULL) const;

    // From t_heuristic_filter
    virtual void operator () (const structs::t_spectra & spectra,
                              t_rank_element * ret,
                              const structs::t_spectra_filter * filter=NULL) const;

    virtual t_rank_element::t_score similarity_coefficient (const structs::t_spectra & spectra,
                                                            t_component_id comp,
                                                            const structs::t_spectra_filter * filter=NULL) const;

protected:
    virtual t_rank_element::t_score similarity_coefficient (const t_count n[2][2]) const = 0;
};

class t_ochiai : public t_similarity {
protected:
    virtual t_rank_element::t_score similarity_coefficient (const t_count n[2][2]) const;

    virtual std::ostream & print (std::ostream & out) const;
};
}
}
#endif