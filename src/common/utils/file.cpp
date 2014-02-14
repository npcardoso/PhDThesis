#include "file.h"

#include <boost/filesystem.hpp>
using namespace std;
using namespace boost::filesystem;

t_file::t_file (const t_path & path) {
    this->path = path;
}

// void t_file::flush() {}

const t_path & t_file::get_path () const {
    return path;
}

bool t_file::open_file (ifstream & stream) {
    if (!exists(path))
        return false;

    stream.open(path.string().c_str());
    return true;
}

bool t_file::open_file (ofstream & stream) {
    if (!exists(path))
        create_directories(path.parent_path());

    stream.open(path.string().c_str());
    return true;
}

bool t_file::operator < (const t_file & other) const {
    return path < other.path;
}