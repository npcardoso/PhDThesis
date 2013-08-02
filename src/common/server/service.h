#ifndef __SERVICE_H__
#define __SERVICE_H__

#include "utils/boost.h"

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>

typedef boost::shared_ptr < std::iostream >t_iostream_ptr;

class t_service {
public:
    DEFINE_BOOST_SHARED_PTRS(t_service);

    virtual void operator () (const t_iostream_ptr & in,
                              const t_iostream_ptr & out);

    virtual void operator () (std::istream & in,
                              std::ostream & out) = 0;
};

#endif