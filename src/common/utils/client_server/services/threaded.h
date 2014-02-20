#ifndef __SERVER_SERVICES_THREADED_H__
#define __SERVER_SERVICES_THREADED_H__

#include "utils/client_server/service.h"

#include <boost/thread.hpp>
// TODO: use std; use execution_controller

class t_threaded_service : public t_service {
    boost::thread_group workers;
    t_ptr<t_service> srv;

    void launch (t_ptr<std::iostream> * in,
                 t_ptr<std::iostream> * out);

public:
    t_threaded_service (t_ptr<t_service> srv);

    void operator () (const t_ptr<std::iostream> & in,
                      const t_ptr<std::iostream> & out);

    virtual void operator () (std::istream & in,
                              std::ostream & out);
};

#endif