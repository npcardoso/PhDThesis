#ifndef __BARINEL_H_711a21f02fd5adb38c911bf85cb43da48da2a32a__
#define __BARINEL_H_711a21f02fd5adb38c911bf85cb43da48da2a32a__

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
    t_barinel_model (const structs::t_spectra & spectra,
                     const structs::t_candidate & candidate,
                     bool use_fuzzy_error=true,
                     bool use_confidence=true,
                     const structs::t_spectra_filter * filter=NULL);

    virtual void gradient (const t_barinel_goodnesses & goodnesses,
                           t_barinel_goodnesses & ret) const;

    virtual void update (const t_barinel_goodnesses & g,
                         const t_barinel_goodnesses & grad,
                         t_barinel_goodnesses & ret,
                         double lambda) const;

private:
    std::vector<t_error> pass;
    std::vector<t_error> fail;
};


class t_barinel : public t_candidate_ranker {
public:
    t_barinel ();
    t_barinel (size_t precision);

    virtual void operator () (const structs::t_spectra & spectra,
                              const structs::t_trie & D,
                              t_ret_type & probs,
                              const structs::t_spectra_filter * filter=NULL) const;

    virtual void operator () (const structs::t_spectra & spectra,
                              const structs::t_candidate & candidate,
                              t_probability_mp & ret,
                              const structs::t_spectra_filter * filter=NULL) const;


    virtual void prior (const structs::t_candidate & candidate,
                        t_goodness_mp & ret) const;

private:
    inline void probability (const structs::t_spectra & spectra,
                             const structs::t_candidate & candidate,
                             const t_barinel_goodnesses & goodnesses,
                             t_probability_mp & ret,
                             const structs::t_spectra_filter * filter=NULL) const {spectra.probability(candidate, goodnesses, ret, filter, use_confidence, use_fuzzy_error, false);}

public:
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