#include "save_hook.h"

#include "utils/iostream.h"

#include <boost/lexical_cast.hpp>
#include <fstream>

namespace diagnosis {
namespace benchmark {
t_save_hook::t_save_hook (std::string d) : t_writer_hook(d) {}

void t_save_hook::_init (const structs::t_spectra & spectra,
                         const structs::t_candidate & correct) {
    t_path base_file(get_root_dir());


    base_file /= boost::lexical_cast<std::string> (get_iterations());
    base_file += ".spectra";

    std::ofstream f_spectra(base_file.c_str());
    f_spectra.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_spectra << spectra;
    f_spectra.close();

    base_file = get_root_dir();
    base_file /= boost::lexical_cast<std::string> (get_iterations());
    base_file += ".correct";

    std::ofstream f_correct(base_file.c_str());
    f_correct.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_correct << correct;
    f_correct.close();
}

void t_save_hook::_cleanup () {}

void t_save_hook::_pre_gen () {}

void t_save_hook::_post_gen (t_candidate_generator::t_ret_type & D,
                             t_time_interval duration) {
    t_path base_file(get_root_dir());


    base_file /= boost::lexical_cast<std::string> (get_iterations());
    base_file += ".";
    base_file += boost::lexical_cast<std::string> (get_generator_id());
    base_file += ".D";

    std::ofstream f_D(base_file.c_str());
    f_D.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_D << D;
    f_D.close();
}

void t_save_hook::_pre_rank () {}

void t_save_hook::_post_rank (const t_candidate_ranker::t_ret_type & probs,
                              t_time_interval duration) {
    t_path base_file(get_root_dir());


    base_file /= boost::lexical_cast<std::string> (get_iterations());
    base_file += ".";
    base_file += boost::lexical_cast<std::string> (get_generator_id());
    base_file += ".";
    base_file += boost::lexical_cast<std::string> (get_ranker_id());
    base_file += ".Probs";

    std::ofstream f(base_file.c_str());
    f.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f << probs;
    f.close();
}
}
}