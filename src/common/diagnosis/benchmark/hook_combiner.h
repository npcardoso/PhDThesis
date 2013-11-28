#ifndef __HOOK_COMBINER_H__20b4fccd457d03a26bb7b24e73393845efee8c11__
#define __HOOK_COMBINER_H__20b4fccd457d03a26bb7b24e73393845efee8c11__

#include "diagnosis/benchmark/benchmark_hook.h"

namespace diagnosis {
namespace benchmark {
class t_hook_combiner : public t_benchmark_hook {
public:
    t_hook_combiner & operator << (t_benchmark_hook * hook);
    t_hook_combiner & operator << (t_benchmark_hook::t_ptr & hook);

protected:
    virtual void init_randomizer (const randomizers::t_spectra_randomizer & randomizer);

    virtual void init (const structs::t_spectra & spectra,
                       const structs::t_candidate & correct);
    virtual void cleanup ();

    virtual void pre_gen (t_id generator_id,
                          const std::string & name);
    virtual void post_gen (t_candidate_generator::t_ret_type & D,
                           t_time_interval duration);

    virtual void pre_rank (t_id ranker_id,
                           const std::string & name);
    virtual void post_rank (const t_candidate_ranker::t_ret_type & probs,
                            t_time_interval duration);
private:
    typedef std::list<t_benchmark_hook::t_ptr> t_hook_list;
    t_hook_list hook_list;
};
}
}

#endif