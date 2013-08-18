#ifndef __BENCHMARK_HOOK_H__0734b6b340e40970218b6c20df0194edd574e1c1__
#define __BENCHMARK_HOOK_H__0734b6b340e40970218b6c20df0194edd574e1c1__

#include "diagnosis/diagnosis_system.h"
#include "diagnosis/structs/spectra.h"
#include "utils/time.h"


namespace diagnosis {
namespace benchmark {
class t_benchmark_hook {
public:
    DEFINE_BOOST_SHARED_PTRS(t_benchmark_hook);

    virtual void init (const structs::t_spectra & spectra,
                       const structs::t_candidate & correct) = 0;
    virtual void cleanup () = 0;

    virtual void pre_gen (t_id generator_id) = 0;
    virtual void post_gen (t_candidate_generator::t_ret_type & D,
                           t_time_interval duration) = 0;

    virtual void pre_rank (t_id ranker_id) = 0;
    virtual void post_rank (const t_candidate_ranker::t_ret_type & probs,
                            t_time_interval duration) = 0;
};

class t_basic_benchmark_hook : public t_benchmark_hook {
public:
    t_basic_benchmark_hook ();

    virtual void init (const structs::t_spectra & spectra,
                       const structs::t_candidate & correct);
    virtual void cleanup ();

    virtual void pre_gen (t_id generator_id);
    virtual void post_gen (t_candidate_generator::t_ret_type & D,
                           t_time_interval duration);

    virtual void pre_rank (t_id ranker_id);
    virtual void post_rank (const t_candidate_ranker::t_ret_type & probs,
                            t_time_interval duration);

protected:
    virtual void _init (const structs::t_spectra & spectra,
                        const structs::t_candidate & correct) = 0;

    virtual void _cleanup () = 0;

    virtual void _pre_gen () = 0;
    virtual void _post_gen (t_candidate_generator::t_ret_type & D,
                            t_time_interval duration) = 0;

    virtual void _pre_rank () = 0;
    virtual void _post_rank (const t_candidate_ranker::t_ret_type & probs,
                             t_time_interval duration) = 0;

    t_id get_generator_id () const;
    t_id get_ranker_id () const;

private:
    t_id generator_id;
    t_id ranker_id;
};
}
}

#endif