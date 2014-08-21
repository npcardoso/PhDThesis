#include "json.h"

#include <boost/foreach.hpp>
#include <iostream>
#include <cmath>

namespace diagnostic{

std::ostream & json_write (std::ostream & out,
                           const char * i) {
    return json_write(out, std::string(i));
}


std::ostream & json_write (std::ostream & out,
                           const t_json_writable & w) {
    return w.json(out);
}


std::ostream & json_write (std::ostream & out,
                           const bool & i) {
    out << (i?"true":"false");
    return out;
}


std::ostream & json_write (std::ostream & out, const int & i) {
    out << i;
    return out;
}

std::ostream & json_write (std::ostream & out,
                           const unsigned int & i) {
    out << i;
    return out;
}

std::ostream & json_write (std::ostream & out, const long & i) {
    out << i;
    return out;
}


std::ostream & json_write (std::ostream & out, const double & i) {
    if(std::isnan(i))
        out << "NaN";
    else
        out << i;
    return out;
}


std::ostream & json_write (std::ostream & out, std::string str) {
    out << '"';
    BOOST_FOREACH (auto c, str) {
        switch (c) {
            case '\\': out << "\\\\"; break;
            case '"':  out << "\\\""; break;
            case '/':  out << "\\/"; break;
            case '\b': out << "\\b"; break;
            case '\f': out << "\\f"; break;
            case '\n': out << "\\n"; break;
            case '\r': out << "\\r"; break;
            case '\t': out << "\\t"; break;
            default: out << c; break;
        }
    }
    out << '"';
    return out;
}
}
