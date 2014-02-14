#include "flusher.h"
namespace diagnosis {
namespace benchmark {
void t_flusher_hook::trigger_event (t_collector & collector,
                                    const randomizers::t_system & system) const {
    collector.flush_all();
}

void t_flusher_hook::trigger_event (t_collector & collector,
                                    const t_status_iteration_init & status) const {
    collector.flush_all();
}

void t_flusher_hook::trigger_event (t_collector & collector,
                                    const t_status_post_gen & status) const {
    collector.flush_all();
}

void t_flusher_hook::trigger_event (t_collector & collector,
                                    const t_status_post_rank & status) const {
    collector.flush_all();
}
}
}