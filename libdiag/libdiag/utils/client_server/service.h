#ifndef __SERVICE_H_5383b2edd339073a56d2c84b446ff83e08bb467c__
#define __SERVICE_H_5383b2edd339073a56d2c84b446ff83e08bb467c__

#include "../boost.h"

#include <iostream>

class t_service {
public:
    virtual void operator () (const t_ptr<std::istream> & in,
                              const t_ptr<std::ostream> & out) = 0;

    virtual void operator () (std::istream & in,
                              std::ostream & out) = 0;
};

#endif