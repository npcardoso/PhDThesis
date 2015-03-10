#ifndef __ALGORITHM_H_7d7849de363d21e25176e228e59862c5c1c7eb3f__
#define __ALGORITHM_H_7d7849de363d21e25176e228e59862c5c1c7eb3f__

#include "algorithm_creator.h"

namespace diagnostic {
namespace configuration {
/**
 * This class stores the algorithm name and configurations.  It is
 * mainly used as representation for the JSON's algorithm
 * object.
 */
class t_algorithm {
public:
    typedef std::map<std::string, std::string> t_configs;

    t_algorithm (std::string name,
                 t_const_ptr<t_configs> configs);

    /**
     * @brief Returns the algorithm's name.
     */
    const std::string & get_name () const;

    /**
     * @brief Returns the algorithm's configurations.
     */
    const t_configs & get_configs () const;

    static t_ptr<t_configs> new_configs ();

private:
    std::string name;
    t_const_ptr<t_configs> configs;
};
}
}

#endif