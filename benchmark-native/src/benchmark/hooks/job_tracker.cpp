#include "job_tracker.h"

#include <boost/lexical_cast.hpp>


using namespace boost;
using namespace boost::posix_time;

namespace diagnostic {
namespace benchmark {
void t_job_tracker_hook::trigger_event (t_collector & collector,
                                        const t_status_post_gen & status) const {
    t_path file = collector.global_path("jobs.csv");
    t_entry entry;


    entry["type"] = status.get_gen_name();
    entry["init"] = lexical_cast<std::string> (status.get_iteration_start());
    entry["start"] = lexical_cast<std::string> (status.get_gen_start());
    entry["end"] = lexical_cast<std::string> (status.get_gen_end());
    entry["end_hooks"] = lexical_cast<std::string> (time_interval());
    collector.add_entry(file, entry);
}

void t_job_tracker_hook::trigger_event (t_collector & collector,
                                        const t_status_post_rank & status) const {
    t_path file = collector.global_path("jobs.csv");
    t_entry entry;


    entry["type"] = status.get_rank_name();
    entry["init"] = lexical_cast<std::string> (status.get_gen_end());
    entry["start"] = lexical_cast<std::string> (status.get_rank_start());
    entry["end"] = lexical_cast<std::string> (status.get_rank_end());
    entry["end_hooks"] = lexical_cast<std::string> (time_interval());
    collector.add_entry(file, entry);
}
}
}