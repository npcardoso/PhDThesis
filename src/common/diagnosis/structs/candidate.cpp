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

std::ostream & t_candidate::print (std::ostream & out) const {
    BOOST_FOREACH(const value_type &c,
                  *this) {
        out << c << " ";
    }
    return out << "0";
}
}
}