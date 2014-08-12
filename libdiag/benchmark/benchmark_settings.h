#ifndef __BENCHMARK_SETTINGS_H_6dc7d2b886ba77e1f76d2b0061b0ffaecab2961b__
#define __BENCHMARK_SETTINGS_H_6dc7d2b886ba77e1f76d2b0061b0ffaecab2961b__

#include "hooks/benchmark_hook.h"
#include "collector.h"
#include "../utils/multithread/job_queue.h"

#include <vector>
#include <map>
#include <list>


namespace diagnostic {
namespace benchmark {
class t_benchmark_settings {
public:
    t_benchmark_settings (const t_const_ptr<t_diagnostic_system> & diagnostic_system,
                          const t_ptr<t_collector> & collector,
                          const t_const_ptr<t_benchmark_hook> & hook,
                          const t_ptr<t_job_queue> & job_queue=t_ptr<t_job_queue> (new t_job_queue()));



    const t_diagnostic_system & get_diagnostic_system() const;
    t_collector & get_collector () const;
    const t_benchmark_hook & get_hook () const;
    t_job_queue & get_job_queue () const;


    private:
    t_const_ptr<t_diagnostic_system> diagnostic_system;
    t_ptr<t_collector> collector;
    t_const_ptr<t_benchmark_hook> hook;
    t_ptr<t_job_queue> job_queue;
};
}
}
#endif
