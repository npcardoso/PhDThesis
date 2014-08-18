#include "candidate.h"

#include <boost/foreach.hpp>
#include <cstdarg>

namespace diagnostic {

t_candidate::t_candidate(int c, ...) {
    va_list vl;
    va_start(vl,c);
    do {
        insert(c);
    } while ((c = va_arg(vl,t_id)));
    va_end(vl);
}


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

std::ostream & t_candidate::generic_write (std::ostream & out,
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
