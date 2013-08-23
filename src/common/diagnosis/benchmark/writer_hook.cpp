#include "writer_hook.h"

#include <boost/filesystem.hpp>
#include <fstream>

#define FILE_ITERATIONS "num_iterations.txt"
#define FILE_RANDOMIZERS "num_randomizers.txt"

namespace diagnosis {
namespace benchmark {
t_writer_hook::t_writer_hook (std::string d) : root_dir(d) {
    boost::filesystem::create_directories(root_dir);


    iterations = 0;
    randomizers = 0;
}

void t_writer_hook::init_randomizer (const randomizers::t_spectra_randomizer & randomizer) {
    randomizers = read_counter(FILE_RANDOMIZERS) + 1;
    t_basic_benchmark_hook::init_randomizer(randomizer);
}

void t_writer_hook::init (const structs::t_spectra & spectra,
                          const structs::t_candidate & correct) {
    assert(get_randomizers() > 0);

    iterations = read_counter(FILE_ITERATIONS) + 1;
    t_basic_benchmark_hook::init(spectra, correct);
}

void t_writer_hook::cleanup () {
    assert(get_iterations() > 0);
    assert(get_iterations() - read_counter(FILE_ITERATIONS) <= 1);
    assert(get_randomizers() - read_counter(FILE_RANDOMIZERS) <= 1);

    write_counter(FILE_ITERATIONS, get_iterations());
    write_counter(FILE_RANDOMIZERS, get_randomizers());
    t_basic_benchmark_hook::cleanup();
}

void t_writer_hook::write_counter (const t_path & rel_path, t_count iterations) const {
    boost::filesystem::path f_iterations(root_dir);


    f_iterations /= rel_path;

    std::ofstream f(f_iterations.c_str());
    f.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    f << iterations;
}

t_count t_writer_hook::read_counter (const t_path & rel_path) const {
    boost::filesystem::path f_iterations(root_dir);


    f_iterations /= rel_path;

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

t_count t_writer_hook::get_randomizers () const {
    return randomizers;
}
}
}