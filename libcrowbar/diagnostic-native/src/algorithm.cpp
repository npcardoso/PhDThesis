#include "candidate_generator.h"

namespace diagnostic {
std::ostream & t_algorithm::json (std::ostream & out) const {
    out << '{';
    json_write(out, "type") << ':';
    json_write(out, get_name());

    t_configs configs;
    json_configs(configs);
    if(configs.size()) {
        out << ',';
        json_write(out, "configs") << ':';
        json_write(out, configs);
    }
    out << '}';
    return out;
}
}

namespace std {
ostream& operator<<(ostream & out,
                    const diagnostic::t_algorithm & a) {
    out << "t_" << a.get_name();
    return out;
}

}
