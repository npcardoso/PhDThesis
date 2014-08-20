#ifndef __CONNECTION_H_db26b5a9e36de709ca3f5fa2f2ec2c699526d355__
#define __CONNECTION_H_db26b5a9e36de709ca3f5fa2f2ec2c699526d355__

#include "types.h"

#include <iostream>

namespace diagnostic {
class t_connection {
    public:
    t_connection (t_id from, t_id to);

    t_id get_from () const;
    t_id get_to () const;

    bool operator < (const t_connection & c) const;

    private:
    t_id from;
    t_id to;
};
}


namespace std {
ostream & operator << (ostream & out,
                       const diagnostic::t_connection & c);
}


#endif
