#include "hook_combiner.h"

#include <boost/foreach.hpp>

namespace diagnosis {
namespace benchmark {
void t_hook_combiner::trigger_event (t_collector & collector,
                                     const randomizers::t_system & system) const {
    BOOST_FOREACH(t_benchmark_hook::t_ptr hook, hook_list) {
        hook->trigger_event(collector, system);
    }
}

void t_hook_combiner::trigger_event (t_collector & collector,
                                     const t_status_iteration_init & status) const {
    BOOST_FOREACH(t_benchmark_hook::t_ptr hook, hook_list) {
        hook->trigger_event(collector, status);
    }
}

void t_hook_combiner::trigger_event (t_collector & collector,
                                     const t_status_post_gen & status) const {
    BOOST_FOREACH(t_benchmark_hook::t_ptr hook, hook_list) {
        hook->trigger_event(collector, status);
    }
}

void t_hook_combiner::trigger_event (t_collector & collector,
                                     const t_status_post_rank & status) const {
    BOOST_FOREACH(t_benchmark_hook::t_ptr hook, hook_list) {
        hook->trigger_event(collector, status);
    }
}

t_hook_combiner & t_hook_combiner::operator << (t_benchmark_hook * hook) {
    hook_list.push_back(t_benchmark_hook::t_ptr(hook));
    return *this;
}

t_hook_combiner & t_hook_combiner::operator << (t_benchmark_hook::t_ptr & hook) {
    hook_list.push_back(hook);
    return *this;
}
}
}