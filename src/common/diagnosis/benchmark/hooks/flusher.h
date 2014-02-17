#ifndef __FLUSHER_H_40804107857bfa70caee529936a3415e4bedd0ca__
#define __FLUSHER_H_40804107857bfa70caee529936a3415e4bedd0ca__

#include "diagnosis/benchmark/hooks/benchmark_hook.h"

namespace diagnosis {
namespace benchmark {
class t_flusher_hook : public t_benchmark_hook {
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