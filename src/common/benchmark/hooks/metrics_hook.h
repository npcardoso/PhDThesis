#ifndef __METRICS_HOOK_H_0b0885703861f344448f13adf9cea1633b73bc5c__
#define __METRICS_HOOK_H_0b0885703861f344448f13adf9cea1633b73bc5c__

#include "benchmark/hooks/benchmark_hook.h"
#include "benchmark/metrics.h"


namespace diagnosis {
namespace benchmark {
class t_metrics_hook : public t_benchmark_hook, public std::list<t_const_ptr<t_metric> > {
public:
    virtual void trigger_event (t_collector & collector,
                                const t_status_post_rank & status) const;
};
}
}

#endif