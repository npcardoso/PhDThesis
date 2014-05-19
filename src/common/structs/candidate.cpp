#include "candidate.h"

#include <boost/foreach.hpp>

namespace diagnosis {
namespace structs {
std::istream & t_candidate::read (std::istream & in) {
    while (true) {
        t_component_id component = 0;
        in >> component;

        if (component == 0 || !in.good())
            break;

        insert(component);
    }

    return in;
}

std::ostream & t_candidate::generic_print (std::ostream & out,
                                           std::string prefix,
                                           std::string suffix,
                                           std::string separator) const {
    bool first = true;


    out << prefix;

    BOOST_FOREACH(const value_type &c,
                  *this) {
        if (!first)
            out << separator;

        out << c;
        first = false;
    }
    return out << suffix;
}
}
}