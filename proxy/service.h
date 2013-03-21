#ifndef __SERVICE_H__
#define __SERVICE_H__

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp> 

#include <iostream>
  typedef boost::shared_ptr<std::iostream> t_iostream_ptr;

class t_service {
public:
  typedef boost::shared_ptr<t_service> t_ptr;

  virtual void operator ()(t_iostream_ptr & stream);
  virtual void operator ()(std::iostream & stream) = 0;
};

class t_threaded_service: public t_service {
  boost::thread_group workers;
  t_service::t_ptr srv;

  void launch(t_iostream_ptr * stream);

public:
  t_threaded_service(t_service::t_ptr srv);
  virtual void operator ()(t_iostream_ptr & stream);
  virtual void operator ()(std::iostream & stream);
};

class t_echo_service: public t_service {
public:
  virtual void operator ()(std::iostream & stream);
};

class t_json_parser_service: public t_service {
public:
  virtual void operator ()(std::iostream & stream);
};
#endif
