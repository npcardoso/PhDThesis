#include "save_hook.h"

#include "diagnosis/structs/diagnosis_report.h"
#include "utils/iostream.h"

#include <boost/lexical_cast.hpp>
#include <fstream>

namespace diagnosis {
namespace benchmark {
void t_save_hook::init_system (t_collector & collector,
                               const randomizers::t_system & system) const {
    /*    std::ofstream f;
     *
     *
     *  open_file(boost::lexical_cast<std::string> (get_system_count()) + ".system.txt", f);
     *  f << system;
     *  f.close();
     */
}

void t_save_hook::init (t_collector & collector,
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

void t_save_hook::post_gen (t_collector & collector,
                            const t_status_post_gen & status) const {
    std::stringstream ss;
    t_entry entry;


    status.prepare_entry(entry);

    ss << status.get_candidates();
    collector.save_file(collector.local_path(entry, "D.txt"),
                        ss.str());
}

void t_save_hook::post_rank (t_collector & collector,
                             const t_status_post_rank & status) const {
    std::stringstream ss;
    t_entry entry;


    status.prepare_entry(entry);

    ss << status.get_probs();
    collector.save_file(collector.local_path(entry, "probs.txt"),
                        ss.str());

    ss.str("");
    ss << structs::t_diagnosis_report(status.get_candidates(), status.get_probs());
    collector.save_file(collector.local_path(entry, "report.txt"),
                        ss.str());
}
}
}