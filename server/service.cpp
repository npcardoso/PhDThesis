#include "service.h"

#include "utils/debug.h"

#include <boost/thread/mutex.hpp>
#include <boost/foreach.hpp>

void t_service::operator()(const t_iostream_ptr & in,
                           const t_iostream_ptr & out) {
  (*this)(*in, *out);
}

t_threaded_service::t_threaded_service(t_service::t_ptr srv):srv(srv) {
}

void t_threaded_service::launch(t_iostream_ptr * in,
                                t_iostream_ptr * out) {
  try {
    (*srv)(*in, *out);
  } catch(...) {
    debug("Error ignored");
  }
  debug("Ending thread");
  delete in;
  delete out;
}

void t_threaded_service::operator ()(std::istream & in,
                                     std::ostream & out) {

}

void t_threaded_service::operator()(const t_iostream_ptr & in,
                                    const t_iostream_ptr & out) {
  t_iostream_ptr * tmp_in = new t_iostream_ptr(in);
  t_iostream_ptr * tmp_out = new t_iostream_ptr(out);
  workers.create_thread(boost::bind(&t_threaded_service::launch, this, tmp_in, tmp_out));
}

void t_echo_service::operator ()(std::istream & in, std::ostream & out) {
  out << "Hi\n";
  while(in) {
    std::string tmp;
    in >> tmp;
    out << "You wrote: " << tmp << std::endl;
  }
  debug("Ended Connection");
}

