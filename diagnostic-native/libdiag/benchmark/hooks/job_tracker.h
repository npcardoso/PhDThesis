#ifndef __JOB_TRACKER_H_3a988067b2a3459a561fa86477c1ef72baa65413__
#define __JOB_TRACKER_H_3a988067b2a3459a561fa86477c1ef72baa65413__

#include "benchmark_hook.h"


namespace diagnostic {
namespace benchmark {
class t_job_tracker_hook : public t_benchmark_hook {
public:
    virtual void trigger_event (t_collector & collector,
                                const t_status_post_gen & status) const;

    virtual void trigger_event (t_collector & collector,
                                const t_status_post_rank & status) const;
};
}
}
#endif