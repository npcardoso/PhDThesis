#include "save_hook.h"

#include "diagnosis/structs/diagnosis_report.h"
#include "utils/iostream.h"

#include <boost/lexical_cast.hpp>
#include <fstream>

namespace diagnosis {
namespace benchmark {
t_save_hook::t_save_hook (std::string d) : t_writer_hook(d) {
    D = NULL;
}

void t_save_hook::_init_randomizer (const randomizers::t_spectra_randomizer & randomizer) {
    std::ofstream f;


    open_file(boost::lexical_cast<std::string> (get_randomizers()) + ".randomizer.txt", f);
    f << randomizer;
    f.close();
}

void t_save_hook::_init (const structs::t_spectra & spectra,
                         const structs::t_candidate & correct) {
    std::ofstream f;


    open_iteration_file("spectra.txt", f);
    f << spectra;
    f.close();

    open_iteration_file("correct.txt", f);
    f << correct;
    f.close();
}

void t_save_hook::_cleanup () {}

void t_save_hook::_pre_gen () {}

void t_save_hook::_post_gen (t_candidate_generator::t_ret_type & D,
                             t_time_interval duration) {
    std::ofstream f;


    open_iteration_file(boost::lexical_cast<std::string> (get_generator_name()) + ".D.txt", f);
    f << D;
    f.close();

    this->D = &D;
}

void t_save_hook::_pre_rank () {}

void t_save_hook::_post_rank (const t_candidate_ranker::t_ret_type & probs,
                              t_time_interval duration) {
    std::ofstream f;


    open_iteration_file(boost::lexical_cast<std::string> (get_generator_name()) + "+" +
                        boost::lexical_cast<std::string> (get_ranker_name()) + ".probs.txt", f);
    f << probs;
    f.close();

    structs::t_diagnosis_report diagnosis_rep(*D, probs);
    open_iteration_file(boost::lexical_cast<std::string> (get_generator_name()) + "+" +
                        boost::lexical_cast<std::string> (get_ranker_name()) + ".report.txt", f);
    f << diagnosis_rep;
    f.close();
}
}
}