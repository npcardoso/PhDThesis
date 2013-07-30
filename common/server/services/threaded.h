#ifndef __SERVER_SERVICES_THREADED_H__
#define __SERVER_SERVICES_THREADED_H__

#include "server/service.h"

#include <boost/thread.hpp>

class t_threaded_service : public t_service {
    boost::thread_group workers;
    t_service::t_ptr srv;

    void launch (t_iostream_ptr * in,
                 t_iostream_ptr * out);

public:
    t_threaded_service (t_service::t_ptr srv);

    void operator () (const t_iostream_ptr & in,
                      const t_iostream_ptr & out);

    virtual void operator () (std::istream & in,
                              std::ostream & out);
};

#endif