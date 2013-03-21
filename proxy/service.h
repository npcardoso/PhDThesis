#ifndef __SERVICE_H__
#define __SERVICE_H__

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp> 

#include <iostream>

class t_service {
public:
  typedef boost::shared_ptr<t_service> t_ptr;

  virtual void operator ()(std::iostream * stream) = 0;
};

class t_threaded_service: public t_service {
  boost::thread_group workers;
  t_service::t_ptr srv;
public:
  t_threaded_service(t_service::t_ptr srv);
  virtual void operator ()(std::iostream * stream);
};

class t_echo_service: public t_service {
  virtual void operator ()(std::iostream * stream);
};
#endif
