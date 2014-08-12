#include "benchmark_settings.h"

namespace diagnostic {
namespace benchmark {
t_benchmark_settings::t_benchmark_settings (const t_const_ptr<t_diagnostic_system> & diagnostic_system,
                                            const t_ptr<t_collector> & collector,
                                            const t_const_ptr<t_benchmark_hook> & hook,
                                            const t_ptr<t_job_queue> & job_queue) : collector(collector), hook(hook), job_queue(job_queue) {
    assert(diagnostic_system.get() != NULL);
    assert(collector.get() != NULL);
    assert(hook.get() != NULL);
    assert(job_queue.get() != NULL);
}

const t_diagnostic_system & t_benchmark_settings::get_diagnostic_system() const {
    return *diagnostic_system;
}


t_collector & t_benchmark_settings::get_collector () const {
    return *collector;
}

const t_benchmark_hook & t_benchmark_settings::get_hook () const {
    return *hook;
}

t_job_queue & t_benchmark_settings::get_job_queue () const {
    return *job_queue;
}
}
}
