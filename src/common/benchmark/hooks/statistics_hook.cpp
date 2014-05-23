#include "statistics_hook.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost;
using namespace boost::posix_time;

namespace diagnosis {
namespace benchmark {
void t_statistics_hook::trigger_event (t_collector & collector,
                                       const t_status_iteration_init & status) const {
    t_path file = collector.global_path("stats.spectra.csv");
    t_entry entry;


    status.prepare_entry(entry);

    entry["date"] = to_simple_string(second_clock::local_time());
    entry["component_count"] = lexical_cast<std::string> (status.get_spectra().get_component_count());
    entry["transaction_count"] = lexical_cast<std::string> (status.get_spectra().get_transaction_count());
    entry["suspicious_count"] = lexical_cast<std::string> (status.get_spectra().get_suspicious_components_count());
    entry["activation_rate"] = lexical_cast<std::string> (status.get_spectra().get_activation_rate());
    entry["error_count"] = lexical_cast<std::string> (status.get_spectra().get_error_count());
    entry["error_rate"] = lexical_cast<std::string> (status.get_spectra().get_error_rate());

    collector.add_entry(file, entry);
}

void t_statistics_hook::trigger_event (t_collector & collector,
                                       const t_status_post_gen & status) const {
    t_path file = collector.global_path("stats.D.csv");
    t_entry entry;


    status.prepare_entry(entry);

    entry["date"] = to_simple_string(second_clock::local_time());
    entry["duration"] = lexical_cast<std::string> (status.get_gen_duration());
    entry["size"] = lexical_cast<std::string> (status.get_candidates().size());

    t_count avg_size = 0;
    t_count minimal = 0;
    BOOST_FOREACH(auto & d, status.get_candidates()) {
        avg_size += d.size();

        if (status.get_spectra().is_minimal_candidate(d))
            minimal++;
    }
    entry["avg_card"] = lexical_cast<std::string> (avg_size / status.get_candidates().size());
    entry["perc_minimal"] = lexical_cast<std::string> (minimal / status.get_candidates().size());

    collector.add_entry(file, entry);
}

void t_statistics_hook::trigger_event (t_collector & collector,
                                       const t_status_post_rank & status) const {
    t_path file = collector.global_path("stats.probs.csv");
    t_entry entry;


    status.prepare_entry(entry);


    entry["date"] = to_simple_string(second_clock::local_time());
    entry["duration"] = lexical_cast<std::string> (status.get_rank_duration());
    entry["size"] = lexical_cast<std::string> (status.get_candidates().size());

    collector.add_entry(file, entry);
}
}
}