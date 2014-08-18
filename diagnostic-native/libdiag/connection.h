#ifndef __CONNECTION_H_db26b5a9e36de709ca3f5fa2f2ec2c699526d355__
#define __CONNECTION_H_db26b5a9e36de709ca3f5fa2f2ec2c699526d355__

#include <iostream>

namespace diagnostic {
class t_connection {
    public:
    inline t_connection (t_id from, t_id to) {
        this->from = from;
        this->to = to;
    }

    inline t_id get_from () const {
        return from;
    }

    inline t_id get_to () const {
        return to;
    }

    bool operator < (const t_connection & c) const {
        if(from == c.from)
            return to < c.to;
        return from < c.from;
    }


    private:
    t_id from;
    t_id to;

};
}


namespace std {
inline std::ostream & operator << (std::ostream & out,
                                   const diagnostic::t_connection & c) {
    out << c.get_from() << "->" << c.get_to();
    return out;
}
}


#endif
