#ifndef __TYPES_H_31ac11d42bdee89ffbb12d344b9e13ba12593f63__
#define __TYPES_H_31ac11d42bdee89ffbb12d344b9e13ba12593f63__

#include <boost/filesystem/path.hpp>
#include <string>
#include <map>

namespace diagnosis {
namespace benchmark {
typedef std::map<std::string, std::string> t_entry;
typedef boost::filesystem::path t_path;
}
}

#endif