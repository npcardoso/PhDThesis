#include "metrics_hook.h"

#include <boost/foreach.hpp>
#include <fstream>

namespace diagnostic {
namespace benchmark {
void t_metrics_hook::trigger_event (t_collector & collector,
                                    const t_status_post_rank & status) const {
    t_ranking r (status.get_candidates(),
                 status.get_probs(),
                 HEURISTIC); //!FIXME: add something linke status.get_ranker().get_score_type();

    t_entry entry;
    std::string filename("metrics.csv");


    BOOST_FOREACH(auto & metric, *this) {
        std::string ret = (* metric)(status.get_spectrum(),
                                     status.get_correct(),
                                     status.get_candidates(),
                                     status.get_probs(),
                                     r,
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
