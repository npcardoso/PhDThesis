#ifndef __MHS_H_957e289b8923d6c40fa69c8aa7a53a6ed1da023b__
#define __MHS_H_957e289b8923d6c40fa69c8aa7a53a6ed1da023b__

#include "similarity.h"
#include "mhs/cutoff.h"
#include "mhs/parallelization.h"
#include "../diagnosis_system.h"
#include "../utils/time.h"

namespace diagnosis {
namespace algorithms {
class t_mhs : public t_candidate_generator {
public:
    t_mhs (t_const_ptr<t_similarity> similarity=t_const_ptr<t_similarity> ());

    void set_cutoff (t_const_ptr<t_basic_cutoff> cutoff);
    void set_parallelization (t_const_ptr<t_parallelization> parallelization);
    void set_similarity (t_const_ptr<t_similarity> similarity);

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
                         t_ret_type & D,
                         const t_ret_type & D_first,
                         const t_ret_type & D_second,
                         const structs::t_spectra_filter & filter_first,
                         const structs::t_spectra_filter & filter_second);

protected:
    t_const_ptr<t_similarity> similarity;
    t_const_ptr<t_basic_cutoff> cutoff;
    t_const_ptr<t_parallelization> parallelization;
};

class t_mhs_parallel : public t_mhs {
public:
    t_mhs_parallel (const t_const_ptr<t_parallelization_factory> pf,
                    t_count n_threads);


    virtual void operator () (const structs::t_spectra & spectra,
                              t_ret_type & D,
                              const structs::t_spectra_filter * filter=NULL) const;
private:
    class t_args {
public:
        t_ptr<t_mhs> mhs;
        const structs::t_spectra * spectra;
        const structs::t_spectra_filter * filter;
        t_ret_type D;
    };

    static void map (t_args * args);

private:
    t_count n_threads;
    t_const_ptr<t_parallelization_factory> pf;
};
}
}

#endif