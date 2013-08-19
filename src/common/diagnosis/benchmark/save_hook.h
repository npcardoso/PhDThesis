#ifndef __SAVE_HOOK_H__9bfc7dc852528cb46b831eae7a7f4e308bf88de4__
#define __SAVE_HOOK_H__9bfc7dc852528cb46b831eae7a7f4e308bf88de4__

#include "diagnosis/benchmark/writer_hook.h"

namespace diagnosis {
namespace benchmark {
class t_save_hook : public t_writer_hook {
public:

    t_save_hook (std::string d);

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

private:
    const t_candidate_generator::t_ret_type * D;
};
}
}
#endif