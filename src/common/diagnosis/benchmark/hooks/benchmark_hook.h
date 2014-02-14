#ifndef __BENCHMARK_HOOK_H__0734b6b340e40970218b6c20df0194edd574e1c1__
#define __BENCHMARK_HOOK_H__0734b6b340e40970218b6c20df0194edd574e1c1__

#include "diagnosis/benchmark/status.h"
#include "diagnosis/benchmark/collector.h"
#include "diagnosis/randomizers/randomizer.h"
#include "diagnosis/diagnosis_system.h"
#include "diagnosis/structs/spectra.h"
#include "utils/time.h"


namespace diagnosis {
namespace benchmark {
class t_benchmark_hook {
public:
    DEFINE_BOOST_SHARED_PTRS(t_benchmark_hook);

    inline virtual void trigger_event (t_collector & collector,
                                       const randomizers::t_system & system) const {}

    inline virtual void trigger_event (t_collector & collector,
                                       const t_status_iteration_init & status) const {}

    inline virtual void trigger_event (t_collector & collector,
                                       const t_status_post_gen & status) const {}

    inline virtual void trigger_event (t_collector & collector,
                                       const t_status_post_rank & status) const {}

    inline virtual ~t_benchmark_hook () {}
};
}
}

#endif