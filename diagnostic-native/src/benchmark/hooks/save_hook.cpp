#include "save_hook.h"

#include "../../report/ranking.h"
#include "../../utils/iostream.h"

#include <boost/lexical_cast.hpp>
#include <fstream>

namespace diagnostic {
namespace benchmark {
void t_save_hook::trigger_event (t_collector & collector,
                                 const t_status_iteration_init & status) const {
    std::stringstream ss;
    t_entry entry;


    status.prepare_entry(entry);


    ss << status.get_spectra();
    collector.save_file(collector.local_path(entry, "spectra.txt"),
                        ss.str());

    ss.str("");
    ss << status.get_correct();
    collector.save_file(collector.local_path(entry, "correct.txt"),
                        ss.str());
}

void t_save_hook::trigger_event (t_collector & collector,
                                 const t_status_post_gen & status) const {
    std::stringstream ss;
    t_entry entry;


    status.prepare_entry(entry);

    ss << status.get_candidates();
    collector.save_file(collector.local_path(entry, "D.txt"),
                        ss.str());
}

void t_save_hook::trigger_event (t_collector & collector,
                                 const t_status_post_rank & status) const {
    std::stringstream ss;
    t_entry entry;


    status.prepare_entry(entry);

    ss << status.get_probs();
    collector.save_file(collector.local_path(entry, "probs.txt"),
                        ss.str());

    ss.str("");
    t_ranking r (status.get_candidates(),
                 status.get_probs(),
                 HEURISTIC); //!FIXME: add something like status.get_ranker().get_score_type();
    ss << r;
    collector.save_file(collector.local_path(entry, "report.txt"),
                        ss.str());
}
}
}
