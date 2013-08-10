#ifndef __DIAGNOSIS_DIAGNOSIS_SYSYEM_H__
#define __DIAGNOSIS_DIAGNOSIS_SYSYEM_H__

#include "types.h"
#include "diagnosis/structs/spectra.h"
#include "diagnosis/structs/spectra_filter.h"
#include "diagnosis/structs/trie.h"
#include "utils/mpreal.h"
#include "utils/boost.h"

#include <list>

namespace diagnosis {
class t_candidate_generator {
public:
    typedef structs::t_trie t_ret_type;
    DEFINE_BOOST_SHARED_PTRS(t_candidate_generator);

    virtual void operator () (const structs::t_spectra & spectra,
                              t_ret_type & D,
                              const structs::t_spectra_filter * filter=NULL) const = 0;
};

class t_candidate_ranker {
public:
    typedef std::list<t_probability> t_ret_type;
    DEFINE_BOOST_SHARED_PTRS(t_candidate_ranker);

    virtual void operator () (const structs::t_spectra & spectra,
                              const structs::t_trie & D,
                              t_ret_type & probs,
                              const structs::t_spectra_filter * filter=NULL) const = 0;
};

class t_diagnosis_system {
public:
    typedef std::list<t_probability> t_ret_type;
    DEFINE_BOOST_SHARED_PTRS(t_diagnosis_system);

    inline t_diagnosis_system (const t_candidate_generator::t_ptr & gen,
                               const t_candidate_ranker::t_ptr & rank) : generator(gen), ranker(rank) {}

    virtual void operator () (const structs::t_spectra & spectra,
                              structs::t_trie & D,
                              t_ret_type & probs,
                              const structs::t_spectra_filter * filter=NULL) const;

protected:
    t_candidate_generator::t_ptr generator;
    t_candidate_ranker::t_ptr ranker;
};
}

#endif