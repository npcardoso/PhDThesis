#ifndef __HOOK_COMBINER_H__20b4fccd457d03a26bb7b24e73393845efee8c11__
#define __HOOK_COMBINER_H__20b4fccd457d03a26bb7b24e73393845efee8c11__

#include "diagnosis/benchmark/hooks/benchmark_hook.h"

namespace diagnosis {
namespace benchmark {
class t_hook_combiner : public t_benchmark_hook {
public:
    virtual void init_system (t_collector & collector,
                              const randomizers::t_system & system) const;

    virtual void init (t_collector & collector,
                       const t_status_iteration_init & status) const;

    virtual void post_gen (t_collector & collector,
                           const t_status_post_gen & status) const;

    virtual void post_rank (t_collector & collector,
                            const t_status_post_rank & status) const;

    t_hook_combiner & operator << (t_benchmark_hook * hook);
    t_hook_combiner & operator << (t_benchmark_hook::t_ptr & hook);

private:
    typedef std::list<t_benchmark_hook::t_ptr> t_hook_list;
    t_hook_list hook_list;
};
}
}

#endif