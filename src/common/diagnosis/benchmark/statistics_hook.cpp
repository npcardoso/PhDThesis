#include "statistics_hook.h"
#include "boost/date_time/posix_time/posix_time.hpp"

#include <fstream>

using namespace boost::posix_time;

namespace diagnosis {
namespace benchmark {
t_statistics_hook::t_statistics_hook (std::string d) : t_writer_hook(d) {}

void t_statistics_hook::_init (const structs::t_spectra & spectra,
                               const structs::t_candidate & correct) {}

void t_statistics_hook::_cleanup () {}

void t_statistics_hook::_pre_gen () {}

void t_statistics_hook::_post_gen (t_candidate_generator::t_ret_type & D,
                                   t_time_interval duration) {
    t_writer_hook::t_path fname(get_root_dir());
    bool new_file;


    fname /= "D.stats";
    new_file = !boost::filesystem::exists(fname);

    std::ofstream f(fname.c_str(), std::ios_base::app);
    f.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    // Print header on first iteration
    if (new_file)
        f << "date, generator, run, duration" << std::endl;

    f << "\"" << to_simple_string(second_clock::local_time()) << "\", " << get_generator_id() << ", " << get_iterations() << ", " << duration << std::endl;

    f.close();
}

void t_statistics_hook::_pre_rank () {}

void t_statistics_hook::_post_rank (const t_candidate_ranker::t_ret_type & probs,
                                    t_time_interval duration) {
    t_writer_hook::t_path fname(get_root_dir());
    bool new_file;


    fname /= "Probs.stats";
    new_file = !boost::filesystem::exists(fname);

    std::ofstream f(fname.c_str(), std::ios_base::app);
    f.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    // Print header on first iteration
    if (new_file)
        f << "date, generator, ranker, run, duration" << std::endl;

    f << "\"" << to_simple_string(second_clock::local_time()) << "\", " << get_generator_id() << ", " << get_ranker_id() << ", " << get_iterations() << ", " << duration << std::endl;

    f.close();
}
}
}