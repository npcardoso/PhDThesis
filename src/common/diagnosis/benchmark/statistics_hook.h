#ifndef __STATISTICS_HOOK_H__9ef8a1ba45feaff2724b5ce2e9bc7b4a111b52b5__
#define __STATISTICS_HOOK_H__9ef8a1ba45feaff2724b5ce2e9bc7b4a111b52b5__
#include "diagnosis/benchmark/writer_hook.h"

namespace diagnosis {
namespace benchmark {
class t_statistics_hook : public t_writer_hook {
public:
    t_statistics_hook (std::string d);

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