#include "collector.h"

#include "utils/iostream.h"
#include <sstream>
#include <iostream>


namespace diagnosis {
namespace benchmark {
t_collector::t_collector (t_path_generator::t_const_ptr path_generator) : path_generator(path_generator) {}

void t_collector::add_entry (const t_path & file,
                             const t_entry & entry) {
    std::cerr << "Adding entry to \"";
    std::cerr << file << "\":" << std::endl;
    std::cerr << entry << std::endl;
}

void t_collector::save_file (const t_path & file,
                             const std::string & data) {
    std::cerr << "Saving File \"";
    std::cerr << file << "\":" << std::endl;
    std::cerr << data << std::endl;
}

void t_collector::debug (const t_status & status,
                         const std::string & data) {
    t_entry entry;


    status.prepare_entry(entry);
    std::stringstream ss(data);

    while (ss.good()) {
        std::string line;
        getline(ss, line);
        std::cerr << "Debug:" << entry << ":" << line << std::endl;
    }
}

t_path t_collector::local_path (const t_entry & entry,
                                const std::string & filename) const {
    return get_path_generator() (entry,
                                 filename);
}

t_path t_collector::global_path (const std::string & filename) const {
    return get_path_generator() (filename);
}

const t_path_generator & t_collector::get_path_generator () const {
    return *path_generator;
}
}
}