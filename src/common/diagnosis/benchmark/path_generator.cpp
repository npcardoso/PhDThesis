#include "path_generator.h"

#include <boost/foreach.hpp>

using namespace boost::filesystem;

namespace diagnosis {
namespace benchmark {
t_path_single_dir::t_path_single_dir (std::string root_dir) : root_dir(root_dir) {}

boost::filesystem::path t_path_single_dir::operator () (std::string filename) const {
    path p(root_dir);


    p /= filename;

    return p;
}

boost::filesystem::path t_path_single_dir::operator () (const t_entry & entry,
                                                        std::string filename) const {
    path p(root_dir);

    bool first = true;


    BOOST_FOREACH(t_entry::value_type s, entry) {
        if (first)
            p /= s.second + ".";
        else
            p += s.second + ".";

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