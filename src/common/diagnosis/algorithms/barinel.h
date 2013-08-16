#ifndef __DIAGNOSIS_ALGORITHMS_BARINEL_H__
#define __DIAGNOSIS_ALGORITHMS_BARINEL_H__

#include "diagnosis/diagnosis_system.h"
#include "diagnosis/types.h"
#include "diagnosis/structs/spectra_iterator.h"
#include "diagnosis/structs/trie.h"

#include <map>

namespace diagnosis {
namespace algorithms {
typedef std::vector<t_goodness_mp> t_barinel_goodnesses;

class t_barinel_model {
public:
    std::vector<t_error> pass;
    std::vector<t_error> fail;

    t_barinel_model ();
    t_barinel_model (size_t components);

    void set_size (size_t components);

    virtual void gradient (const t_barinel_goodnesses & goodnesses,
                           t_barinel_goodnesses & ret) const;

    virtual void update (const t_barinel_goodnesses & g,
                         const t_barinel_goodnesses & grad,
                         t_barinel_goodnesses & ret,
                         double lambda) const;
};


class t_barinel : public t_candidate_ranker {
public:
    DEFINE_BOOST_SHARED_PTRS(t_barinel);

    t_barinel ();
    t_barinel (size_t precision);

    virtual void operator () (const structs::t_spectra & spectra,
                              const structs::t_trie & D,
                              t_ret_type & probs,
                              const structs::t_spectra_filter * filter=NULL) const;

    virtual void calculate (const structs::t_spectra & spectra,
                            const structs::t_candidate & candidate,
                            t_probability_mp & ret,
                            const structs::t_spectra_filter * filter=NULL) const;

    virtual void model (const structs::t_spectra & spectra,
                        const structs::t_candidate & candidate,
                        t_barinel_model & model,
                        const structs::t_spectra_filter * filter=NULL) const;


    virtual void prior (const structs::t_candidate & candidate,
                        t_goodness_mp & ret) const;

    t_goodness_mp g_j;
    double epsilon;
    double lambda;
    t_count iterations;
    size_t precision;
    bool use_fuzzy_error;
    bool use_confidence;
};
}
}

#endif