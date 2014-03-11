#ifndef __MHS_H_957e289b8923d6c40fa69c8aa7a53a6ed1da023b__
#define __MHS_H_957e289b8923d6c40fa69c8aa7a53a6ed1da023b__

#include "diagnosis/algorithms/similarity.h"
#include "diagnosis/algorithms/mhs/cutoff.h"
#include "diagnosis/algorithms/mhs/parallelization.h"
#include "diagnosis/diagnosis_system.h"
#include "utils/time.h"

namespace diagnosis {
namespace algorithms {
class t_mhs : public t_candidate_generator {
public:
    t_mhs (t_const_ptr<t_similarity> similarity=t_const_ptr<t_similarity> ());

    void set_cutoff (t_const_ptr<t_basic_cutoff> cutoff);
    void set_parallelization (t_const_ptr<t_parallelization> parallelization);

    virtual void operator () (const structs::t_spectra & spectra,
                              t_ret_type & D,
                              const structs::t_spectra_filter * filter=NULL) const;

    virtual void calculate (const structs::t_spectra & spectra,
                            t_ret_type & D,
                            structs::t_spectra_filter & filter,
                            structs::t_candidate & candidate,
                            t_time_interval start_time=time_interval()) const;

    void update (const structs::t_spectra & spectra,
                 t_ret_type & D,
                 const t_ret_type & old_D,
                 const structs::t_spectra_filter & filter) const;


    static void combine (const structs::t_spectra & spectra,
                         structs::t_trie & D,
                         const structs::t_trie & D_first,
                         const structs::t_trie & D_second,
                         const structs::t_spectra_filter & filter_first,
                         const structs::t_spectra_filter & filter_second);

private:
    t_const_ptr<t_similarity> similarity;
    t_const_ptr<t_basic_cutoff> cutoff;
    t_const_ptr<t_parallelization> parallelization;
};
}
}

#endif