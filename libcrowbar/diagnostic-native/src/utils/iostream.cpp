#include "iostream.h"


t_group::t_stack_element::t_stack_element (std::string rbracket,
                                           std::string sep) : first(true), rbracket(rbracket), sep(sep) {}

std::ostream & t_group::open (std::ostream & out,
                              std::string lbracket,
                              std::string rbracket,
                              std::string sep) {
    s.push(t_stack_element(rbracket, sep));
    out << lbracket;
    return out;
}

std::ostream & t_group::element (std::ostream & out) {
    if (!s.top().first)
        out << s.top().sep;

    s.top().first = false;
    return out;
}

std::ostream & t_group::close (std::ostream & out) {
    out << s.top().rbracket;
    s.pop();
    return out;
}