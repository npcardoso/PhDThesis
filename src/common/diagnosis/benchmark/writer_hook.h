#ifndef __WRITER_HOOK_H__5dcd1c68fbb3ce40c88feaff4f11c16403dfb70a__
#define __WRITER_HOOK_H__5dcd1c68fbb3ce40c88feaff4f11c16403dfb70a__

#include "diagnosis/benchmark/benchmark_hook.h"
#include <boost/filesystem.hpp>

namespace diagnosis {
namespace benchmark {
class t_writer_hook : public t_basic_benchmark_hook {
public:
    typedef boost::filesystem::path t_path;

    virtual void init_randomizer (const randomizers::t_spectra_randomizer & randomizer);
    virtual void init (const structs::t_spectra & spectra,
                       const structs::t_candidate & correct);
    virtual void cleanup ();


protected:
    t_writer_hook (std::string d);

    t_count get_iterations () const;
    t_count get_randomizers () const;

    // IO

    bool open_file (const t_path & rel_path, std::ofstream & f, bool append=false) const; // Returns true if file is new
    bool open_randomizer_file (const t_path & rel_path, std::ofstream & f, bool append=false) const; // Returns true if file is new
    bool open_iteration_file (const t_path & rel_path, std::ofstream & f, bool append=false) const; // Returns true if file is new
    void write_counter (const t_path & rel_path, t_count iterations) const;
    t_count read_counter (const t_path & rel_path) const;

private:
    const t_path & get_root_dir () const;

private:
    t_path root_dir;
    t_count iterations;
    t_count randomizers;
};
}
}

#endif