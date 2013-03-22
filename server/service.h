#ifndef __SERVICE_H__
#define __SERVICE_H__

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp> 

#include <iostream>

typedef boost::shared_ptr<std::iostream> t_iostream_ptr;

class t_service {
public:
  typedef boost::shared_ptr<t_service> t_ptr;

  virtual void operator()(const t_iostream_ptr & in,
                          const t_iostream_ptr & out);

  virtual void operator()(std::istream & in,
                          std::ostream & out) = 0;
};

class t_threaded_service: public t_service {
  boost::thread_group workers;
  t_service::t_ptr srv;

  void launch(t_iostream_ptr * in,
              t_iostream_ptr * out);

public:
  t_threaded_service(t_service::t_ptr srv);

  void operator()(const t_iostream_ptr & in,
                  const t_iostream_ptr & out);

  virtual void operator ()(std::istream & in,
                           std::ostream & out);
};

class t_echo_service: public t_service {
public:
  virtual void operator ()(std::istream & in,
                           std::ostream & out);
};

#endif
