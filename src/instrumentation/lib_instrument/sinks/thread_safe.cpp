#include "instrumentation/sinks/thread_safe.h"

namespace instrumentation {
namespace sinks {
t_thread_safe_construct::t_thread_safe_construct (t_ptr<t_construct_sink> sink) : sink(sink) {}

bool t_thread_safe_construct::operator << (const t_ptr<t_transaction_construct> & ctr) {
    return protect(ctr);
}

bool t_thread_safe_construct::operator << (const t_ptr<t_oracle_construct> & ctr) {
    return protect(ctr);
}

bool t_thread_safe_construct::operator << (const t_ptr<t_probe_construct> & ctr) {
    return protect(ctr);
}

t_thread_safe_observation::t_thread_safe_observation (t_ptr<t_observation_sink> sink) : sink(sink) {}

bool t_thread_safe_observation::operator << (const t_ptr<t_transaction_observation> & obs) {
    return protect(obs);
}

bool t_thread_safe_observation::operator << (const t_ptr<t_oracle_observation> & obs) {
    return protect(obs);
}

bool t_thread_safe_observation::operator << (const t_ptr<t_probe_observation> & obs) {
    return protect(obs);
}
}
}