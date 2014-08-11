#include "verbose_hook.h"
#include <sstream>

namespace diagnosis {
namespace benchmark {
void t_verbose_hook::trigger_event (t_collector & collector,
                                    const t_status_iteration_init & status) const {
    std::stringstream ss;


    ss << "---Init Hook---" << std::endl;
    ss << status.get_spectra() << std::endl;
    ss << "Correct Candidate: " << status.get_correct() << std::endl;
    collector.debug(status, ss.str());
}

void t_verbose_hook::trigger_event (t_collector & collector,
                                    const t_status_post_gen & status) const {
    std::stringstream ss;


    ss << "---Post_gen Hook---" << std::endl;
    ss << "|D|: " << status.get_candidates().size() << std::endl;
    ss << "Took " << status.get_gen_duration() << "ms" << std::endl;
    collector.debug(status, ss.str());
}

void t_verbose_hook::trigger_event (t_collector & collector,
                                    const t_status_post_rank & status) const {
    std::stringstream ss;


    ss << "---Post_rank Hook---" << std::endl;
    ss << "Took " << status.get_rank_duration() << "ms" << std::endl;
    collector.debug(status, ss.str());
}
}
}