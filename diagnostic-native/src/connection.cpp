#include "connection.h"

namespace diagnostic {
t_connection::t_connection (t_id from,
                            t_id to) {
    this->from = from;
    this->to = to;
}

t_id t_connection::get_from () const {
    return from;
}

t_id t_connection::get_to () const {
    return to;
}

bool t_connection::operator < (const t_connection & c) const {
    if(from == c.from)
        return to < c.to;
    return from < c.from;
}
}

namespace std {
ostream & operator << (ostream & out,
                       const diagnostic::t_connection & c) {
    out << c.get_from() << "->" << c.get_to();
    return out;
}
}
