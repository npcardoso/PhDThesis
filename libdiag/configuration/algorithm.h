#ifndef __ALGORITHM_H_7d7849de363d21e25176e228e59862c5c1c7eb3f__
#define __ALGORITHM_H_7d7849de363d21e25176e228e59862c5c1c7eb3f__

#include "algorithm_creator.h"

namespace diagnostic {
namespace configuration {
class t_algorithm {
    public:
    typedef std::map<std::string, std::string> t_configs;

    t_algorithm (std::string name,
                 t_const_ptr<t_configs> configs);

    const std::string & get_name() const;

    const t_configs & get_configs() const;

    static t_ptr<t_configs> new_configs ();

    private:
    std::string name;
    t_const_ptr<t_configs> configs;
};
}
}

#endif
