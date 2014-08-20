#ifndef __SERVER_SERVICES_ECHO_H__
#define __SERVER_SERVICES_ECHO_H__

#include "../service.h"

class t_echo_service : public t_service {
public:
    virtual void operator () (std::istream & in,
                              std::ostream & out);
};

#endif