#ifndef __BENCHMARK_HOOK_H__0734b6b340e40970218b6c20df0194edd574e1c1__
#define __BENCHMARK_HOOK_H__0734b6b340e40970218b6c20df0194edd574e1c1__

#include "../status.h"
#include "../collector.h"


namespace diagnostic {
namespace benchmark {
class t_benchmark_hook {
public:
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
