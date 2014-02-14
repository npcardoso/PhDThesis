#include "metrics_hook.h"

#include <boost/foreach.hpp>
#include <fstream>

namespace diagnosis {
namespace benchmark {
t_metrics_hook & t_metrics_hook::operator << (t_metric::t_ptr & metric) {
    metrics_list.push_back(metric);
    return *this;
}

t_metrics_hook & t_metrics_hook::operator << (t_metric * metric) {
    metrics_list.push_back(t_metric::t_ptr(metric));
    return *this;
}

void t_metrics_hook::trigger_event (t_collector & collector,
                                    const t_status_post_rank & status) const {
    structs::t_diagnosis_report dr(status.get_candidates(),
                                   status.get_probs());

    t_entry entry;
    std::string filename("metrics.csv");


    BOOST_FOREACH(const t_metrics_list::value_type & metric,
                  metrics_list) {
        std::string ret = (* metric)(status.get_spectra(),
                                     status.get_correct(),
                                     status.get_candidates(),
                                     status.get_probs(),
                                     dr,
                                     entry);


        assert(entry.count(metric->key()) == 0);
        entry[metric->key()] = ret;
    }
    status.prepare_entry(entry);
    collector.add_entry(collector.global_path(filename),
                        entry);
}
}
}