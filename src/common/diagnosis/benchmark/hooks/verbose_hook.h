#ifndef __VERBOSE_HOOK_H__55534f6145f92c9bf1fd001e9682291868dc8331__
#define __VERBOSE_HOOK_H__55534f6145f92c9bf1fd001e9682291868dc8331__

#include "diagnosis/benchmark/hooks/benchmark_hook.h"

namespace diagnosis {
namespace benchmark {
class t_verbose_hook : public t_benchmark_hook {
protected:
    virtual void init (t_collector & collector,
                       const t_status_iteration_init & status) const;

    virtual void post_gen (t_collector & collector,
                           const t_status_post_gen & status) const;

    virtual void post_rank (t_collector & collector,
                            const t_status_post_rank & status) const;
};
}
}

#endif