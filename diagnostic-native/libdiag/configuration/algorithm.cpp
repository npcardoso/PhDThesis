#include "algorithm_creator.h"

namespace diagnostic {
namespace configuration {

t_algorithm::t_algorithm (std::string name,
                          t_const_ptr<t_configs> configs):name(name), configs(configs) {
    assert(configs.get() != NULL);
}

const std::string & t_algorithm::get_name() const {
    return name;
}

const t_algorithm::t_configs & t_algorithm::get_configs() const {
    return *configs;
}

t_ptr<t_algorithm::t_configs> t_algorithm::new_configs () {
    return t_ptr<t_configs>(new t_configs());
}

}
}
