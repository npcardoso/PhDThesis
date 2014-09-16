#include "threaded.h"

#include "../../debug.h"

#include <boost/thread/mutex.hpp>

t_threaded_service::t_threaded_service (t_ptr<t_service> srv) : srv(srv) {}

void t_threaded_service::launch (t_ptr<std::iostream> * in,
                                 t_ptr<std::iostream> * out) {
    try {
        (* srv)(* in,
                * out);
    }
    catch (...) {
        debug("Error ignored");
    }
    debug("Ending thread");
    delete in;
    delete out;
}

void t_threaded_service::operator () (std::istream & in,
                                      std::ostream & out) {}

void t_threaded_service::operator () (const t_ptr<std::iostream> & in,
                                      const t_ptr<std::iostream> & out) {
    t_ptr<std::iostream> * tmp_in = new t_ptr<std::iostream> (in);
    t_ptr<std::iostream> * tmp_out = new t_ptr<std::iostream> (out);


    workers.create_thread(boost::bind(&t_threaded_service::launch, this, tmp_in, tmp_out));
}