#ifndef __VERBOSE_HOOK_H__55534f6145f92c9bf1fd001e9682291868dc8331__
#define __VERBOSE_HOOK_H__55534f6145f92c9bf1fd001e9682291868dc8331__

#include "diagnosis/benchmark/benchmark_hook.h"

namespace diagnosis {
namespace benchmark {
class t_verbose_hook : public t_basic_benchmark_hook {
protected:
    virtual void _init (const structs::t_spectra & spectra,
                        const structs::t_candidate & correct);
    virtual void _cleanup ();

    virtual void _pre_gen ();
    virtual void _post_gen (t_candidate_generator::t_ret_type & D,
                            t_time_interval duration);

    virtual void _pre_rank ();
    virtual void _post_rank (const t_candidate_ranker::t_ret_type & probs,
                             t_time_interval duration);
};
}
}

#endif