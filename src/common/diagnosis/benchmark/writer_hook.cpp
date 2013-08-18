#include "writer_hook.h"

#include <boost/filesystem.hpp>
#include <fstream>


namespace diagnosis {
namespace benchmark {
t_writer_hook::t_writer_hook (std::string d) : root_dir(d) {
    boost::filesystem::create_directories(root_dir);
}

void t_writer_hook::init (const structs::t_spectra & spectra,
                          const structs::t_candidate & correct) {
    iterations = read_iterations() + 1;
    t_basic_benchmark_hook::init(spectra, correct);
}

void t_writer_hook::cleanup () {
    std::cerr << "read_iterations(): " << read_iterations() << " get_iterations(): " << get_iterations() << std::endl;


    assert(get_iterations() - read_iterations() <= 1);
    write_iterations(get_iterations());
    t_basic_benchmark_hook::cleanup();
}

void t_writer_hook::write_iterations (t_count iterations) const {
    boost::filesystem::path f_iterations(root_dir);


    f_iterations /= "num_iteration.txt";

    std::ofstream f(f_iterations.c_str());
    f.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    f << iterations;
}

t_count t_writer_hook::read_iterations () const {
    boost::filesystem::path f_iterations(root_dir);


    f_iterations /= "num_iteration.txt";

    std::ifstream f(f_iterations.c_str());
    t_count iterations = 0;

    f >> iterations;

    return iterations;
}

const t_writer_hook::t_path & t_writer_hook::get_root_dir () const {
    return root_dir;
}

t_count t_writer_hook::get_iterations () const {
    return iterations;
}
}
}