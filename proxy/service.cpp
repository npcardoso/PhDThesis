#include "service.h"

#include "utils/debug.h"

#include <boost/thread/mutex.hpp>
#include <boost/foreach.hpp>

void t_service::operator ()(t_iostream_ptr & stream) {
  (*this)(*stream);
}

t_threaded_service::t_threaded_service(t_service::t_ptr srv):srv(srv) {
}

void t_threaded_service::launch(t_iostream_ptr * stream) {
  try {
    (*srv)(*stream);
  } catch(...) {
    debug("Error ignored");
  }
  debug("Ending thread");
  delete stream;
}

void t_threaded_service::operator()(std::iostream & stream) {
}

void t_threaded_service::operator()(t_iostream_ptr & stream) {
  t_iostream_ptr * tmp = new t_iostream_ptr(stream);
  workers.create_thread(boost::bind(&t_threaded_service::launch, this, tmp));
}

void t_echo_service::operator()(std::iostream & stream) {
  stream << "Hi\n";
  while(stream) {
    std::string tmp;
    stream >> tmp;
    stream << "You wrote: " << tmp << std::endl;
  }
  debug("Ended Connection");
}

