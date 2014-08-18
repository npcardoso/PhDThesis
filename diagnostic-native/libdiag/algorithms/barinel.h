#ifndef __BARINEL_H_711a21f02fd5adb38c911bf85cb43da48da2a32a__
#define __BARINEL_H_711a21f02fd5adb38c911bf85cb43da48da2a32a__


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
typedef std::vector<t_goodness_mp> t_barinel_goodnesses;


class t_barinel_model {
public:
    t_barinel_model (const t_spectra & spectra,
                     const t_candidate & candidate,
                     bool use_fuzzy_error=true,
                     bool use_confidence=true,
                     const t_spectra_filter * filter=NULL);

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
    t_barinel (size_t precision=128);

    virtual void operator () (const t_spectra & spectra,
                              const t_trie & D,
                              t_ret_type & probs,
                              const t_spectra_filter * filter=NULL) const;

    virtual void operator () (const t_spectra & spectra,
                              const t_candidate & candidate,
                              t_probability_mp & ret,
                              const t_spectra_filter * filter=NULL) const;


    virtual void probability (const t_spectra & spectra,
                              const t_candidate & candidate,
                              const t_barinel_goodnesses & goodnesses,
                              t_probability_mp & ret,
                              const t_spectra_filter * filter=NULL,
                              bool use_confidence=true,
                              bool use_fuzzy_error=true,
                              bool use_count=false) const;


    virtual void prior (const t_candidate & candidate,
                        t_goodness_mp & ret) const;

    inline virtual t_score_type get_score_type () const {
        return PROBABILITY;
    }

    inline virtual std::string to_string() const {
        return "t_barinel";
    }
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
