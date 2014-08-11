#include "path_generator.h"

#include "status.h"

#include <boost/foreach.hpp>

namespace diagnosis {
namespace benchmark {
t_path_generator::t_path_generator (std::string root_dir) : root_dir(root_dir) {
    relevant_keys.push_back(STATUS_KEY_ITERATION);
    relevant_keys.push_back(STATUS_KEY_GENERATOR);
    relevant_keys.push_back(STATUS_KEY_RANKER);
}

t_path_single_dir::t_path_single_dir (std::string root_dir) : t_path_generator(root_dir) {}

t_path t_path_single_dir::operator () (std::string filename) const {
    t_path p(root_dir);


    p /= filename;

    return p;
}

t_path t_path_single_dir::operator () (const t_entry & entry,
                                       std::string filename) const {
    t_path p(root_dir);

    bool first = true;


    BOOST_FOREACH(std::string s, relevant_keys) {
        t_entry::const_iterator it = entry.find(s);


        if (it == entry.end())
            break;

        if (first)
            p /= it->second + ".";
        else
            p += it->second + ".";

        first = false;
    }

    if (first)
        p /= filename;
    else
        p += filename;

    return p;
}
}
}