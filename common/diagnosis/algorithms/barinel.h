#ifndef __DIAGNOSIS_ALGORITHMS_BARINEL_H__
#define __DIAGNOSIS_ALGORITHMS_BARINEL_H__

#include "diagnosis/heuristic.h"
#include "diagnosis/spectra.h"
#include "diagnosis/spectra_filter.h"
#include "diagnosis/spectra_iterator.h"
#include "diagnosis/structs/trie.h"

#include <map>

namespace diagnosis {
namespace algorithms {

  class t_barinel_model {
  public:
    std::vector<t_error> pass;
    std::vector<t_error> fail;

    t_barinel_model();
    t_barinel_model(size_t components);

    void set_size(size_t components);
  };

  typedef std::vector<t_goodness_mp> t_barinel_goodnesses;

  class t_barinel {
  public:
    t_barinel();
    t_barinel(size_t precision);

    void calculate(const t_spectra & spectra,
                   const t_candidate & candidate,
                   t_probability_mp & ret,
                   const t_spectra_filter * filter = NULL) const;

    void model(const t_spectra & spectra,
               const t_candidate & candidate,
               t_barinel_model & model,
               const t_spectra_filter * filter = NULL) const;

    void gradient(const t_barinel_model & model,
                  const t_barinel_goodnesses & goodnesses,
                  t_id component,
                  t_goodness_mp & ret) const;

    virtual void prior(const t_candidate & candidate,
                       t_goodness_mp & ret) const;
    
    t_goodness_mp g_j;
    double epsilon;
    double lambda;
    t_count iterations;
    size_t precision;
  };

}
}

#endif

