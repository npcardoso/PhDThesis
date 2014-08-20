#ifndef __FUZZINEL_H_ceba8192faeec08ddc2b9f84ca6b52db29dbae59__
#define __FUZZINEL_H_ceba8192faeec08ddc2b9f84ca6b52db29dbae59__
#include "../diagnostic_system.h"
#include "../types.h"

#include <boost/multiprecision/mpfr.hpp>

#include <map>

namespace diagnostic {
namespace algorithms {

// Arbitrary Precision
namespace mp = boost::multiprecision;
typedef mp::number<mp::mpfr_float_backend<300>, mp::et_off>  t_probability_mp;
typedef t_probability_mp t_goodness_mp;



class t_fuzzinel : public t_candidate_ranker {
public:
    t_fuzzinel (size_t precision=128);

    virtual void operator () (const t_spectra & spectra,
                              const t_trie & D,
                              t_ret_type & probs,
                              const t_spectra_filter * filter=NULL) const;

    virtual void operator () (const t_spectra & spectra,
                              const t_candidate & candidate,
                              t_probability_mp & ret,
                              const t_spectra_filter * filter=NULL) const;


    inline virtual t_score_type get_score_type () const {
        return PROBABILITY;
    }

    inline virtual std::string get_name() const {
        return "fuzzinel";
    }

    virtual void json_configs (t_configs & out) const;

public:
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
