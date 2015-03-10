#include "candidate.h"

#include "utils/iostream.h"

#include <boost/foreach.hpp>
#include <cstdarg>

namespace diagnostic {
t_candidate::t_candidate (int c,
                          ...) {
    va_list vl;


    va_start(vl, c);

    do {
        insert(c);
    } while ((c = va_arg(vl, t_id)));

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
    t_group group;


    group.open(out, prefix, suffix, separator);
    BOOST_FOREACH(const auto & el, *this) {
        group.element(out);
        out << el;
    }
    group.close(out);

    return out;
}

std::ostream & t_candidate::write (std::ostream & out) const {
    return generic_write(out,
                         CANDIDATE_NORMAL_PREFIX,
                         CANDIDATE_NORMAL_SUFFIX,
                         CANDIDATE_NORMAL_SEP);
}

std::ostream & t_candidate::pretty_write (std::ostream & out) const {
    return generic_write(out,
                         CANDIDATE_PRETTY_PREFIX,
                         CANDIDATE_PRETTY_SUFFIX,
                         CANDIDATE_PRETTY_SEP);
}

std::ostream & t_candidate::latex_write (std::ostream & out) const {
    return generic_write(out,
                         CANDIDATE_LATEX_PREFIX,
                         CANDIDATE_LATEX_SUFFIX,
                         CANDIDATE_LATEX_SEP);
}

std::ostream & t_candidate::json (std::ostream & out) const {
    t_group group;


    group.open(out, "[", "]", ",");
    BOOST_FOREACH(const auto & el, *this) {
        group.element(out);
        json_write(out, el - 1); // In the serialized format, the indices are 0-based
    }
    group.close(out);
    return out;
}
}

namespace std {
istream & operator >> (istream & in,
                       t_candidate & candidate) {
    return candidate.read(in);
}

ostream & operator << (ostream & out,
                       const t_candidate & candidate) {
    return candidate.write(out);
}
}