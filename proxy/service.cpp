#include "service.h"

#include "utils/debug.h"

#include <boost/thread/mutex.hpp>

t_threaded_service::t_threaded_service(t_service::t_ptr srv):srv(srv){
}

void t_threaded_service::operator()(std::iostream * stream) {
  workers.create_thread(boost::bind(&t_service::operator(), srv, stream));
}

void t_echo_service::operator()(std::iostream * stream) {
  *stream << "Hi\n";
  while(*stream) {
    std::string tmp;
    *stream >> tmp;
    *stream << "You wrote: " << tmp << std::endl;
  }
  debug("Ended Connection");
  delete stream;
}
