#ifndef __STATISTICS_HOOK_H__9ef8a1ba45feaff2724b5ce2e9bc7b4a111b52b5__
#define __STATISTICS_HOOK_H__9ef8a1ba45feaff2724b5ce2e9bc7b4a111b52b5__
#include "diagnosis/benchmark/hooks/benchmark_hook.h"

namespace diagnosis {
namespace benchmark {
class t_statistics_hook : public t_benchmark_hook {
public:
    virtual void trigger_event (t_collector & collector,
                                const t_status_iteration_init & status) const;

    virtual void trigger_event (t_collector & collector,
                                const t_status_post_gen & status) const;

    virtual void trigger_event (t_collector & collector,
                                const t_status_post_rank & status) const;
};
}
}

#endif